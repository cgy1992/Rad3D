#include "GrassModule.h"
#include "MainFrame.h"
#include "Editor.h"
#include "ToolUtil.h"
#include "MTerrainMesh.h"

GrassModule::GrassModule()
	: EditorModule("Grass")
{
	mBrushSize = 0.5f * UNIT_METRES;
	mMinSize = 0.5f * UNIT_METRES;
	mMaxSize = 0.8f * UNIT_METRES;
	mImageBox_Selected = NULL;

	mLayout = MGUI::Layout::Load("GrassPanel.layout", NULL);
	mLayout->SetVisible(false);

	mEdMinSize = (MGUI::EditBox *)mLayout->GetChild("MinSize");
	mEdMinSize->SetCaption(f2str(mMinSize, true, "%.2f").c_wstr());

	mEdMaxSize = (MGUI::EditBox *)mLayout->GetChild("MaxSize");
	mEdMaxSize->SetCaption(f2str(mMaxSize, true, "%.2f").c_wstr());

	mEdMinSize->E_KeyLostFocus += new cListener1<GrassModule, const MGUI::FocusEvent *>(this, &GrassModule::OnSize);
	mEdMaxSize->E_KeyLostFocus += new cListener1<GrassModule, const MGUI::FocusEvent *>(this, &GrassModule::OnSize);

	mGrid_Grass = new MGUI::GridBox(NULL, mLayout->GetChild("Grass"));
	mGrid_Grass->SetColumn(3);
	mGrid_Grass->SetAutoResizeType(MGUI::GridBox::NONE);
	mGrid_Grass->SetRect(0, 0, 3 * 70, 3 * 70);
	mGrid_Grass->SetGridSize(70, 70);
	mGrid_Grass->SetAlign(MGUI::eAlign::H_CENTER | MGUI::eAlign::V_STRETCH);

	mShaderFX_Brush = ShaderFXManager::Instance()->Load("TerrainBrush", "Shader/TerrainBrush.mfx");
	d_assert (mShaderFX_Brush != NULL && mShaderFX_Brush->GetPassCount());

	Editor::Instance()->E_Update += new cListener0<GrassModule>(this, &GrassModule::OnUpdate);
	Editor::Instance()->E_SceneUnload += new cListener0<GrassModule>(this, &GrassModule::OnSceneUnload);
	Editor::Instance()->E_SceneLoaded += new cListener0<GrassModule>(this, &GrassModule::OnSceenLoaded);
	World::Instance()->E_RenderSolid += new cListener0<GrassModule>(this, &GrassModule::OnRender);

	World::Instance()->E_SectionCurrentChanged += new cListener1<GrassModule, WorldSection *>(this, &GrassModule::OnSectionChanged);
}

GrassModule::~GrassModule()
{
	delete mLayout;
}

void GrassModule::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void GrassModule::Show()
{
	Layout();

	mLayout->SetVisible(true);

	Editor::Instance()->SetSelectNode(NULL);
	Editor::Instance()->ClearFlags(FLAG_ALL);
}

void GrassModule::Hide()
{
	mLayout->SetVisible(false);
}

void GrassModule::OnUpdate()
{
	if (!mLayout->IsVisible() || 
		!MainRenderView::Instance()->IsMouseFoucsed() ||
		mImageBox_Selected == NULL || mBrushSize < 0)
		return ;

	GrassSection * section = GrassManager::Instance()->_getCurrentSection();
	if (section == NULL)
		return ;

	Float2 pt = IMouse::Instance()->GetPosition();
	Editor::Instance()->MapScreenUnit(pt.x, pt.y);
	if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
		return ;

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
	if (!Terrain::Instance()->RayCheck(&mBrushPosition, ray))
		return ;

	RectF rect;
	rect.x1 = mBrushPosition.x - mBrushSize * 0.5f;
	rect.y1 = mBrushPosition.z - mBrushSize * 0.5f;
	rect.x2 = mBrushPosition.x + mBrushSize * 0.5f;
	rect.y2 = mBrushPosition.z + mBrushSize * 0.5f;

	Float3 position = mBrushPosition;
	int id = (int)mImageBox_Selected->GetUserData();

	if (IKeyboard::Instance()->KeyUp(InputCode::KC_ADD))
	{
		mBrushSize += 0.1f * UNIT_METRES;
	}
	else if (IKeyboard::Instance()->KeyUp(InputCode::KC_SUBTRACT))
	{
		mBrushSize -= 0.1f * UNIT_METRES;
		mBrushSize = Max(0.1f * UNIT_METRES, mBrushSize);
	}
	else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LCONTROL))
	{
		if (IMouse::Instance()->KeyPressed(InputCode::MKC_LEFT) && IMouse::Instance()->MouseMoved())
		{
			float dy = IMouse::Instance()->GetPositionDiff().y;

			mBrushSize += -dy * 0.05f * UNIT_METRES;
		}
	}
	else if (IMouse::Instance()->KeyUp(InputCode::MKC_LEFT) && IKeyboard::Instance()->KeyPressed(InputCode::KC_LSHIFT))
	{
		section->RemoveInstance(id, rect);
	}
	else if (IMouse::Instance()->KeyUp(InputCode::MKC_LEFT) &&
		section->GetWorldSection()->GetBound().Contain(position))
	{
		float h;
		Float3 n;

		if (Terrain::Instance()->GetHeightAt(h, position.x, position.z) &&
			Terrain::Instance()->GetNormalAt(n, position.x, position.z))
		{
			float size = Math::RandRange(mMinSize, mMaxSize);
			if (size > 0.1f)
			{
				GrassInstance gi;
				gi.Id = id;
				gi.Position = Float3(position.x, h, position.z);
				gi.Normal = n;
				gi.Size = size;
				gi.Rotation = Math::RandRange(0 , K_GRASS_ROTATION_SEED - 1);

				Float3 lightingColor = Float3(1, 1, 1);
				
				if (Terrain::Instance()->GetLightingColorAt(lightingColor, position.x, position.z))
				{
					gi.LightingColor = lightingColor;
				}
				
				section->AddInstance(gi);
			}
		}
	}
}

void GrassModule::OnRender()
{
	if (!mLayout->IsVisible() || mImageBox_Selected == NULL || !Terrain::Instance()->IsValid())
		return ;

	Aabb aabb;
	aabb.minimum = mBrushPosition - Float3(mBrushSize * 0.5f, 1, mBrushSize * 0.5f);
	aabb.maximum = mBrushPosition + Float3(mBrushSize * 0.5f, 1, mBrushSize * 0.5f);

	RenderSystem::Instance()->SetWorldTM(Mat4::Identity);

	for (int y = 0; y < Terrain::Instance()->GetInfo()->BlockCount.x; ++y)
	{
		for (int x = 0; x < Terrain::Instance()->GetInfo()->BlockCount.y; ++x)
		{
			TerrainMesh * mesh = Terrain::Instance()->_getBlock(x, y);
			const Aabb & bound = mesh->GetWorldAabb();

			float x0 = Max(aabb.minimum.x, bound.minimum.x);
			float z0 = Max(aabb.minimum.z, bound.minimum.z);

			float x1 = Min(aabb.maximum.x, bound.maximum.x);
			float z1 = Min(aabb.maximum.z, bound.maximum.z);

			if (x0 <= x1 && z0 <= z1)
			{
				mShaderFX_Brush->GetPass(0)->SetUniform("u_MinInvSize", Float4(aabb.minimum.x, aabb.minimum.z, 1 / mBrushSize, 1 / mBrushSize));
				mShaderFX_Brush->GetPass(0)->SetUniform("u_XZTransform", mesh->GetXZTransform());

				RenderSystem::Instance()->SetTexture(0, MGUI::Helper::Instance()->GetWhiteTexture().c_ptr());
				RenderSystem::Instance()->Render(mShaderFX_Brush, mesh->GetRenderOp());
			}
		}
	}
}

void GrassModule::OnSize(const MGUI::FocusEvent *)
{
	String t_minSize, t_maxSize;
	
	t_minSize.FromUnicode(mEdMinSize->GetCaption().c_str());
	t_maxSize.FromUnicode(mEdMaxSize->GetCaption().c_str());

	mMinSize = t_minSize.ToFloat();
	mMaxSize = t_maxSize.ToFloat();
}

void GrassModule::OnGrassClick(const MGUI::MouseEvent * e)
{
	if (mImageBox_Selected)
		mImageBox_Selected->SetColor(Float4(1.0f, 1.0f, 1.0f));

	mImageBox_Selected = (MGUI::ImageBox *)e->sender;

	if (mImageBox_Selected)
		mImageBox_Selected->SetColor(Float4(0.7f, 0.7f, 1.0f));
}

void GrassModule::OnSceenLoaded()
{
	/*
	for (int i = 0; i < GrassManager::Instance()->GetTemplateCount(); ++i)
	{
		const GrassTemplate * gs = GrassManager::Instance()->GetTemplate(i);
		TexturePtr grassTex = GrassManager::Instance()->GetTexture();

		if (gs->Id == -1)
			continue ;

		MGUI::Rect uv;
		uv.x = gs->UVRect.x1;
		uv.y = gs->UVRect.y1;
		uv.w = gs->UVRect.x2 - gs->UVRect.x1;
		uv.h = gs->UVRect.y2 - gs->UVRect.y1;

		MGUI::ImageBox * pImageBox = new MGUI::ImageBox(NULL, NULL);
		pImageBox->SetAlign(MGUI::eAlign::CENTER);
		pImageBox->SetRect(0, 0, 64, 64);
		pImageBox->SetSkinEx(grassTex);
		pImageBox->SetUVRect(uv);
		pImageBox->SetUserData((void *)gs->Id);
		pImageBox->E_MouseClick += new cListener1<GrassModule, const MGUI::MouseEvent *>(this, &GrassModule::OnGrassClick);

		mGrid_Grass->Append(pImageBox);
	}
	*/
}

void GrassModule::OnSceneUnload()
{
	mGrid_Grass->Clear();
	mImageBox_Selected = NULL;
}

void GrassModule::OnSectionChanged(WorldSection * ws)
{
	mGrid_Grass->Clear();
	mImageBox_Selected = NULL;

	GrassSection * section = ws->GetDataT<GrassSection>();
	if (section != NULL)
	{
		for (int i = 0; i < section->GetTemplateCount(); ++i)
		{
			const GrassTemplate * gs = section->GetTemplate(i);
			TexturePtr grassTex = section->_getTexture();

			if (gs->Id == -1)
				continue ;

			MGUI::Rect uv;
			uv.x = gs->UVRect.x1;
			uv.y = gs->UVRect.y1;
			uv.w = gs->UVRect.x2 - gs->UVRect.x1;
			uv.h = gs->UVRect.y2 - gs->UVRect.y1;

			MGUI::ImageBox * pImageBox = new MGUI::ImageBox(NULL, NULL);
			pImageBox->SetAlign(MGUI::eAlign::CENTER);
			pImageBox->SetRect(0, 0, 64, 64);
			pImageBox->SetSkinEx(grassTex);
			pImageBox->SetUVRect(uv);
			pImageBox->SetUserData((void *)gs->Id);
			pImageBox->E_MouseClick += new cListener1<GrassModule, const MGUI::MouseEvent *>(this, &GrassModule::OnGrassClick);

			mGrid_Grass->Append(pImageBox);
		}
	}
}
#include "TerrainLayerPanel.h"
#include "TerrainModule.h"
#include "MainFrame.h"
#include "Editor.h"
#include "MTerrainMesh.h"

TerrainLayerPanel::TerrainLayerPanel()
{
	mLayout = MGUI::Layout::Load("TerrainLayerPanel.layout", NULL);
	mLayout->SetVisible(false);

	// Back
	MGUI::Button * bn_Back = (MGUI::Button *)mLayout->GetChild("Back");
	bn_Back->E_MouseClick += new cListener1<TerrainLayerPanel, const MGUI::MouseEvent *>(this, &TerrainLayerPanel::OnBack);

	mEbSize = (MGUI::EditBox *)mLayout->GetChild("Size");
	mEbDensity = (MGUI::EditBox *)mLayout->GetChild("Density");

	mEbSize->E_KeyLostFocus += new cListener1<TerrainLayerPanel, const MGUI::FocusEvent *>(this, &TerrainLayerPanel::OnSize);
	mEbDensity->E_KeyLostFocus += new cListener1<TerrainLayerPanel, const MGUI::FocusEvent *>(this, &TerrainLayerPanel::OnDensity);

	mGrid_Brush = NULL;
	mImageBox_Brush = NULL;
	mImageBox_Layer = NULL;

	mGrid_Layer = new MGUI::GridBox(NULL, mLayout->GetChild("Layer"));
	mGrid_Layer->SetColumn(3);
	mGrid_Layer->SetAutoResizeType(MGUI::GridBox::NONE);
	mGrid_Layer->SetRect(0, 0, 3 * 70, 3 * 70);
	mGrid_Layer->SetGridSize(70, 70);
	mGrid_Layer->SetAlign(MGUI::eAlign::H_CENTER | MGUI::eAlign::V_STRETCH);

	//
	mBrushSize = 2 * UNIT_METRES;
	mBrushDensity = 0.2f;
	mBrushPosition = Float3(0, 0, 0);
	mShaderFX_Brush = ShaderFXManager::Instance()->Load("TerrainBrush", "Shader/TerrainBrush.mfx");
	d_assert (mShaderFX_Brush != NULL && mShaderFX_Brush->GetPassCount());

	mEbSize->SetCaption(f2str(mBrushSize, "%.1f").c_wstr());
	mEbDensity->SetCaption(f2str(mBrushDensity, "%.1f").c_wstr());

	//
	Editor::Instance()->E_LoadSetting += new cListener1<TerrainLayerPanel, xml_doc &>(this, &TerrainLayerPanel::OnProjectLoad);
	Editor::Instance()->E_ProjectUnload += new cListener0<TerrainLayerPanel>(this, &TerrainLayerPanel::OnProjectUnLoad);
	Editor::Instance()->E_SceneUnload += new cListener0<TerrainLayerPanel>(this, &TerrainLayerPanel::OnSceneUnload);
	Editor::Instance()->E_SceneLoaded += new cListener0<TerrainLayerPanel>(this, &TerrainLayerPanel::OnAfterSceneLoad);
	Editor::Instance()->E_Update += new cListener0<TerrainLayerPanel>(this, &TerrainLayerPanel::OnUpdate);

	World::Instance()->E_RenderSolid += new cListener0<TerrainLayerPanel>(this, &TerrainLayerPanel::OnRender);
}

TerrainLayerPanel::~TerrainLayerPanel()
{
	delete mLayout;
}

void TerrainLayerPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void TerrainLayerPanel::Show()
{
	Layout();

	mLayout->SetVisible(true);
	mLayout->RemoveAllComponent();

	float w = mLayout->GetRect().w;

	MGUI::KFAnimMove * pMoveKey = new MGUI::KFAnimMove;
	pMoveKey->Append(0, -w, 0);
	pMoveKey->Append(INDENT_TIME, 0, 0);

	mLayout->AddComponent(pMoveKey);
}

void TerrainLayerPanel::Hide()
{
	mLayout->SetVisible(false);
}

void TerrainLayerPanel::OnBack(const MGUI::MouseEvent *)
{
	Layout();

	mLayout->RemoveAllComponent();

	float w = mLayout->GetRect().w;

	MGUI::KFAnimMove * pMoveKey = new MGUI::KFAnimMove;
	pMoveKey->Append(0, 0, 0);
	pMoveKey->Append(INDENT_TIME, -w, 0);
	pMoveKey->E_KeyEnd += new cListener1<TerrainLayerPanel, MGUI::Component *>(this, &TerrainLayerPanel::OnBack_KeyEnd);

	mLayout->AddComponent(pMoveKey);
	mLayout->SetPickFlag(MGUI::ePickFlag::NONE);
}

void TerrainLayerPanel::OnBack_KeyEnd(MGUI::Component *)
{
	mLayout->SetVisible(false);
	mLayout->SetPickFlag(MGUI::ePickFlag::ALL);

	TerrainModule::Instance()->_showWithAnim();
}

void TerrainLayerPanel::OnSize(const MGUI::FocusEvent *)
{
	String text;
	text.FromUnicode(mEbSize->GetCaption().c_str());

	mBrushSize = text.ToFloat();
}

void TerrainLayerPanel::OnDensity(const MGUI::FocusEvent *)
{
	String text;
	text.FromUnicode(mEbDensity->GetCaption().c_str());

	mBrushDensity = text.ToFloat();
}

void TerrainLayerPanel::OnProjectLoad(xml_doc & doc)
{
	safe_delete (mGrid_Brush);
	mImageBox_Brush = NULL;

	mGrid_Brush = new MGUI::GridBox(NULL, mLayout->GetChild("Brush"));
	mGrid_Brush->SetAlign(MGUI::eAlign::CENTER);
	mGrid_Brush->SetColumn(3);
	mGrid_Brush->SetRect(MGUI::Rect(0, 0, 70 * 3, 70 * 2));
	mGrid_Brush->SetGridSize(70, 70);
	mGrid_Brush->SetAutoResizeType(MGUI::GridBox::NONE);

	xml_node node = doc.first_node().first_node("Brush");
	while (node != NULL)
	{
		const char * val = node.first_attribute("value");

		TexturePtr pSkin = HWBufferManager::Instance()->LoadTexture(val, -1);

		if (pSkin != NULL)
		{
			MGUI::ImageBox * pImageBox = new MGUI::ImageBox(NULL, NULL);
			pImageBox->SetSkin(val);
			pImageBox->SetRect(0, 0, 64, 64);
			pImageBox->E_MouseClick += new cListener1<TerrainLayerPanel, const MGUI::MouseEvent *>(this, &TerrainLayerPanel::OnBrushClick);

			mGrid_Brush->Append(pImageBox);
		}

		node = node.next_sibling("Brush");
	}
}

void TerrainLayerPanel::OnProjectUnLoad()
{
	safe_delete (mGrid_Brush);
	mImageBox_Brush = NULL;
}

void TerrainLayerPanel::OnSceneUnload()
{
	mGrid_Layer->Clear();
	mImageBox_Layer = NULL;
}

void TerrainLayerPanel::OnAfterSceneLoad()
{
	mGrid_Layer->Clear();

	for (int i = 0; i < Terrain::Instance()->GetLayerCount(); ++i)
	{
		const Terrain::Layer * layer = Terrain::Instance()->GetLayer(i);
		if (layer->DetailMap != "")
		{
			MGUI::ImageBox * pImageBox = new MGUI::ImageBox(NULL, NULL);
			pImageBox->SetSkin(layer->DetailMap);
			pImageBox->SetRect(0, 0, 64, 64);
			pImageBox->SetUserData((void *)i);
			pImageBox->E_MouseClick += new cListener1<TerrainLayerPanel, const MGUI::MouseEvent *>(this, &TerrainLayerPanel::OnLayerClick);

			mGrid_Layer->Append(pImageBox);
		}
	}
}

void TerrainLayerPanel::OnBrushClick(const MGUI::MouseEvent * e)
{
	MGUI::ImageBox * pImageBox = (MGUI::ImageBox *)e->sender;

	if (mImageBox_Brush == pImageBox)
		return ;

	if (mImageBox_Brush)
		mImageBox_Brush->SetColor(Float4(1, 1, 1));

	mImageBox_Brush = pImageBox;

	mImageBox_Brush->SetColor(Float4(0, 0, 1));
}

void TerrainLayerPanel::OnLayerClick(const MGUI::MouseEvent * e)
{
	MGUI::ImageBox * pImageBox = (MGUI::ImageBox *)e->sender;

	if (mImageBox_Layer == pImageBox)
		return ;

	if (mImageBox_Layer)
		mImageBox_Layer->SetColor(Float4(1, 1, 1));

	mImageBox_Layer = pImageBox;

	mImageBox_Layer->SetColor(Float4(0.5f, 0.5f, 1));
}

void TerrainLayerPanel::OnUpdate()
{
	if (!mLayout->IsVisible() ||
		!MainRenderView::Instance()->IsMouseFoucsed() ||
		!Terrain::Instance()->IsValid())
		return ;

	Float2 pt = IMouse::Instance()->GetPosition();

	Editor::Instance()->MapScreenUnit(pt.x, pt.y);
	if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
		return ;

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);

	if (!Terrain::Instance()->RayCheck(&mBrushPosition, ray))
		return ;

	if (!IMouse::Instance()->KeyPressed(InputCode::MKC_LEFT))
		return ;

	_UpdateGeometry();
}

void TerrainLayerPanel::_UpdateGeometry()
{
	if (mImageBox_Brush == NULL|| mImageBox_Brush->GetSkin() == NULL || mImageBox_Layer == NULL)
		return ;

	Float3 center = mBrushPosition;
	float size = mBrushSize;
	float density = mBrushDensity;
	int layerId = (int)mImageBox_Layer->GetUserData();

	float sx = center.x - size * 0.5f;
	float ex = center.x + size * 0.5f;
	float sz = center.z - size * 0.5f;
	float ez = center.z + size * 0.5f;

	const Terrain::Info * ti = Terrain::Instance()->GetInfo();

	int xWeightMapSize = ti->WMapSize * ti->BlockCount.x - 1;
	int zWeightMapSize = ti->WMapSize * ti->BlockCount.y - 1;

	int isx = (int)Math::Ceil(sx * ti->InvSize.x * xWeightMapSize);
	int iex = (int)((ex * ti->InvSize.x) * xWeightMapSize);
	int isz = (int)Math::Ceil((sz * ti->InvSize.y) * zWeightMapSize);
	int iez = (int)((ez * ti->InvSize.y) * zWeightMapSize);

	isx = Math::Clamp(isx, 0, xWeightMapSize);
	iex = Math::Clamp(iex, 0, xWeightMapSize);
	isz = Math::Clamp(isz, 0, zWeightMapSize);
	iez = Math::Clamp(iez, 0, zWeightMapSize);

	int index = 0;
	RectI rc = RectI(isx, isz, iex, iez);
	float * weights = Terrain::Instance()->LockWeightMap(rc);

	if (weights == NULL)
		return ;

	TexturePtr brushImage = mImageBox_Brush->GetSkin();
	int w = brushImage->GetWidth() - 1;
	int h = brushImage->GetHeight() - 1;

	for (int j = isz; j <= iez; ++j)
	{
		for (int i = isx; i <= iex; ++i)
		{
			float x = float(i) / xWeightMapSize * ti->Size.x;
			float z = float(j) / zWeightMapSize * ti->Size.y;

			if (x < 0 || x > ti->Size.x ||
				z < 0 || z > ti->Size.y)
				continue ;

			float u = (x - sx) / (ex - sx);
			float v = (z - sz) / (ez - sz);

			u = Math::Clamp(u, 0.0f, 1.0f);
			v = 1 - Math::Clamp(v, 0.0f, 1.0f);

			int iu = int(u * w);
			int iv = int(v * h);
			int iu1 = iu + 1;
			int iv1 = iv + 1;

			iu1 = Min(iu1, w);
			iv1 = Min(iv1, h);

			float du = u * w - iu;
			float dv = v * h - iv;

			Float4 c0, c1, c2, c3;
			brushImage->GetColorData(c0, iu,  iv);
			brushImage->GetColorData(c1, iu1,  iv);
			brushImage->GetColorData(c2, iu,  iv1);
			brushImage->GetColorData(c3, iu1,  iv1);

			Float4 cx0 = c0 + (c1 - c0) * du;
			Float4 cx1 = c2 + (c3 - c2) * du;

			Float4 cy = cx0 + (cx1 - cx0) * dv;

			weights[index++] = cy.r * density;
		}
	}

	Terrain::Instance()->UnlockWeightMap(layerId);
}

void TerrainLayerPanel::OnRender()
{
	if (!mLayout->IsVisible() || mImageBox_Brush == NULL)
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

				RenderSystem::Instance()->SetTexture(0, mImageBox_Brush->GetSkin().c_ptr());
				RenderSystem::Instance()->Render(mShaderFX_Brush, mesh->GetRenderOp());
			}
		}
	}
}

#include "TerrainHeightPanel.h"
#include "TerrainModule.h"
#include "MainFrame.h"
#include "Editor.h"
#include "MTerrainMesh.h"

TerrainHeightPanel::TerrainHeightPanel()
{
	mLayout = MGUI::Layout::Load("TerrainHeightPanel.layout", NULL);
	mLayout->SetVisible(false);

	// Back
	MGUI::Button * bn_Back = (MGUI::Button *)mLayout->GetChild("Back");
	bn_Back->E_MouseClick += new cListener1<TerrainHeightPanel, const MGUI::MouseEvent *>(this, &TerrainHeightPanel::OnBack);

	mEbSize = (MGUI::EditBox *)mLayout->GetChild("Size");
	mEbDensity = (MGUI::EditBox *)mLayout->GetChild("Density");
	
	mEbSize->E_KeyLostFocus += new cListener1<TerrainHeightPanel, const MGUI::FocusEvent *>(this, &TerrainHeightPanel::OnSize);
	mEbDensity->E_KeyLostFocus += new cListener1<TerrainHeightPanel, const MGUI::FocusEvent *>(this, &TerrainHeightPanel::OnDensity);

	mGrid_Brush = NULL;
	mImageBox_Brush = NULL;

	//
	mOp = OP_UP;
	mBrushSize = 5.0f * UNIT_METRES;
	mBrushDensity = 0.1f * UNIT_METRES;
	mBrushPosition = Float3(0, 0, 0);
	mShaderFX_Brush = ShaderFXManager::Instance()->Load("TerrainBrush", "Shader/TerrainBrush.mfx");
	d_assert (mShaderFX_Brush != NULL && mShaderFX_Brush->GetPassCount());

	mEbSize->SetCaption(f2str(mBrushSize, "%.1f").c_wstr());
	mEbDensity->SetCaption(f2str(mBrushDensity, "%.1f").c_wstr());

	//
	Editor::Instance()->E_LoadSetting += new cListener1<TerrainHeightPanel, xml_doc &>(this, &TerrainHeightPanel::OnProjectLoad);
	Editor::Instance()->E_ProjectUnload += new cListener0<TerrainHeightPanel>(this, &TerrainHeightPanel::OnProjectUnLoad);
	Editor::Instance()->E_Update += new cListener0<TerrainHeightPanel>(this, &TerrainHeightPanel::OnUpdate);

	World::Instance()->E_RenderSolid += new cListener0<TerrainHeightPanel>(this, &TerrainHeightPanel::OnRender);
}

TerrainHeightPanel::~TerrainHeightPanel()
{
	delete mLayout;
}

void TerrainHeightPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void TerrainHeightPanel::Show()
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

void TerrainHeightPanel::Hide()
{
	mLayout->SetVisible(false);
}

void TerrainHeightPanel::OnBack(const MGUI::MouseEvent *)
{
	Layout();

	mLayout->RemoveAllComponent();

	float w = mLayout->GetRect().w;

	MGUI::KFAnimMove * pMoveKey = new MGUI::KFAnimMove;
	pMoveKey->Append(0, 0, 0);
	pMoveKey->Append(INDENT_TIME, -w, 0);
	pMoveKey->E_KeyEnd += new cListener1<TerrainHeightPanel, MGUI::Component *>(this, &TerrainHeightPanel::OnBack_KeyEnd);

	mLayout->AddComponent(pMoveKey);
	mLayout->SetPickFlag(MGUI::ePickFlag::NONE);
}

void TerrainHeightPanel::OnBack_KeyEnd(MGUI::Component *)
{
	mLayout->SetVisible(false);
	mLayout->SetPickFlag(MGUI::ePickFlag::ALL);

	TerrainModule::Instance()->_showWithAnim();
}

void TerrainHeightPanel::OnSize(const MGUI::FocusEvent *)
{
	String text;
	text.FromUnicode(mEbSize->GetCaption().c_str());

	mBrushSize = text.ToFloat();
}

void TerrainHeightPanel::OnDensity(const MGUI::FocusEvent *)
{
	String text;
	text.FromUnicode(mEbDensity->GetCaption().c_str());

	mBrushDensity = text.ToFloat();
}

void TerrainHeightPanel::OnProjectLoad(xml_doc & doc)
{
	safe_delete (mGrid_Brush);
	mImageBox_Brush = NULL;

	mGrid_Brush = new MGUI::GridBox(NULL, mLayout->GetChild("Brush"));
	mGrid_Brush->SetAlign(MGUI::eAlign::CENTER);
	mGrid_Brush->SetColumn(3);
	mGrid_Brush->SetGridSize(70, 70);
	mGrid_Brush->SetRect(MGUI::Rect(0, 0, 70 * 3, 70 * 3));
	mGrid_Brush->SetAutoResizeType(MGUI::GridBox::NONE);

	xml_node node = doc.first_node().first_node("Brush");
	while (node != NULL)
	{
		const char * val = node.first_attribute("value");

		MGUI::ImageBox * pImageBox = new MGUI::ImageBox(NULL, NULL);
		pImageBox->SetSkinAligned(val);
		pImageBox->E_MouseClick += new cListener1<TerrainHeightPanel, const MGUI::MouseEvent *>(this, &TerrainHeightPanel::OnBrushClick);

		mGrid_Brush->Append(pImageBox);

		node = node.next_sibling("Brush");
	}
}

void TerrainHeightPanel::OnProjectUnLoad()
{
	safe_delete (mGrid_Brush);
	mImageBox_Brush = NULL;
}

void TerrainHeightPanel::OnBrushClick(const MGUI::MouseEvent * e)
{
	MGUI::ImageBox * pImageBox = (MGUI::ImageBox *)e->sender;
	if (mImageBox_Brush == pImageBox)
		return ;

	if (mImageBox_Brush)
		mImageBox_Brush->SetColor(Float4(1, 1, 1));

	mImageBox_Brush = pImageBox;

	mImageBox_Brush->SetColor(Float4(0, 0, 1));
}

void TerrainHeightPanel::OnUpdate()
{
	if (!mLayout->IsVisible())
		return ;

	if (!MainRenderView::Instance()->IsMouseFoucsed())
		return ;

	Float2 pt = IMouse::Instance()->GetPosition();

	Editor::Instance()->MapScreenUnit(pt.x, pt.y);
	if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
		return ;

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
	if (!IMouse::Instance()->KeyPressed(InputCode::MKC_MIDDLE))
	{
		Terrain::Instance()->RayCheck(&mBrushPosition, ray);
	}

	if (IMouse::Instance()->KeyPressed(InputCode::MKC_LEFT))
	{
		mOp = OP_UP;

		if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LSHIFT))
		{
			mOp = OP_DOWN;
		}
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LALT))
		{
			mOp = OP_SMOOTH;
		}

		_UpdateGeometry();
	}
	else if (IMouse::Instance()->KeyPressed(InputCode::MKC_MIDDLE))
	{
		mOp = OP_UP;

		if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LSHIFT))
		{
			mOp = OP_DOWN;
		}
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LALT))
		{
			mOp = OP_SMOOTH;
		}

		_UpdateGeometry();
	}
}

void TerrainHeightPanel::_UpdateGeometry()
{
	if (mImageBox_Brush == NULL || mImageBox_Brush->GetSkin() == NULL)
		return ;

	Float3 center = mBrushPosition;
	float size = mBrushSize;
	float density = mBrushDensity;

	// calculate brush box
	float sx = center.x - size * 0.5f;
	float sz = center.z - size * 0.5f;
	float ex = center.x + size * 0.5f;
	float ez = center.z + size * 0.5f;

	int isx = (int)Math::Ceil(sx * Terrain::Instance()->GetInfo()->InvGridSize);
	int iex = (int)(ex  * Terrain::Instance()->GetInfo()->InvGridSize);
	int isz = (int)Math::Ceil(sz  * Terrain::Instance()->GetInfo()->InvGridSize);
	int iez = (int)(ez  * Terrain::Instance()->GetInfo()->InvGridSize);

	isx = Max(isx, 0);
	isz = Max(isz, 0);
	iex = Max(iex, 0);
	iez = Max(iez, 0);

	int index = 0;
	RectI rc = RectI(isx, isz, iex, iez);
	if (!rc.Valid())
		return ;

	float * heights = Terrain::Instance()->LockHeight(rc);
	if (heights == NULL)
		return ;

	TexturePtr brushImage = mImageBox_Brush->GetSkin();
	int w = brushImage->GetWidth() - 1;
	int h = brushImage->GetHeight() - 1;

	for (int j = isz; j <= iez; ++j)
	{
		for (int i = isx; i <= iex; ++i)
		{
			float x = float(i) * Terrain::Instance()->GetInfo()->GridSize;
			float z = float(j) * Terrain::Instance()->GetInfo()->GridSize;

			if (x < 0 || z < 0)
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

			float d = cy.r * density;

			if (mOp == OP_UP)
				heights[index++] += d;
			else if (mOp == OP_DOWN)
				heights[index++] -= d;
			else if (mOp == OP_SMOOTH)
			{
				float h[5] = { 0, 0, 0, 0, 0 };

				h[4] = heights[index];

				h[0] = Terrain::Instance()->_getHeightClamp(i - 1, j + 0);
				h[1] = Terrain::Instance()->_getHeightClamp(i + 1, j + 0);
				h[2] = Terrain::Instance()->_getHeightClamp(i + 0, j - 1);
				h[3] = Terrain::Instance()->_getHeightClamp(i + 0, j + 1);

				float hAvg = (h[4] + h[0] + h[1] + h[2] + h[3]) / 5;

				heights[index++] = Math::Lerp(h[4], hAvg, d);
			}
		}
	}

	Terrain::Instance()->UnlockHeight();
}


void TerrainHeightPanel::OnRender()
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
				mShaderFX_Brush->GetPass(0)->SetConst("u_MinInvSize", Float4(aabb.minimum.x, aabb.minimum.z, 1 / mBrushSize, 1 / mBrushSize));
				mShaderFX_Brush->GetPass(0)->SetConst("u_XZTransform", mesh->GetXZTransform());

				RenderSystem::Instance()->SetTexture(0, mImageBox_Brush->GetSkin().c_ptr());
				RenderSystem::Instance()->Render(mShaderFX_Brush, mesh->GetRenderOp());
			}
		}
	}
}
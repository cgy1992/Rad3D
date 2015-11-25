#include "TerrainPickerPanel.h"
#include "TerrainModule.h"
#include "App.h"
#include "Editor.h"
#include "MainFrame.h"
#include "MTerrainMesh.h"
#include "MUtil.h"

TerrainPickerPanel::TerrainPickerPanel()
{
	mLayout = MGUI::Layout::Load("TerrainPickerPanel.layout", NULL);
	mLayout->SetVisible(false);

	mGrid_Layer = new MGUI::GridBox(NULL, mLayout->GetChild("Layer"));
	mGrid_Layer->SetColumn(3);
	mGrid_Layer->SetAutoResizeType(MGUI::GridBox::NONE);
	mGrid_Layer->SetRect(0, 0, 3 * 70, 3 * 70);
	mGrid_Layer->SetGridSize(70, 70);
	mGrid_Layer->SetAlign(MGUI::eAlign::CENTER);

	mImageBox_Layer = NULL;

	MGUI::Button * bnBack = (MGUI::Button *)mLayout->GetChild("Back");
	bnBack->E_Click += new cListener1<TerrainPickerPanel, const MGUI::ClickEvent *>(this, &TerrainPickerPanel::OnBack);

	MGUI::Button * bnRemove = (MGUI::Button *)mLayout->GetChild("Remove");
	bnRemove->E_Click += new cListener1<TerrainPickerPanel, const MGUI::ClickEvent *>(this, &TerrainPickerPanel::OnRemove);

	mCheckBox_Unused = (MGUI::CheckBox *)mLayout->GetChild("Unused");
	mCheckBox_Unused->E_Checked += new cListener2<TerrainPickerPanel, const MGUI::Event *, bool>(this, &TerrainPickerPanel::OnUnused);

	Editor::Instance()->E_SceneUnload += new cListener0<TerrainPickerPanel>(this, &TerrainPickerPanel::OnSceneUnload);
	Editor::Instance()->E_SceneLoaded += new cListener0<TerrainPickerPanel>(this, &TerrainPickerPanel::OnSceneLoaded);

	MainRenderView::Instance()->GetLayout()->E_MouseDoubleClick += new cListener1<TerrainPickerPanel, const MGUI::MouseEvent *>(this, &TerrainPickerPanel::OnPick);

	World::Instance()->E_RenderSolid += new cListener0<TerrainPickerPanel>(this, &TerrainPickerPanel::OnRender);

	mRenderOp = NULL;
	mShaderFX = NULL;

	mPickedSection = NULL;
}

TerrainPickerPanel::~TerrainPickerPanel()
{
	safe_delete_array (mRenderOp);

	delete mLayout;
}

void TerrainPickerPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void TerrainPickerPanel::Show()
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

void TerrainPickerPanel::Hide()
{
	mLayout->SetVisible(false);
}

void TerrainPickerPanel::OnBack(const MGUI::ClickEvent * e)
{
	Layout();

	mLayout->RemoveAllComponent();

	float w = mLayout->GetRect().w;

	MGUI::KFAnimMove * pMoveKey = new MGUI::KFAnimMove;
	pMoveKey->Append(0, 0, 0);
	pMoveKey->Append(INDENT_TIME, -w, 0);
	pMoveKey->E_KeyEnd += new cListener1<TerrainPickerPanel, MGUI::Component *>(this, &TerrainPickerPanel::OnBack_KeyEnd);

	mLayout->AddComponent(pMoveKey);
	mLayout->SetPickFlag(MGUI::ePickFlag::NONE);
}

void TerrainPickerPanel::OnBack_KeyEnd(MGUI::Component * cp)
{
	mLayout->SetVisible(false);
	mLayout->SetPickFlag(MGUI::ePickFlag::ALL);

	TerrainModule::Instance()->_showWithAnim();
}

#define TN_MAKE_FLAG(x, y, i) (int)((x << 20) | (y << 8) | i)
#define TN_GET_INDEX(flag) (int)((flag) & 0xFF)
#define TN_GET_SECTION_X(flag) (int)((flag) >> 20)
#define TN_GET_SECTION_Z(flag) (int)((flag & 0xFFF00) >> 8)

void TerrainPickerPanel::OnPick(const MGUI::MouseEvent * e)
{
	if (!mLayout->IsVisible() ||
		!Terrain::Instance()->IsValid())
		return ;

	Float2 pt = IMouse::Instance()->GetPosition();

	Editor::Instance()->MapScreenUnit(pt.x, pt.y);
	if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
		return ;

	Terrain * pTerrain = Terrain::Instance();;
	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);

	Float3 position;
	if (!pTerrain->RayCheck(&position, ray))
		return ;

	int xIndex = int(position.x * pTerrain->GetInfo()->InvBlockSize);
	int zIndex = int(position.z * pTerrain->GetInfo()->InvBlockSize);

	if (xIndex >= 0 && xIndex < pTerrain->GetInfo()->BlockCount.x &&
		zIndex >= 0 && zIndex < pTerrain->GetInfo()->BlockCount.y)
	{
		mPickedSection = pTerrain->_getBlock(xIndex, zIndex);

		mImageBox_Layer = NULL;
		mGrid_Layer->Clear();
		for (int i = 0; i < 4; ++i)
		{
			int layer = mPickedSection->GetLayer(i);

			MGUI::ImageBox * pImageBox = new MGUI::ImageBox(NULL, NULL);
			pImageBox->SetRect(0, 0, 64, 64);
			pImageBox->SetSkinEx(pTerrain->_getDetailMap(layer));
			pImageBox->SetUserData((void *)TN_MAKE_FLAG(xIndex, zIndex, i));
			pImageBox->E_MouseClick += new cListener1<TerrainPickerPanel, const MGUI::MouseEvent *>(this, &TerrainPickerPanel::OnSelectLayer);

			mGrid_Layer->Append(pImageBox);
		}

		mCheckBox_Unused->SetChecked(mPickedSection->IsUnused());

		_updateGeomety(xIndex, zIndex);
	}
}

void TerrainPickerPanel::OnRemove(const MGUI::ClickEvent * e)
{
	if (mImageBox_Layer == NULL)
		return ;

	int flag = (int)mImageBox_Layer->GetUserData();
	int i = TN_GET_INDEX(flag);
	int x = TN_GET_SECTION_X(flag);
	int z = TN_GET_SECTION_Z(flag);

	TerrainMesh * pSection = Terrain::Instance()->_getBlock(x, z);

	pSection->SetLayer(i, -1);

	mImageBox_Layer->SetSkinEx(Terrain::Instance()->_getDetailMap(-1));
}

void TerrainPickerPanel::OnUnused(const MGUI::Event * e, bool checked)
{
	if (mPickedSection != NULL)
	{
		mPickedSection->SetUnused(checked);

		mGrid_Layer->Clear();
		mImageBox_Layer = NULL;
	}
}

void TerrainPickerPanel::OnSelectLayer(const MGUI::MouseEvent * e)
{
	MGUI::ImageBox * pImageBox = (MGUI::ImageBox *)e->sender;

	if (mImageBox_Layer == pImageBox)
		return ;

	if (mImageBox_Layer)
		mImageBox_Layer->SetColor(Float4(1, 1, 1));

	mImageBox_Layer = pImageBox;

	mImageBox_Layer->SetColor(Float4(0.5f, 0.5f, 1));
}

void TerrainPickerPanel::OnSceneUnload()
{
	mGrid_Layer->Clear();
	mImageBox_Layer = NULL;
	mPickedSection = NULL;

	safe_delete_array(mRenderOp);
}

void TerrainPickerPanel::OnSceneLoaded()
{
	mGrid_Layer->Clear();
	mImageBox_Layer = NULL;
	mPickedSection = NULL;

	safe_delete_array(mRenderOp);
}

void TerrainPickerPanel::_updateGeomety(int xIndex, int zIndex)
{
	if (mRenderOp == NULL)
	{
		mShaderFX = ShaderFXManager::Instance()->Load("TerrainBorder", "Shader/TerrainBorder.mfx");
		d_assert(mShaderFX != NULL);

		mRenderOp = new RenderOp[4];

		mRenderOp[0].vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		mRenderOp[1].vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		mRenderOp[2].vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		mRenderOp[3].vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

		mRenderOp[0].vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(Float3), Terrain::kMeshVertexCount);
		mRenderOp[1].vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(Float3), Terrain::kMeshVertexCount);
		mRenderOp[2].vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(Float3), Terrain::kMeshVertexCount);
		mRenderOp[3].vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(Float3), Terrain::kMeshVertexCount);

		mRenderOp[0].primCount = Terrain::kMeshGridCount;
		mRenderOp[1].primCount = Terrain::kMeshGridCount;
		mRenderOp[2].primCount = Terrain::kMeshGridCount;
		mRenderOp[3].primCount = Terrain::kMeshGridCount;
		
		mRenderOp[0].primType = ePrimType::LINE_STRIP;
		mRenderOp[1].primType = ePrimType::LINE_STRIP;
		mRenderOp[2].primType = ePrimType::LINE_STRIP;
		mRenderOp[3].primType = ePrimType::LINE_STRIP;
	}

	Terrain * pTerrain = Terrain::Instance();
	Float3 * v = (Float3 *)mRenderOp[0].vertexBuffers[0]->Lock(eLockFlag::WRITE);
	{
		int sx = (xIndex + 0) * Terrain::kMeshGridCount;
		int ex = (xIndex + 1) * Terrain::kMeshGridCount;
		int y = (zIndex + 0) * Terrain::kMeshGridCount;

		for (int x = sx; x <= ex; ++x)
		{
			*v++ = pTerrain->_getPosition(x, y) + Float3(0, 0.01f * UNIT_METRES, 0);
		}
	}
	mRenderOp[0].vertexBuffers[0]->Unlock();

	v = (Float3 *)mRenderOp[1].vertexBuffers[0]->Lock(eLockFlag::WRITE);
	{
		int sx = (xIndex + 0) * Terrain::kMeshGridCount;
		int ex = (xIndex + 1) * Terrain::kMeshGridCount;
		int y = (zIndex + 1) * Terrain::kMeshGridCount;

		for (int x = sx; x <= ex; ++x)
		{
			*v++ = pTerrain->_getPosition(x, y) + Float3(0, 0.01f * UNIT_METRES, 0);
		}
	}
	mRenderOp[1].vertexBuffers[0]->Unlock();

	v = (Float3 *)mRenderOp[2].vertexBuffers[0]->Lock(eLockFlag::WRITE);
	{
		int x = (xIndex + 0) * Terrain::kMeshGridCount;
		int sy = (zIndex + 0) * Terrain::kMeshGridCount;
		int ey = (zIndex + 1) * Terrain::kMeshGridCount;

		for (int y = sy; y <= ey; ++y)
		{
			*v++ = pTerrain->_getPosition(x, y) + Float3(0, 0.01f * UNIT_METRES, 0);
		}
	}
	mRenderOp[2].vertexBuffers[0]->Unlock();

	v = (Float3 *)mRenderOp[3].vertexBuffers[0]->Lock(eLockFlag::WRITE);
	{
		int x = (xIndex + 1) * Terrain::kMeshGridCount;
		int sy = (zIndex + 0) * Terrain::kMeshGridCount;
		int ey = (zIndex + 1) * Terrain::kMeshGridCount;

		for (int y = sy; y <= ey; ++y)
		{
			*v++ = pTerrain->_getPosition(x, y) + Float3(0, 0.01f * UNIT_METRES, 0);
		}
	}
	mRenderOp[3].vertexBuffers[0]->Unlock();
}

void TerrainPickerPanel::OnRender()
{
	if (!mLayout->IsVisible() || mRenderOp == NULL)
		return ;

	RenderSystem::Instance()->SetWorldTM(Mat4::Identity);

	RenderSystem::Instance()->Render(mShaderFX, &mRenderOp[0]);
	RenderSystem::Instance()->Render(mShaderFX, &mRenderOp[1]);
	RenderSystem::Instance()->Render(mShaderFX, &mRenderOp[2]);
	RenderSystem::Instance()->Render(mShaderFX, &mRenderOp[3]);
}

#include "stdafx.h"

#include "TerrainWaterPanel.h"
#include "MainFrame.h"
#include "Editor.h"


TerrainWaterPanel * TerrainWaterPanel::msInstance = NULL;

TerrainWaterPanel::TerrainWaterPanel()
{
	msInstance = this;

	mLayout = MGUI_LayoutLoader::Load("TerrainWaterPanel.layout", NULL);
	mLayout->SetVisible(false);

	mTopWidget = mLayout->GetChild("Caption");

	MGUI_TextBox * pCaption = new MGUI_TextBox(mTopWidget);
	pCaption->SetAlign(MGUI_Align::Center);
	pCaption->SetCaption(L"Terrain Water");

	MGUI_TextBox * pSize = new MGUI_TextBox(mLayout->GetChild("Text_Size"));
	pSize->SetAlign(MGUI_Align::Left | MGUI_Align::VCenter);
	pSize->SetCaption(L"Size:");

	mEdSize = (MGUI_EditBox *)mLayout->GetChild("Size");
	mEdSize->SetCaption(L"5");
	mEdSize->eventKeyLostFocus += new cListener1<TerrainWaterPanel, const MGUI_FocusEvent *>(this, &TerrainWaterPanel::OnSize);

	mBnBack = (MGUI_Button *)mLayout->GetChild("Back");
	mBnBack->eventMouseClick += new cListener1<TerrainWaterPanel, const MGUI_MouseEvent *>(this, &TerrainWaterPanel::OnBack);

	MGUI_TextBox * pSet = new MGUI_TextBox(mLayout->GetChild("T_Set"));
	pSet->SetAlign(MGUI_Align::Left | MGUI_Align::VCenter);
	pSet->SetCaption(L"Set");

	mCbSet = (MGUI_CheckBox *)mLayout->GetChild("Set");
	mCbSet->SetChecked(true);

	mGrid_Water = new MGUI_Grid(mLayout->GetChild("Layer"));
	mGrid_Water->SetColumn(3);
	mGrid_Water->SetAutoResizeType(MGUI_Grid::A_NONE);
	mGrid_Water->SetRect(0, 0, 3 * 70, 3 * 70);
	mGrid_Water->SetGridSize(70, 70);
	mGrid_Water->SetAlign(MGUI_Align::Center);

	MGUI_Button * bn_Add = (MGUI_Button *)mLayout->GetChild("Add");
	MGUI_Button * bn_Remove = (MGUI_Button *)mLayout->GetChild("Remove");
	MGUI_Button * bn_Edit = (MGUI_Button *)mLayout->GetChild("Edit");

	bn_Add->SetCaption(L"New");
	bn_Remove->SetCaption(L"Delete");
	bn_Edit->SetCaption(L"Edit");

	bn_Add->eventMouseClick += new cListener1<TerrainWaterPanel, const MGUI_MouseEvent *>(this, &TerrainWaterPanel::OnAdd);
	bn_Remove->eventMouseClick += new cListener1<TerrainWaterPanel, const MGUI_MouseEvent *>(this, &TerrainWaterPanel::OnRemove);
	bn_Edit->eventMouseClick += new cListener1<TerrainWaterPanel, const MGUI_MouseEvent *>(this, &TerrainWaterPanel::OnEdit);

	mBnAdd = bn_Add;
	mBnRemove = bn_Remove;
	mBnEdit = bn_Edit;

	mTerrainWaterDlg = new TerrainWaterDialog;
	mTerrainWaterDlg->eventEndDialog += new cListener1<TerrainWaterPanel, bool>(this, &TerrainWaterPanel::OnWaterDialog_EndDialog);

	mBrushSize = 5;
	mImageBox_Selected = NULL;
	mRenderOp_Brush = NULL;
	mRenderOp_Water = NULL;

	InitBrush();

	Editor::Instance()->eventUpdate += new cListener0<TerrainWaterPanel>(this, &TerrainWaterPanel::OnUpdate);
	Editor::Instance()->eventSceneUnload += new cListener0<TerrainWaterPanel>(this, &TerrainWaterPanel::OnSceneUnload);
	World::Instance()->MainRenderContext()->GetRenderPipeline()->eventDrawSolid += new cListener0<TerrainWaterPanel>(this, &TerrainWaterPanel::OnRender);
}

TerrainWaterPanel::~TerrainWaterPanel()
{
	safe_delete(mRenderOp_Brush);
	safe_delete(mRenderOp_Water);

	delete mTerrainWaterDlg;
	delete mLayout;
	msInstance = NULL;
}

void TerrainWaterPanel::Layout()
{
	MGUI_Rect rect = MGUI_Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(rect.w - w, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void TerrainWaterPanel::Show()
{
	Layout();

	mLayout->SetVisible(true);

	mLayout->DeleteAllComponent();

	float w = mLayout->GetRect().w;

	MGUI_Component_MoveKey * pMoveKey = new MGUI_Component_MoveKey;
	pMoveKey->AddKeyFrame(0, w, 0);
	pMoveKey->AddKeyFrame(INDENT_TIME, 0, 0);

	mLayout->AddComponent(pMoveKey);

	mLayout->Move(w, 0);
}

void TerrainWaterPanel::Hide()
{
	mLayout->SetVisible(false);
}

void TerrainWaterPanel::InitBrush()
{
	mRenderOp_Brush = new RenderOp;

	int iVertexCount = 4;

	VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(sizeof(float) * 3, iVertexCount);
	Float3 * vert = (Float3 *)vb->Lock(eLockFlag::WRITE);
	{
		*vert++ = Float3(-0.5f, 0, -0.5f);
		*vert++ = Float3(+0.5f, 0, -0.5f);
		*vert++ = Float3(-0.5f, 0, +0.5f);
		*vert++ = Float3(+0.5f, 0, +0.5f);
	}
	vb->Unlock();

	mRenderOp_Brush->vertexDecl.AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
	mRenderOp_Brush->vertexBuffer = vb;
	mRenderOp_Brush->primCount = 2;
	mRenderOp_Brush->primType = ePrimType::TRIANGLE_STRIP;

	mShaderFX_WaterGrid = ShaderFXManager::Instance()->Load("Shader/TerrainWaterGrid.mfx");
	mShaderFX_WaterBrush = ShaderFXManager::Instance()->Load("Shader/TerrainWaterBrush.mfx");

	d_assert(mShaderFX_WaterBrush->GetPassCount() && mShaderFX_WaterGrid->GetPassCount());
}

void TerrainWaterPanel::OnWaterSelChanged()
{
	safe_delete (mRenderOp_Water);

	if (mImageBox_Selected == NULL)
		return ;

	Water * pWater = (Water *)mImageBox_Selected->GetUserData();

	mRenderOp_Water = new RenderOp;

	mRenderOp_Water->vertexDecl.AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

	int iVertexCount = (pWater->GetConfig().xGridCount + 1) * 2 + (pWater->GetConfig().zGridCount + 1) * 2;
	
	mRenderOp_Water->vertexBuffer = VideoBufferManager::Instance()->CreateVertexBuffer(sizeof(float) * 3, iVertexCount);

	Float3 * vert = (Float3 *)mRenderOp_Water->vertexBuffer->Lock(eLockFlag::WRITE);
	{
		float z_step = 1.0f / pWater->GetConfig().zGridCount;

		for (int i = 0; i < pWater->GetConfig().zGridCount + 1; ++i)
		{
			*vert++ = Float3(0, 0, i * z_step); *vert++ = Float3(1, 0, i * z_step);
		}

		float x_step = 1.0f / pWater->GetConfig().xGridCount;

		for (int i = 0; i < pWater->GetConfig().xGridCount + 1; ++i)
		{
			*vert++ = Float3(i * x_step, 0, 0); *vert++ = Float3(i * x_step, 0, 1);
		}
	}
	mRenderOp_Water->vertexBuffer->Unlock();

	mRenderOp_Water->primCount = iVertexCount / 2;
	mRenderOp_Water->primType = ePrimType::LINE_LIST;
}

void TerrainWaterPanel::OnUpdate()
{
	if (!mLayout->GetVisible())
		return ;

	if (!MainRenderView::Instance()->IsMouseFoucsed() ||
		mImageBox_Selected == NULL)
		return ;

	if (Editor::Instance()->GetMode() != eMode_Terrain)
		return ;

	Float2 pt = IMouse::Instance()->GetPositionUnit();

	if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
		return ;

	Water * pWater = (Water *)mImageBox_Selected->GetUserData();

	Plane plane = Plane(Float3(0, 1, 0), -pWater->GetPosition().y);
	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
	float gridSize = pWater->GetConfig().GridSize;

	float dist = 0;

	if (!ray.Intersection(dist, plane))
		return ;

	Float3 position = ray.GetPosition(dist);

	int x, z;

	pWater->MapCoord(x, z, position.x, position.z);

	mBrushPosition.x = x * gridSize + gridSize / 2 + pWater->GetPosition().x;
	mBrushPosition.z = z * gridSize + gridSize / 2 + pWater->GetPosition().z;
	mBrushPosition.y = position.y;

	if (mBrushSize <= 0 || !IMouse::Instance()->KeyUp(MKC_BUTTON0))
		return ;
		
	int halfBSize = mBrushSize / 2;
	char flag = mCbSet->GetChecked() ? 1 : 0;
	int sizeX = pWater->GetConfig().xGridCount;
	int sizeZ = pWater->GetConfig().zGridCount;

	for (int j = 0; j < mBrushSize; ++j)
	{
		for (int i = 0; i < mBrushSize; ++i)
		{
			int _x = x + i - halfBSize;
			int _z = z + j - halfBSize;

			if (_x < 0 || _x > sizeX - 1 ||
				_z < 0 || _z > sizeZ - 1)
				continue ;

			pWater->SetData(_x, _z, flag);
		}
	}

	Rect2 rect = Rect2(x - halfBSize, z - halfBSize, x + halfBSize, z + halfBSize);
	pWater->NotifyChanged(rect);
}

void TerrainWaterPanel::OnRender()
{
	if (!mLayout->GetVisible() || mImageBox_Selected == NULL)
		return ;

	if (mRenderOp_Brush && mBrushSize > 0)
	{
		Water * pWater = (Water *)mImageBox_Selected->GetUserData();

		Float3 Position = mBrushPosition;
		Float3 Scale = Float3(mBrushSize * pWater->GetConfig().GridSize, 1, mBrushSize * pWater->GetConfig().GridSize);

		Mat4 worldTM;
		worldTM.MakeTransform(Position, Quat::Identity, Scale);
		RenderSystem::Instance()->SetWorldTM(worldTM);

		RenderSystem::Instance()->Render(mShaderFX_WaterBrush, mRenderOp_Brush);
	}

	if (mRenderOp_Water)
	{
		Water * pWater = (Water *)mImageBox_Selected->GetUserData();

		Float3 Position = pWater->GetPosition();
		Float3 Scale = Float3(pWater->GetConfig().A_xSize, 1, pWater->GetConfig().A_zSize);

		Mat4 worldTM;
		worldTM.MakeTransform(Position, Quat::Identity, Scale);
		RenderSystem::Instance()->SetWorldTM(worldTM);

		RenderSystem::Instance()->Render(mShaderFX_WaterBrush, mRenderOp_Water);
	}
	
}

void TerrainWaterPanel::OnBack(const MGUI_MouseEvent *)
{
	Layout();

	mLayout->DeleteAllComponent();

	float w = mLayout->GetRect().w;

	MGUI_Component_MoveKey * pMoveKey = new MGUI_Component_MoveKey;
	pMoveKey->AddKeyFrame(0, 0, 0);
	pMoveKey->AddKeyFrame(INDENT_TIME, w, 0);
	pMoveKey->eventKeyEnd += new cListener1<TerrainWaterPanel, MGUI_Component *>(this, &TerrainWaterPanel::OnBack_KeyEnd);

	mLayout->AddComponent(pMoveKey);
	mLayout->SetPickFlag(MGUI_PickFlag::None);
}

void TerrainWaterPanel::OnBack_KeyEnd(MGUI_Component *)
{
	mLayout->SetVisible(false);
	mLayout->SetPickFlag(MGUI_PickFlag::All);

	TerrainLayout::Instance()->_showWithAnim();
}

void TerrainWaterPanel::OnSize(const MGUI_FocusEvent *)
{
	TString128 text = TString128::FromUnicode(mEdSize->GetCaption().c_str());

	mBrushSize = text.ToInt();
}

void TerrainWaterPanel::OnAdd(const MGUI_MouseEvent *)
{
	mTerrainWaterDlg->DoModal(NULL, mBnAdd);
}

void TerrainWaterPanel::OnRemove(const MGUI_MouseEvent *)
{
	if (mImageBox_Selected)
	{
		MessageBoxDialog::Instance()->eventEndDialog += new cListener1<TerrainWaterPanel, bool>(this, &TerrainWaterPanel::OnRemove_EndDialog);
		MessageBoxDialog::Instance()->DoModal("Delete The Water?", MessageBoxDialog::T_Asking);
	}
}

void TerrainWaterPanel::OnRemove_EndDialog(bool _ok)
{
	if (!_ok || mImageBox_Selected == NULL)
		return ;

	Water * pWater = (Water *)mImageBox_Selected->GetUserData();

	mGrid_Water->Remove(mImageBox_Selected);

	Environment::Instance()->GetWaterManager()->DeleteWater(pWater);

	mImageBox_Selected = NULL;
	OnWaterSelChanged();
}

void TerrainWaterPanel::OnEdit(const MGUI_MouseEvent *)
{
	if (mImageBox_Selected == NULL)
		return ;

	Water * pWater = (Water *)mImageBox_Selected->GetUserData();

	mTerrainWaterDlg->DoModal(pWater, mBnEdit);
}

void TerrainWaterPanel::OnWaterDialog_EndDialog(bool _ok)
{
	if (!_ok)
		return ;

	if (mTerrainWaterDlg->GetUserData() == mBnAdd)
	{
		Water::Config cfg;
		Float3 pos;

		mTerrainWaterDlg->GetConfig(cfg);
		pos = mTerrainWaterDlg->GetPosition();

		Water * w = Environment::Instance()->GetWaterManager()->NewWater(cfg);
		w->SetPosition(pos);

		TexturePtr tex = w->GetWaveTexture(0);

		MGUI_ImageBox * pImageBox = new MGUI_ImageBox(NULL);
		pImageBox->SetAlign(MGUI_Align::Center);
		pImageBox->SetRect(0, 0, 64, 64);
		pImageBox->SetSkinEx(tex);
		pImageBox->SetUserData(w);
		pImageBox->SetAlpha(cfg.BaseAlpha);
		pImageBox->eventMouseClick += new cListener1<TerrainWaterPanel, const MGUI_MouseEvent *>(this, &TerrainWaterPanel::OnWaterClick);
		mGrid_Water->Append(pImageBox);
	}
	else if (mTerrainWaterDlg->GetUserData() == mBnEdit)
	{
		Water::Config cfg;
		Float3 pos;

		mTerrainWaterDlg->GetConfig(cfg);
		pos = mTerrainWaterDlg->GetPosition();

		Water * pWater = (Water *)mImageBox_Selected->GetUserData();

		pWater->ResetConfig(cfg);
		pWater->SetPosition(pos);
	}
}

void TerrainWaterPanel::OnWaterClick(const MGUI_MouseEvent * e)
{
	MGUI_ImageBox * pImageBox = RTTI_StaticCast(MGUI_ImageBox, e->sender);

	if (mImageBox_Selected == pImageBox)
		return ;

	if (mImageBox_Selected)
	{
		float a = mImageBox_Selected->GetAlpha();
		mImageBox_Selected->SetColor(MGUI_Color(1, 1, 1, a));
	}

	mImageBox_Selected = pImageBox;

	if (mImageBox_Selected)
	{
		float a = mImageBox_Selected->GetAlpha();
		mImageBox_Selected->SetColor(MGUI_Color(0.7f, 0.7f, 1, a));

		OnWaterSelChanged();
	}
}

void TerrainWaterPanel::OnCustomSerialize(bool _isSave)
{
	if (!_isSave)
	{
		TString128 sceneFile = Editor::Instance()->GetSceneFilename();
		TString128 path = FileHelper::GetFileDir(sceneFile);
		TString128 name = FileHelper::GetBaseName(sceneFile);
		name = FileHelper::RemoveExternName(name);
		name += ".water";

		TString128 fullname = path + "/" + name;

		fullname = ResourceManager::Instance()->GetRelativeSourceName(fullname);

		if (ResourceManager::Instance()->HasResource(fullname))
		{
			Environment::Instance()->GetWaterManager()->Load(fullname.c_str());
		}

		for (int i = 0; i < Environment::Instance()->GetWaterManager()->GetWaterCount(); ++i)
		{
			Water * w = Environment::Instance()->GetWaterManager()->GetWater(i);

			TexturePtr tex = w->GetWaveTexture(0);

			MGUI_ImageBox * pImageBox = new MGUI_ImageBox(NULL);
			pImageBox->SetAlign(MGUI_Align::Center);
			pImageBox->SetRect(0, 0, 64, 64);
			pImageBox->SetSkinEx(tex);
			pImageBox->SetUserData(w);
			pImageBox->SetAlpha(w->GetConfig().BaseAlpha);
			pImageBox->eventMouseClick += new cListener1<TerrainWaterPanel, const MGUI_MouseEvent *>(this, &TerrainWaterPanel::OnWaterClick);

			mGrid_Water->Append(pImageBox);
		}
	}
	else
	{
		TString128 sceneFile = Editor::Instance()->GetSceneFilename();
		TString128 path = FileHelper::GetFileDir(sceneFile);
		TString128 name = FileHelper::GetBaseName(sceneFile);
		name = FileHelper::RemoveExternName(name);
		name += ".water";

		TString128 fullname = path + "/" + name;

		if (Environment::Instance()->GetWaterManager()->GetWaterCount() > 0)
			Environment::Instance()->GetWaterManager()->Save(fullname.c_str());
	}
}

void TerrainWaterPanel::OnSceneUnload()
{
	Environment::Instance()->GetWaterManager()->Clear();
	mGrid_Water->Clear();
	mImageBox_Selected = NULL;
	OnWaterSelChanged();
}
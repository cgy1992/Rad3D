#include "WaterModule.h"
#include "MainFrame.h"
#include "Editor.h"
#include "App.h"

ImplementSingleton(WaterModule);

WaterModule::WaterModule()
	: EditorModule("Water")
{
	mLayout = MGUI::Layout::Load("Water.layout", NULL);
	mLayout->SetVisible(false);

	mLbWater = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), mLayout->GetChild("WaterList"));
	mLbWater->SetAlign(MGUI::eAlign::STRETCH);
	mLbWater->E_SelectChanged += new cListener2<WaterModule, const MGUI::Event *, int>(this, &WaterModule::OnWaterSelChanged);

	mBnAdd = (MGUI::Button *)mLayout->GetChild("Add");
	mBnRefresh = (MGUI::Button *)mLayout->GetChild("Refresh");
	mBnEdit = (MGUI::Button *)mLayout->GetChild("Edit");

	mBnAdd->E_Click += new cListener1<WaterModule, const MGUI::ClickEvent *>(this, &WaterModule::OnAdd);
	mBnRefresh->E_Click += new cListener1<WaterModule, const MGUI::ClickEvent *>(this, &WaterModule::OnRefresh);
	mBnEdit->E_Click += new cListener1<WaterModule, const MGUI::ClickEvent *>(this, &WaterModule::OnEdit);

	mRenderOp_Brush = NULL;
	mRenderOp_Water = NULL;

	InitBrush();

	mWaterDialog = new WaterDialog;

	mWaterProperty = new MGUI::Layout(AllLookFeel::Instance()->GetPanel(), NULL);
	mWaterProperty->SetRect(0, 0, 256, 256);
	mWaterProperty->SetVisible(false);

	mPropertyGridWater = new PropertyGrid(mWaterProperty);
	mPropertyGridWater->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGridWater->SetDockable(true);

	mPropertyGridShader = new PropertyGrid(mWaterProperty);
	mPropertyGridShader->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGridShader->SetDockable(true);

	Editor::Instance()->E_Update += new cListener0<WaterModule>(this, &WaterModule::OnUpdate);
	Editor::Instance()->E_SceneLoaded += new cListener0<WaterModule>(this, &WaterModule::OnSceneLoaded);
	Editor::Instance()->E_SceneUnload += new cListener0<WaterModule>(this, &WaterModule::OnSceneUnload);
	Editor::Instance()->E_NodeDestroy += new cListener1<WaterModule, Node *>(this, &WaterModule::OnNodeDestroy);
	World::Instance()->E_SectionLoaded += new cListener1<WaterModule, WorldSection *>(this, &WaterModule::OnSectionLoaded);
	World::Instance()->E_SectionUnload += new cListener1<WaterModule, WorldSection *>(this, &WaterModule::OnSectionUnload);

	World::Instance()->E_RenderAlphaEnd += new cListener0<WaterModule>(this, &WaterModule::OnRender);
}

WaterModule::~WaterModule()
{
	safe_delete(mRenderOp_Brush);
	safe_delete(mRenderOp_Water);

	delete mWaterProperty;
	delete mWaterDialog;
	delete mLayout;
}

void WaterModule::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;
	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);

	mWaterProperty->SetRect(w, D_MAINMENU_H, 256, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void WaterModule::Show()
{
	Layout();

	mLayout->SetVisible(true);
	mWaterProperty->SetVisible(true);

	Editor::Instance()->SetSelectNode(NULL);
	Editor::Instance()->ClearFlags(FLAG_ALL);
	Gizmo::Instance()->SetOperator(Gizmo::OP_MOVE);
}

void WaterModule::Hide()
{
	mLayout->SetVisible(false);
	mWaterProperty->SetVisible(false);
}

void WaterModule::InitBrush()
{
	mRenderOp_Brush = new RenderOp;

	int iVertexCount = 4;

	VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 3, iVertexCount);
	Float3 * vert = (Float3 *)vb->Lock(eLockFlag::WRITE);
	{
		*vert++ = Float3(-0.5f, 0, -0.5f);
		*vert++ = Float3(+0.5f, 0, -0.5f);
		*vert++ = Float3(-0.5f, 0, +0.5f);
		*vert++ = Float3(+0.5f, 0, +0.5f);
	}
	vb->Unlock();

	mRenderOp_Brush->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
	mRenderOp_Brush->vertexBuffers[0] = vb;
	mRenderOp_Brush->primCount = 2;
	mRenderOp_Brush->primType = ePrimType::TRIANGLE_STRIP;

	mShaderFX_WaterGrid = ShaderFXManager::Instance()->Load("WaterGrid", "Shader/WaterGrid.mfx");
	mShaderFX_WaterBrush = ShaderFXManager::Instance()->Load("WaterBrush", "Shader/WaterBrush.mfx");

	d_assert(mShaderFX_WaterBrush->GetPassCount() && mShaderFX_WaterGrid->GetPassCount());
}

void WaterModule::OnWaterSelChanged(const MGUI::Event * e, int index)
{
	safe_delete (mRenderOp_Water);

	mPropertyGridWater->Attach(NULL);
	mPropertyGridShader->Attach(NULL);

	if (index < 0)
		return ;

	Water * pWater = (Water *)mLbWater->GetUserData(index);
	int xGridCount = pWater->GetGridWidth();
	int zGridCount = pWater->GetGridHeight();

	mRenderOp_Water = new RenderOp;

	mRenderOp_Water->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

	int iVertexCount = (xGridCount + 1) * 2 + (zGridCount + 1) * 2;

	mRenderOp_Water->vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 3, iVertexCount);

	Float3 * vert = (Float3 *)mRenderOp_Water->vertexBuffers[0]->Lock(eLockFlag::WRITE);
	{
		float z_step = 1.0f / zGridCount;

		for (int i = 0; i < zGridCount + 1; ++i)
		{
			*vert++ = Float3(0, 0, i * z_step); *vert++ = Float3(1, 0, i * z_step);
		}

		float x_step = 1.0f / xGridCount;

		for (int i = 0; i < xGridCount + 1; ++i)
		{
			*vert++ = Float3(i * x_step, 0, 0); *vert++ = Float3(i * x_step, 0, 1);
		}
	}
	mRenderOp_Water->vertexBuffers[0]->Unlock();

	mRenderOp_Water->primCount = iVertexCount / 2;
	mRenderOp_Water->primType = ePrimType::LINE_LIST;

	Editor::Instance()->SetSelectNode(pWater);

	mPropertyGridWater->Attach(pWater);
	mPropertyGridShader->Attach(pWater->GetWaterShader().c_ptr());
}

void WaterModule::OnUpdate()
{
	if (!mLayout->IsVisible())
		return ;

	if (!MainRenderView::Instance()->IsMouseFoucsed())
		return ;

	if (mLbWater->GetSelectIndex() < 0)
		return ;

	Water * pWater = (Water *)mLbWater->GetUserData(mLbWater->GetSelectIndex());
	if (pWater->GetWaterShader().c_ptr() != mPropertyGridShader->GetAttachObj())
	{
		mPropertyGridShader->Attach(pWater->GetWaterShader().c_ptr());
	}

	Float2 pt = IMouse::Instance()->GetPosition();
	Editor::Instance()->MapScreenUnit(pt.x, pt.y);
	if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
		return ;

	Plane plane = Plane(Float3(0, 1, 0), -pWater->GetPosition().y);
	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
	float gridSize = pWater->GetGridSize();

	float dist = 0;

	if (!ray.Intersect(&dist, plane))
		return ;

	Float3 position = ray.orig + ray.dir * dist;

	int x, z;
	if (!pWater->MapCoord(x, z, position.x, position.z))
		return ;

	mBrushPosition.x = x * gridSize + gridSize / 2 + pWater->GetPosition().x;
	mBrushPosition.z = z * gridSize + gridSize / 2 + pWater->GetPosition().z;
	mBrushPosition.y = position.y;

	if (!IMouse::Instance()->KeyPressed(InputCode::MKC_LEFT))
		return ;
		
	unsigned char flag = IKeyboard::Instance()->KeyPressed(InputCode::KC_LSHIFT) ? 0 : 1;
	if (pWater->GetGridInfo(x, z) != flag)
	{
		int gridWidth = pWater->GetGridWidth();
		int gridHeight = pWater->GetGridHeight();
		unsigned char * gridInfo = new unsigned char[gridWidth * gridHeight];

		memcpy(gridInfo, pWater->GetGridInfo(), gridWidth * gridHeight);
		gridInfo[z * gridWidth + x] = flag;

		pWater->Build(gridInfo, gridSize, gridWidth, gridHeight);

		delete[] gridInfo;
	}
}

void WaterModule::OnRender()
{
	if (!mLayout->IsVisible() || mLbWater->GetSelectIndex() < 0)
		return ;

	if (mRenderOp_Water)
	{
		Water * pWater = (Water *)mLbWater->GetUserData(mLbWater->GetSelectIndex());
		float xSize = pWater->GetXSize();
		float zSize = pWater->GetZSize();

		Float3 Position = pWater->GetPosition();
		Float3 Scale = Float3(xSize, 1, zSize);

		Mat4 worldTM;
		worldTM.MakeTransform(Position, Quat::Identity, Scale);
		RenderSystem::Instance()->SetWorldTM(worldTM);

		RenderSystem::Instance()->Render(mShaderFX_WaterGrid, mRenderOp_Water);
	}

	if (mRenderOp_Brush)
	{
		Water * pWater = (Water *)mLbWater->GetUserData(mLbWater->GetSelectIndex());
		float gridSize = pWater->GetGridSize();

		Float3 Position = mBrushPosition;
		Float3 Scale = Float3(gridSize, 1, gridSize);

		Position.y += 0.01f;

		Mat4 worldTM;
		worldTM.MakeTransform(Position, Quat::Identity, Scale);
		RenderSystem::Instance()->SetWorldTM(worldTM);

		RenderSystem::Instance()->Render(mShaderFX_WaterBrush, mRenderOp_Brush);
	}
}

void WaterModule::OnAdd(const MGUI::ClickEvent * e)
{
	if (!Editor::Instance()->IsLoaded())
		return ;

	mWaterDialog->E_EndDialog += new cListener1<WaterModule, bool>(this, &WaterModule::OnAdd_OK);
	mWaterDialog->DoModal(NULL);
}

void WaterModule::OnAdd_OK(bool _ok)
{
	if (!_ok)
		return ;

	float gridSize = mWaterDialog->GetGridSize();
	int gridWidth = mWaterDialog->GetXGridCount();
	int gridHeight = mWaterDialog->GetZGridCount();

	Water * pWater = new Water;

	unsigned char * gridInfo = new unsigned char[gridWidth * gridHeight];
	memset(gridInfo, 0, gridWidth * gridHeight);
	pWater->Build(gridInfo, gridSize, gridWidth, gridHeight);
	delete[] gridInfo;

	pWater->SetPosition(Editor::Instance()->GetHitPosition(0.5f, 0.5f));

	mLbWater->Append(WSTR_("Water"), pWater);
	mLbWater->SetSelectIndex(mLbWater->GetCount() - 1);

	Editor::Instance()->AddNode(pWater);
}

void WaterModule::OnRefresh(const MGUI::ClickEvent * e)
{
	if (!Editor::Instance()->IsLoaded())
		return ;

	OnSceneLoaded();
}

void WaterModule::OnEdit(const MGUI::ClickEvent * e)
{
	if (!Editor::Instance()->IsLoaded())
		return ;

	if (mLbWater->GetSelectIndex() >= 0)
	{
		Water * pWater = (Water *)mLbWater->GetUserData(mLbWater->GetSelectIndex());

		mWaterDialog->E_EndDialog += new cListener1<WaterModule, bool>(this, &WaterModule::OnEdit_OK);
		mWaterDialog->DoModal(pWater);
	}
}

void WaterModule::OnEdit_OK(bool _ok)
{
	if (_ok && mLbWater->GetSelectIndex() >= 0)
	{
		float gridSize = mWaterDialog->GetGridSize();
		int gridWidth = mWaterDialog->GetXGridCount();
		int gridHeight = mWaterDialog->GetZGridCount();

		Water * pWater = (Water *)mLbWater->GetUserData(mLbWater->GetSelectIndex());

		if (pWater->GetGridSize() != gridSize ||
			pWater->GetGridWidth() != gridWidth ||
			pWater->GetGridHeight() != gridHeight)
		{
			unsigned char * gridInfo = new unsigned char[gridWidth * gridHeight];
			memset(gridInfo, 0, gridWidth * gridHeight);

			pWater->Build(gridInfo, gridSize, gridWidth, gridHeight);

			delete[] gridInfo;
		}
	}
}

void WaterModule::OnSceneLoaded()
{
}

void WaterModule::OnSceneUnload()
{
	mLbWater->Clear();
}

void WaterModule::OnNodeDestroy(Node * n)
{
	for (int i = 0; i < mLbWater->GetCount(); ++i)
	{
		Node * pWater = (Node *)mLbWater->GetUserData(i);
		if (pWater == n)
		{
			mLbWater->Remove(i);
			break;
		}
	}

	mPropertyGridWater->Attach(NULL);
	mPropertyGridShader->Attach(NULL);
}

void WaterModule::OnSectionLoaded(WorldSection * section)
{
	const Array<Node *> & nodeArray = section->GetNodes();
	for (int i = 0; i < nodeArray.Size(); ++i)
	{
		if (KIND_OF(Water, nodeArray[i]))
		{
			mLbWater->Append(nodeArray[i]->GetName().c_wstr(), nodeArray[i]);
		}
	}
}

void WaterModule::OnSectionUnload(WorldSection * section)
{
	for (int i = 0; i < mLbWater->GetCount(); ++i)
	{
		Water * water = (Water *)mLbWater->GetUserData(i);
		if (water->GetSection() == section)
		{
			mLbWater->Remove(i--);
		}
	}
}
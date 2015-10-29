#include "TerrainModule.h"
#include "MainFrame.h"
#include "Editor.h"

ImplementSingleton(TerrainModule);

TerrainModule::TerrainModule()
	: EditorModule("Terrain")
{
	mLayout = MGUI::Layout::Load("TerrainPanel.layout", NULL);
	mLayout->SetVisible(false);

	MGUI::Button * bnHeight = (MGUI::Button *)mLayout->GetChild("Height");
	MGUI::Button * bnLayer = (MGUI::Button *)mLayout->GetChild("Layer");
	MGUI::Button * bnPicker = (MGUI::Button *)mLayout->GetChild("Picker");

	bnHeight->E_MouseClick += new cListener1<TerrainModule, const MGUI::MouseEvent *>(this, &TerrainModule::OnHeight);
	bnLayer->E_MouseClick += new cListener1<TerrainModule, const MGUI::MouseEvent *>(this, &TerrainModule::OnLayer);
	bnPicker->E_MouseClick += new cListener1<TerrainModule, const MGUI::MouseEvent *>(this, &TerrainModule::OnPicker);

	//
	mTerrainHeightPanel = new TerrainHeightPanel;
	mTerrainLayerPanel = new TerrainLayerPanel;
	mTerrainPickerPanel = new TerrainPickerPanel;

	//
	Editor::Instance()->E_SceneUnload += new cListener0<TerrainModule>(this, &TerrainModule::OnSceneUnload);
}

TerrainModule::~TerrainModule()
{
	delete mTerrainPickerPanel;
	delete mTerrainLayerPanel;
	delete mTerrainHeightPanel;
	delete mLayout;
}

void TerrainModule::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);

	mTerrainHeightPanel->Layout();
	mTerrainLayerPanel->Layout();
	mTerrainPickerPanel->Layout();
}

void TerrainModule::Show()
{
	Layout();

	mLayout->SetVisible(true);

	Editor::Instance()->SetSelectNode(NULL);
	Editor::Instance()->ClearFlags(FLAG_ALL);
}

void TerrainModule::Hide()
{
	mTerrainHeightPanel->Hide();
	mTerrainLayerPanel->Hide();
	mTerrainPickerPanel->Hide();

	mLayout->SetVisible(false);
}

void TerrainModule::_showWithAnim()
{
	Layout();
	
	mLayout->SetVisible(true);
	mLayout->RemoveAllComponent();

	float w = mLayout->GetRect().w;

	MGUI::KFAnimMove * pMoveKey = new MGUI::KFAnimMove;
	pMoveKey->Append(0, -w, 0);
	pMoveKey->Append(INDENT_TIME, 0, 0);

	mLayout->AddComponent(pMoveKey);

	pMoveKey->Update(0);
}

void TerrainModule::OnHeight(const MGUI::MouseEvent *)
{
	Layout();

	mLayout->RemoveAllComponent();

	float w = mLayout->GetRect().w;

	MGUI::KFAnimMove * pMoveKey = new MGUI::KFAnimMove;
	pMoveKey->Append(0, 0, 0);
	pMoveKey->Append(INDENT_TIME, -w, 0);
	pMoveKey->SetUserData((void *)0);
	pMoveKey->E_KeyEnd += new cListener1<TerrainModule, MGUI::Component *>(this, &TerrainModule::OnKeyEnd);

	mLayout->AddComponent(pMoveKey);
	mLayout->SetPickFlag(MGUI::ePickFlag::NONE);
}

void TerrainModule::OnLayer(const MGUI::MouseEvent *)
{
	Layout();

	mLayout->RemoveAllComponent();

	float w = mLayout->GetRect().w;

	MGUI::KFAnimMove * pMoveKey = new MGUI::KFAnimMove;
	pMoveKey->Append(0, 0, 0);
	pMoveKey->Append(INDENT_TIME, -w, 0);
	pMoveKey->SetUserData((void *)1);
	pMoveKey->E_KeyEnd += new cListener1<TerrainModule, MGUI::Component *>(this, &TerrainModule::OnKeyEnd);

	mLayout->AddComponent(pMoveKey);
	mLayout->SetPickFlag(MGUI::ePickFlag::NONE);
}

void TerrainModule::OnPicker(const MGUI::MouseEvent *)
{
	Layout();

	mLayout->RemoveAllComponent();

	float w = mLayout->GetRect().w;

	MGUI::KFAnimMove * pMoveKey = new MGUI::KFAnimMove;
	pMoveKey->Append(0, 0, 0);
	pMoveKey->Append(INDENT_TIME, -w, 0);
	pMoveKey->SetUserData((void *)2);
	pMoveKey->E_KeyEnd += new cListener1<TerrainModule, MGUI::Component *>(this, &TerrainModule::OnKeyEnd);

	mLayout->AddComponent(pMoveKey);
	mLayout->SetPickFlag(MGUI::ePickFlag::NONE);
}

void TerrainModule::OnKeyEnd(MGUI::Component * cp)
{
	mLayout->SetVisible(false);
	mLayout->SetPickFlag(MGUI::ePickFlag::ALL);

	if (cp->GetUserData() == (void *)0)
	{
		mTerrainHeightPanel->Show();
	}
	else if (cp->GetUserData() == (void *)1)
	{
		mTerrainLayerPanel->Show();
	}
	else if (cp->GetUserData() == (void *)2)
	{
		mTerrainPickerPanel->Show();
	}
}

void TerrainModule::OnSceneUnload()
{
}

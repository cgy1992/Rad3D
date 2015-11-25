#include "MainMenu.h"
#include "MainFrame.h"
#include "Editor.h"
#include "AllLookFeel.h"
#include "MUtil.h"

MainMenu::MainMenu()
{
	mLayout = MGUI::Layout::Load("MainMenu.layout", NULL);
	
	MGUI::Button * Bn_Open = (MGUI::Button *)mLayout->GetChild("Open");
	Bn_Open->E_MouseClick += new cListener1<MainMenu, const MGUI::MouseEvent *>(this, &MainMenu::OnOpen);
	
	MGUI::Button * Bn_Save = (MGUI::Button *)mLayout->GetChild("Save");
	Bn_Save->E_MouseClick += new cListener1<MainMenu, const MGUI::MouseEvent *>(this, &MainMenu::OnSave);

	MGUI::Button * Bn_SavePrefab = mLayout->GetChildT<MGUI::Button>("SavePrefab");
	Bn_SavePrefab->E_MouseClick += new cListener1<MainMenu, const MGUI::MouseEvent *>(this, &MainMenu::OnSavePrefab);

	//
	MGUI::Widget * editorBack = (MGUI::Widget *)mLayout->GetChild("EditorBack");
	
	mComboBox_EditorMode = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), editorBack);
	mComboBox_EditorMode->SetAlign(MGUI::eAlign::STRETCH);
	mComboBox_EditorMode->Append(WSTR_("None"));
	mComboBox_EditorMode->SetSelectIndex(0);

	mComboBox_EditorMode->E_SelectChanged += new cListener2<MainMenu, const MGUI::Event *, int>(this, &MainMenu::OnEditorModeChanged);

	//
	MGUI::Widget * cameraBack = (MGUI::Widget *)mLayout->GetChild("CameraBack");
	mComboBox_CameraMode = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), cameraBack);
	mComboBox_CameraMode->SetAlign(MGUI::eAlign::STRETCH);
	mComboBox_CameraMode->Append(WSTR_("Normal"));
	mComboBox_CameraMode->Append(WSTR_("No_Y"));
	mComboBox_CameraMode->SetSelectIndex(0);

	mComboBox_CameraMode->E_SelectChanged += new cListener2<MainMenu, const MGUI::Event *, int>(this, &MainMenu::OnCameraModeChanged);

	mSceneFile = Editor::Instance()->GetConfig()->GetString("SceneFileCache", "");}

MainMenu::~MainMenu()
{
	delete mLayout;
}

void MainMenu::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	mLayout->SetRect(0, 0, rect.w, D_MAINMENU_H);
}

void MainMenu::OnOpen(const MGUI::MouseEvent *)
{
	String filename = Util_GetOpenFile("project(*.scene)\0*.scene\0", FileHelper::GetFileDir(mSceneFile));
	if (filename != "")
	{
		mSceneFile = filename;
		mProjectFile = FileHelper::GetFileDir(FileHelper::GetFileDir(mSceneFile));
		mProjectFile = FileHelper::GetFileDir(mProjectFile) + "/m.project";

		Editor::Instance()->LoadProject(mProjectFile.c_str());

		Editor::Instance()->LoadScene(mSceneFile.c_str());

		Editor::Instance()->GetConfig()->SetVariable("SceneFileCache", mSceneFile.c_str());
	}
}

void MainMenu::OnSave(const MGUI::MouseEvent *)
{
	if (mSceneFile == "")
		return ;

	Editor::Instance()->SaveScene();
}

void MainMenu::OnSavePrefab(const MGUI::MouseEvent *)
{
	if (mSceneFile == "")
		return ;

	Editor::Instance()->SaveScene();
	Editor::Instance()->SavePrefab();
}

void MainMenu::OnEditorModeChanged(const MGUI::Event * e, int index)
{
	String modeName;
	modeName.FromUnicode(mComboBox_EditorMode->GetText(index).c_str());

	MainFrame::Instance()->ChangeModule(modeName);
}

void MainMenu::OnCameraModeChanged(const MGUI::Event * e, int index)
{
	Editor::Instance()->SetCameraMode(index);
}

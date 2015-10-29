#include "MainFrame.h"
#include "Editor.h"
#include "EntityModule.h"
#include "TerrainModule.h"
#include "WaterModule.h"
#include "GrassModule.h"
#include "EnvironmentModule.h"
#include "ParticleFXModule.h"
#include "LightFXModule.h"
#include "NavPathModule.h"
#include "ComponentModule.h"
#include "MaterialModule.h"

ImplementSingleton(MainFrame);

MainFrame::MainFrame()
{
	mAllLookFeel = new AllLookFeel;

	mMainMenu = new MainMenu;
	mMainStatusBar = new MainStatusBar;
	mMainRenderView = new MainRenderView;

	mColorPickerDlg = new ColorPickerDialog;
	mMessageBoxDlg = new MessageBoxDialog;
	mProgressDlg = new ProgressDialog;

	mConsole = new Console;

	mCurrentModule = NULL;

	AddModule(new EntityModule);
	AddModule(new TerrainModule);
	AddModule(new EnvironmentModule);
	AddModule(new WaterModule);
	AddModule(new GrassModule);
	AddModule(new ParticleFXModule);
	AddModule(new LightFXModule);
	AddModule(new NavPathModule);
	AddModule(new ComponentModule);
	AddModule(new MaterialModule);

	Layout();
}

MainFrame::~MainFrame()
{
	Editor::Instance()->UnloadScene();

	MGUI::InputManager::Instance()->SetMouseFocusedWidget(NULL);
	MGUI::InputManager::Instance()->SetKeyFocusedWidget(NULL);

	for (int i = 0; i < mModules.Size(); ++i)
		delete mModules[i];
	mModules.Clear();

	delete mProgressDlg;
	delete mMessageBoxDlg;
	delete mColorPickerDlg;
	delete mConsole;

	delete mMainRenderView;
	delete mMainStatusBar;
	delete mMainMenu;

	delete mAllLookFeel;
}

void MainFrame::Layout()
{
	mMainMenu->Layout();
	mMainStatusBar->Layout();
	mMainRenderView->Layout();

	for (int i = 0; i < mModules.Size(); ++i)
	{
		mModules[i]->Layout();
	}
}

void MainFrame::AddModule(EditorModule * editor)
{
	d_assert (editor != NULL);

	mModules.PushBack(editor);

	mMainMenu->GetEditorModeComoBox()->Append(editor->GetName().c_wstr());
}

EditorModule * MainFrame::GetModule(const String & modeName)
{
	for (int i = 0; i < mModules.Size(); ++i)
	{
		if (mModules[i]->GetName() == modeName)
			return mModules[i];
	}

	return NULL;
}

void MainFrame::ChangeModule(const String & modeName)
{
	if (mCurrentModule != NULL)
		mCurrentModule->Hide();

	mCurrentModule = GetModule(modeName);

	if (mCurrentModule)
		mCurrentModule->Show();
}
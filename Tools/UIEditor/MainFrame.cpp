#include "MainFrame.h"

ImplementSingleton(MainFrame);

MainFrame::MainFrame()
{
	mAllLookFeel = new AllLookFeel;
	mAllLookFeel->InitEditorHelper();

	mMainMenu = new MainMenu;
	mMainStatusBar = new MainStatusBar;

	mLookFeelEditor= new LookFeelEditor;
	mLayoutEditor = new LayoutEditor;

	SetEditorMode(EM_Lookfeel);

	Layout();
}

MainFrame::~MainFrame()
{
	E_EditorModeChanged.DetachAll();

	delete mLayoutEditor;
	delete mLookFeelEditor;

	delete mMainMenu;
	delete mMainStatusBar;

	delete mAllLookFeel;
}

void MainFrame::Layout()
{
	mMainMenu->Layout();
	mMainStatusBar->Layout();

	mLookFeelEditor->Layout();
	mLayoutEditor->Layout();
}



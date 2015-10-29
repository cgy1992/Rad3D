/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "AllLookFeel.h"
#include "MainMenu.h"
#include "MainStatusBar.h"
#include "MainRenderView.h"

#include "ColorPickerDialog.h"
#include "MessageBoxDialog.h"
#include "ProgressDialog.h"
#include "EditorModule.h"

#include "Console.h"

#define D_MAINMENU_H 32.0f
#define D_MAINSTATUSBAR_H 32.0f
#define D_MAINTOOLBAR_W 48.0f

class MSTUDIO_ENTRY MainFrame : public Singleton<MainFrame>
{
public:
	MainFrame();
	~MainFrame();

	void 
		Layout();

	void
		ChangeModule(const String & modeName);
	void 
		AddModule(EditorModule * editor);
	EditorModule * 
		GetModule(const String & modeName);
	EditorModule *
		GetCurrentModule() { return mCurrentModule; }

protected:
	AllLookFeel * mAllLookFeel;

	MainMenu * mMainMenu;
	MainStatusBar * mMainStatusBar;
	MainRenderView * mMainRenderView;

	//
	ColorPickerDialog * mColorPickerDlg;
	MessageBoxDialog * mMessageBoxDlg;
	ProgressDialog * mProgressDlg;

	Console * mConsole;

	Array<EditorModule *> mModules;
	EditorModule * mCurrentModule;
};


/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "AllLookFeel.h"
#include "MainMenu.h"
#include "MainStatusBar.h"
#include "LookFeelEditor.h"
#include "LayoutEditor.h"

enum eEditorMode {
	EM_None,
	EM_Lookfeel,
	EM_Layout,
};

class MainFrame : public Singleton<MainFrame>
{
public:
	tEvent1<int> E_EditorModeChanged;

public:
	MainFrame();
	~MainFrame();

	void Layout();

	void SetEditorMode(eEditorMode mode)
	{
		if (mEditorMode != mode)
		{
			mEditorMode = mode;
			E_EditorModeChanged(mode);
		}
	}

	eEditorMode GetEditorMode()
	{
		return mEditorMode;
	}

protected:
	eEditorMode mEditorMode;

	AllLookFeel * mAllLookFeel;
	MainMenu * mMainMenu;
	MainStatusBar * mMainStatusBar;
	LookFeelEditor * mLookFeelEditor;
	LayoutEditor * mLayoutEditor;
};


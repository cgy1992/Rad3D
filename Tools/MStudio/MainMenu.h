/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void 
		Layout();

	MGUI::ComboBox * 
		GetEditorModeComoBox() { return mComboBox_EditorMode; }

protected:
	void 
		OnOpen(const MGUI::MouseEvent *);
	void 
		OnSave(const MGUI::MouseEvent *);
	void 
		OnSavePrefab(const MGUI::MouseEvent *);

	void 
		OnEditorModeChanged(const MGUI::Event * e, int index);

	void
		OnCameraModeChanged(const MGUI::Event * e, int index);

protected:
	MGUI::Layout * mLayout;

	MGUI::ComboBox * mComboBox_EditorMode;
	MGUI::ComboBox * mComboBox_CameraMode;

	String mProjectFile;
	String mSceneFile;
};


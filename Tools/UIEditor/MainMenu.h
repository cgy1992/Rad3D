/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void Layout();

protected:
	void OnOpen_(const MGUI::MouseEvent * e);
	void OnSave_(const MGUI::MouseEvent * e);
	void OnEditModeChanged_(const MGUI::Event * e, int index);

protected:
	MGUI::Layout * mLayout;
	MGUI::Button * mBn_Open;
	MGUI::Button * mBn_Save;

	MGUI::Layout * mLayout_EditMode;
	MGUI::ComboBox * mCb_EditMode;
};


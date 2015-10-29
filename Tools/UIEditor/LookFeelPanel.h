/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class LookFeelPanel
{
public:
	LookFeelPanel();
	~LookFeelPanel();

	void Layout();
	MGUI::Layout * GetLayout() { return mLayout; }

	int GetEditWidgetType();

protected:
	void OnWidgetTypeChanged_(const MGUI::Event * e, int index);
	void OnLookFeelChanged_(const MGUI::Event * e, int index);
	void OnNameChanged_();
	void OnOpenLookFeel_();

	void OnCreate_(const MGUI::MouseEvent * e);
	void OnDelete_(const MGUI::MouseEvent * e);

protected:
	MGUI::Layout * mLayout;
	MGUI::ComboBox * mComboBox_WidgetType;
	MGUI::ListBox * mListBox_LookFeel;
};


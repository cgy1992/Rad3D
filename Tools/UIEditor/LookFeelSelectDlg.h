/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class LookFeelSelectDlg
{
public:
	tEvent1<bool> E_EndDialog;

public:
	LookFeelSelectDlg();
	~LookFeelSelectDlg();

	void DoModal();

	int GetSelectType();
	MGUI::LookFeel * GetSelectLookFeel();
	String GetNewName();

protected:
	MGUI::Layout * mLayout;
	MGUI::Widget * mDialog;

	MGUI::Label * mLabel_Caption;

	MGUI::ComboBox * mComboBox_WidgetType;
	MGUI::ListBox * mListBox_LookFeel;

	MGUI::Widget * mWidget_Priview;
	MGUI::Button * mButton_OK;
	MGUI::Button * mButton_Cancel;

	MGUI::Label * mLabel_NewName;
	MGUI::EditBox * mEditBox_NewName;

	MGUI::Widget * mWidget;

protected:
	void OnWidgetTypeChanged_(const MGUI::Event * e, int index);
	void OnLookFeelChanged_(const MGUI::Event * e, int index);
	void OnOK_(const MGUI::MouseEvent * e);
	void OnCancel_(const MGUI::MouseEvent * e);
};


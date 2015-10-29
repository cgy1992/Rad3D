/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"
#include "LookFeelSelectDlg.h"

class LookFeelProperty
{
public:
	LookFeelProperty();
	~LookFeelProperty();

	void 
		Layout();
	MGUI::Layout * 
		GetLayout() { return mLayout; }

protected:
	void 
		OnEditLookFeelChanged_(MGUI::LookFeel * _lookfeel);
	void 
		OnEditRectChanged_(MGUI::Rect _rect);

	void 
		OnSkinChanged_(const MGUI::FocusEvent * e);
	void 
		OnStateChanged_(const MGUI::Event * e, int index);

	void 
		OnNameReturn_(const MGUI::FocusEvent * e);
	void 
		OnClientReturn_(const MGUI::FocusEvent * e);
	void 
		OnUVRectReturn_(const MGUI::FocusEvent * e);
	void 
		OnTextColorChanged(const MGUI::MouseEvent * e);

	void 
		OnAddChild(const MGUI::ClickEvent *);
	void 
		OnRemoveChild(const MGUI::ClickEvent *);
	void 
		OnLookFeelSelect_EndDialog(bool _ok);

protected:
	MGUI::Layout * mLayout;

	MGUI::EditBox * mEditBox_Name;
	MGUI::EditBox * mEditBox_Skin;
	MGUI::EditBox * mEditBox_Client;
	MGUI::ComboBox * mComboBox_State;
	MGUI::EditBox * mEditBox_UVRect;
	MGUI::Widget * mWidget_TextColor;
	MGUI::ListBox * mListBox_Child;
	MGUI::Button * mButton_AddChild;
	MGUI::Button * mButton_RemoveChild;

	LookFeelSelectDlg * mLookFeelSelectDlg;
};


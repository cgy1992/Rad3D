/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TypeEd.h"

class TypeEd_ListBox : public TypeEd
{
public:
	TypeEd_ListBox(MGUI::Widget * editbox, MGUI::Widget * panel);
	virtual ~TypeEd_ListBox();

	void OnItemHeightChanged(const MGUI::FocusEvent * e);
	void OnFontChanged(const MGUI::Event * e, int index);

protected:
	MGUI::ListBox * mListBox;

	MGUI::Label * mLabel_ItemHeight;
	MGUI::EditBox * mEditBox_ItemHeight;

	MGUI::Label * mLabel_Font;
	MGUI::ComboBox * mCombo_Font;
};
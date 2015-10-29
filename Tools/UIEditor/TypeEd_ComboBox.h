/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TypeEd.h"

class TypeEd_ComboBox : public TypeEd
{
public:
	TypeEd_ComboBox(MGUI::Widget * editbox, MGUI::Widget * panel);
	virtual ~TypeEd_ComboBox();

	void OnItemHeightChanged(const MGUI::FocusEvent * e);
	void OnFontChanged(const MGUI::Event * e, int index);

protected:
	MGUI::ComboBox * mComboBox;

	MGUI::Label * mLabel_ItemHeight;
	MGUI::EditBox * mEditBox_ItemHeight;

	MGUI::Label * mLabel_Font;
	MGUI::ComboBox * mCombo_Font;
};
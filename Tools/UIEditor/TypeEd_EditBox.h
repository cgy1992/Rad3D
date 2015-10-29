/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TypeEd.h"

class TypeEd_EditBox : public TypeEd
{
public:
	TypeEd_EditBox(MGUI::Widget * editbox, MGUI::Widget * panel);
	virtual ~TypeEd_EditBox();

	void OnCaptionChanged(const MGUI::FocusEvent * e);
	void OnFontChanged(const MGUI::Event * e, int index);
	void OnTextAlignChanged(const MGUI::Event * e, int index);
	void OnTextColorChanged(const MGUI::MouseEvent * e);

protected:
	MGUI::EditBox * mEditBox;

	MGUI::Label * mLabel_Caption;
	MGUI::EditBox * mEditBox_Caption;

	MGUI::Label * mLabel_Font;
	MGUI::ComboBox * mCombo_Font;

	MGUI::Label * mLabel_TextAlign;
	MGUI::ComboBox * mCombo_TextAlign;

	MGUI::Label * mLabel_TextColor;
	MGUI::Widget * mWidget_TextColor;
};
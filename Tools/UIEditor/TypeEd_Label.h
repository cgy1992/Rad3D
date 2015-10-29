/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TypeEd.h"

class TypeEd_Label : public TypeEd
{
public:
	TypeEd_Label(MGUI::Widget * label, MGUI::Widget * panel);
	virtual ~TypeEd_Label();

	void OnCaptionChanged(const MGUI::FocusEvent * e);
	void OnFontChanged(const MGUI::Event * e, int index);
	void OnTextAlignChanged(const MGUI::Event * e, int index);
	void OnTextColorChanged(const MGUI::MouseEvent * e);

protected:
	MGUI::Label * mLabel;

	MGUI::Label * mLabel_Caption;
	MGUI::EditBox * mEditBox_Caption;

	MGUI::Label * mLabel_Font;
	MGUI::ComboBox * mCombo_Font;

	MGUI::Label * mLabel_TextAlign;
	MGUI::ComboBox * mCombo_TextAlign;

	MGUI::Label * mLabel_TextColor;
	MGUI::Widget * mWidget_TextColor;
};
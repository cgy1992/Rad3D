/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TypeEd.h"

class TypeEd_Button : public TypeEd
{
public:
	TypeEd_Button(MGUI::Widget * button, MGUI::Widget * panel);
	virtual ~TypeEd_Button();

	void OnCaptionChanged(const MGUI::FocusEvent * e);
	void OnFontChanged(const MGUI::Event * e, int index);

protected:
	MGUI::Button * mButton;

	MGUI::Label * mLabel_Caption;
	MGUI::EditBox * mEditBox_Caption;

	MGUI::Label * mLabel_Font;
	MGUI::ComboBox * mCombo_Font;
};
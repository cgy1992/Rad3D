/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TypeEd.h"

class TypeEd_CheckBox : public TypeEd
{
public:
	TypeEd_CheckBox(MGUI::Widget * widget, MGUI::Widget * panel);
	virtual ~TypeEd_CheckBox();

	void OnCheckedChanged(const MGUI::Event * e, int index);

protected:
	MGUI::CheckBox * mCheckBox;

	MGUI::Label * mLabel_Checked;
	MGUI::ComboBox * mComboBox_Checked;
};
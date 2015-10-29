/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TypeEd.h"

class TypeEd_EditView : public TypeEd
{
public:
	TypeEd_EditView(MGUI::Widget * editbox, MGUI::Widget * panel);
	virtual ~TypeEd_EditView();

	void OnFontChanged(const MGUI::Event * e, int index);

protected:
	MGUI::EditView * mEditView;

	MGUI::Label * mLabel_Font;
	MGUI::ComboBox * mCombo_Font;
};
/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TypeEd.h"

class TypeEd_ImageBox : public TypeEd
{
public:
	TypeEd_ImageBox(MGUI::Widget * widget, MGUI::Widget * panel);
	virtual ~TypeEd_ImageBox();

	void OnImageChanged(const MGUI::FocusEvent * e);
	void OnUVRectChanged(const MGUI::FocusEvent * e);

protected:
	MGUI::ImageBox * mImageBox;

	MGUI::Label * mLabel_Image;
	MGUI::EditBox * mEditBox_Image;

	MGUI::Label * mLabel_UVRect;
	MGUI::EditBox * mEditBox_UVRect;
};
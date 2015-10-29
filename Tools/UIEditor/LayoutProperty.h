/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"
#include "TypeEd.h"

class LayoutProperty
{
public:
	LayoutProperty();
	~LayoutProperty();

	void Layout();
	MGUI::Layout * GetLayout() { return mLayout; }

protected:
	MGUI::Layout * mLayout;

	MGUI::EditBox * mEditBox_Name;
	MGUI::EditBox * mEditBox_Rect;
	MGUI::EditBox * mEditBox_Order;
	MGUI::Widget * mWidget_Color;
	MGUI::EditBox * mEditBox_Opacity;
	MGUI::ComboBox * mComboBox_InheritAlpha;

	MGUI::ComboBox * mComboBox_HAlign;
	MGUI::ComboBox * mComboBox_VAlign;

	MGUI::ComboBox * mComboBox_Enable;
	MGUI::ComboBox * mComboBox_PickFlag;

	MGUI::Panel * mTypeEdPanel;

	TypeEd * mTypeEd;

protected:
	void OnNameChanged_(const MGUI::FocusEvent * e);
	void OnRectChanged_(const MGUI::FocusEvent * e);
	void OnOrderChanged_(const MGUI::FocusEvent * e);
	void OnColorChanged_(const MGUI::MouseEvent * e);
	void OnOpacityChanged_(const MGUI::FocusEvent * e);
	void OnInheritAlphaChanged_(const MGUI::Event * e, int _index);
	void OnAlignChanged_(const MGUI::Event * e, int _index);
	void OnEnableChanged_(const MGUI::Event * e, int _index);
	void OnPickFlagChanged_(const MGUI::Event * e, int _index);

	void OnEditWidgetChanged_(MGUI::Widget * _widget);
	void OnEditRectChanged_(MGUI::Rect rect);
};


/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyFieldButton.h"

class PropertyFieldQuaternion : public PropertyFieldButton
{
public:
	PropertyFieldQuaternion(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldQuaternion();

protected:
	void 
		OnClick(const MGUI::ClickEvent * e);
	void 
		OnOK(const MGUI::ClickEvent * e);
	void 
		OnCancel(const MGUI::ClickEvent * e);

protected:
	MGUI::Layout * mLayout;

	MGUI::EditBox * mEditBoxX;
	MGUI::EditBox * mEditBoxY;
	MGUI::EditBox * mEditBoxZ;
};

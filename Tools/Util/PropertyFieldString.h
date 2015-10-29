/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyFieldEditBox.h"

class PropertyFieldFixedString : public PropertyFieldEditBox
{
public:
	PropertyFieldFixedString(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldFixedString();

protected:
	void OnTextChanged(const MGUI::FocusEvent *);
};


class PropertyFieldString : public PropertyFieldEditBox
{
public:
	PropertyFieldString(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldString();

protected:
	void OnTextChanged(const MGUI::FocusEvent *);
};

/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyGrid.h"

class PropertyFieldEditBox : public PropertyField
{
public:
	PropertyFieldEditBox(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldEditBox();

protected:
	MGUI::EditBox * mEditBox;
};
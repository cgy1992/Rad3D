/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyGrid.h"

class PropertyFieldButton : public PropertyField
{
public:
	PropertyFieldButton(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldButton();

protected:
	MGUI::Button * mButton;
};

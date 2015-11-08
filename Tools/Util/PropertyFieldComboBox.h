/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyGrid.h"

class PropertyFieldComboBox : public PropertyField
{
public:
	PropertyFieldComboBox(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldComboBox();

protected:
	MGUI::ComboBox * mComboBox;
};

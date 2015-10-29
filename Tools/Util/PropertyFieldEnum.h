/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyFieldComboBox.h"

class PropertyFieldEnum : public PropertyFieldComboBox
{
public:
	PropertyFieldEnum(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldEnum();

	void OnSelChanged(const MGUI::Event * e, int index);
};
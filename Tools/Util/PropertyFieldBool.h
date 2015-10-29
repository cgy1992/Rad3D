/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyFieldComboBox.h"

class PropertyFieldBool : public PropertyFieldComboBox
{
public:
	PropertyFieldBool(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldBool();

	void OnSelChanged(const MGUI::Event * e, int index);
};
/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyFieldComboBox.h"

class PropertyFieldClass : public PropertyFieldComboBox
{
public:
	PropertyFieldClass(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldClass();

protected:
	void OnSelChanged(const MGUI::Event * e, int index);

protected:
	Array<FixedString32> mClassNames;
};

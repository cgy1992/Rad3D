/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MInclude.h"

class PropertyGroup;

class PropertyField
{
public:
	PropertyField(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyField();

	virtual float _updateHeight(float y);

protected:
	PropertyGroup * mPropertyGroup;
	IObject * mObject;
	const Property * mProperty;

	MGUI::Widget * mWidget;
	MGUI::Label * mLabel;
	MGUI::Widget * mWidget_Value;
};

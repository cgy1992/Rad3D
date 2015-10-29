/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyField.h"

class PropertyGrid;

class PropertyGroup
{
	static const float MIN_HEIGHT;

public:
	PropertyGroup(const String & name, PropertyGrid * grid);
	virtual ~PropertyGroup();

	MGUI::Widget * GetClientWidget() { return mWidget_Client; }

	const String & GetName() { return mName; }

	void Add(PropertyField * field);

	float _updateHeight(float y);

protected:
	void OnExpand(const MGUI::Event * e, bool checked);

protected:
	String mName;
	PropertyGrid * mPropertyGrid;

	MGUI::Layout * mLayout;

	MGUI::Widget * mWidget_Caption;
	MGUI::TextBox * mTextBox_Caption;
	MGUI::CheckBox * mCheckBox_Expand;

	MGUI::Widget * mWidget_Client;

	Array<PropertyField *> mFields;

	float mMaxHeight;
};

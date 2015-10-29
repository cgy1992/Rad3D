/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyField.h"
#include "PropertyGroup.h"

class IPropertyGrid_UserCreator
{
public:
	IPropertyGrid_UserCreator() {}
	virtual ~IPropertyGrid_UserCreator() {}

	virtual PropertyField * CreatePropertyField(PropertyGroup * group, IObject * obj, const Property * prop) = 0;
};

class PropertyGrid : public MGUI::Widget
{
public:
	PropertyGrid(MGUI::Widget * parent, bool nogroup = false);
	~PropertyGrid();

	void 
		Clear();

	void 
		Attach(IObject * obj, IPropertyGrid_UserCreator * userCreator = NULL);

	IObject * 
		GetAttachObj() { return mObject; }

	bool
		NoGroup() { return mNoGroup; }

protected:
	PropertyField * 
		_createProperty(const Property * prop, IPropertyGrid_UserCreator * userCreator);
	PropertyGroup * 
		_getPropertyGroup(const char * gourpName);

protected:
	virtual void 
		OnUpdate(float elapsedTime);
	virtual void 
		UpdateRenderItem(MGUI::Layout * _layout);

protected:
	IObject * mObject;
	Array<PropertyGroup *> mGroups;
	bool mNoGroup;
};
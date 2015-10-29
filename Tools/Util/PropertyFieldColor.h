/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyGrid.h"

class PropertyFieldColor3 : public PropertyField
{
public:
	PropertyFieldColor3(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldColor3();

protected:
	void OnClick(const MGUI::MouseEvent *);

	void OnEndDialog_(bool _ok);
	void OnPreview_();

protected:
	MGUI::Widget * mWidget;

	Float4 mColor;
};



class PropertyFieldColor4 : public PropertyField
{
public:
	PropertyFieldColor4(PropertyGroup * group, IObject * obj, const Property * prop);
	virtual ~PropertyFieldColor4();

protected:
	void OnClick(const MGUI::MouseEvent *);

	void OnEndDialog_(bool _ok);
	void OnPreview_();

protected:
	MGUI::Widget * mWidget;

	Float4 mColor;
};
/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

struct eHAlign : public IEnum
{
	DECLARE_ENUM(eHAlign);

	enum enum_t {
		NONE,
		LEFT,
		RIGHT,
		CENTER,
		STRETCH,
	};
};

struct eVAlign : public IEnum
{
	DECLARE_ENUM(eVAlign);

	enum enum_t {
		NONE,
		TOP,
		BOTTOM,
		CENTER,
		STRETCH,
	};
};

struct ePickFlag : public IEnum
{
	DECLARE_ENUM(ePickFlag);

	enum enum_t {
		NONE,
		SELF,
		CHILD,
		ALL,
	};
};

struct WidgetPorperty : public IObject
{
	DECLARE_PROPERTY(IObject);

	MGUI::Widget * Widget;

	FixedString32 Name;
	MGUI::Rect Rect;
	int Order;
	Float3 Color;
	float Opacity;
	bool InheritAlpha;
	bool InheritColor;
	int HAlign;
	int VAlign;
	int PickFlag;
	bool Enable;

	WidgetPorperty();

	void 
		Attach(MGUI::Widget * w);
	void
		OnPropertyChanged(const Property * p);
};

#include "WidgetProperty.h"

DF_ENUM_BEGIN(eHAlign)
	DF_ENUM(NONE)
	DF_ENUM(LEFT)
	DF_ENUM(RIGHT)
	DF_ENUM(CENTER)
	DF_ENUM(STRETCH)
DF_ENUM_END()

DF_ENUM_BEGIN(eVAlign)
	DF_ENUM(NONE)
	DF_ENUM(TOP)
	DF_ENUM(BOTTOM)
	DF_ENUM(CENTER)
	DF_ENUM(STRETCH)
DF_ENUM_END()

DF_ENUM_BEGIN(ePickFlag)
	DF_ENUM(NONE)
	DF_ENUM(SELF)
	DF_ENUM(CHILD)
	DF_ENUM(ALL)
DF_ENUM_END()

DF_PROPERTY_BEGIN(WidgetPorperty)
DF_PROPERTY_END()

WidgetPorperty::WidgetPorperty()
{
}

void WidgetPorperty::Attach(MGUI::Widget * w)
{
	Widget = w;

	Name = w->GetName();
	Rect = w->GetRect();
	Order = w->GetOrder();
	Color = Float3(w->GetColor().r, w->GetColor().g, w->GetColor().b);
	Opacity = w->GetColor().a;
	InheritAlpha = w->IsInheritAlpha();
	InheritColor = w->IsInheritColor();

	int align = w->GetAlign()._value;
	if (align & MGUI::eAlign::LEFT)
		HAlign = eHAlign::LEFT;
	else if (align & MGUI::eAlign::H_CENTER)
		HAlign = eHAlign::CENTER;
	else if (align & MGUI::eAlign::RIGHT)
		HAlign = eHAlign::RIGHT;
	else if (align & MGUI::eAlign::H_STRETCH)
		HAlign = eHAlign::STRETCH;
	else
		HAlign = eHAlign::NONE;

	if (align & MGUI::eAlign::TOP)
		VAlign = eVAlign::TOP;
	else if (align & MGUI::eAlign::V_CENTER)
		VAlign = eVAlign::CENTER;
	else if (align & MGUI::eAlign::BOTTOM)
		VAlign = eVAlign::BOTTOM;
	else if (align & MGUI::eAlign::V_STRETCH)
		VAlign = eVAlign::STRETCH;
	else
		VAlign = eVAlign::NONE;

	PickFlag = w->GetPickFlag();
	Enable = w->IsEnable();
}

void WidgetPorperty::OnPropertyChanged(const Property * p)
{
	d_assert (Widget != NULL);

	Widget->SetName(Name);
	Widget->SetRect(Rect);
	Widget->SetOrder(Order);
	Widget->SetColor(Float4(Color.r, Color.g, Color.b, Opacity));
	Widget->SetInheritAlpha(InheritAlpha);
	Widget->SetInheritColor(InheritColor);

	int align = MGUI::eAlign::NONE;
	switch(HAlign)
	{
	case eHAlign::LEFT:
		align |= MGUI::eAlign::LEFT;
		break;

	case eHAlign::RIGHT:
		align |= MGUI::eAlign::RIGHT;
		break;

	case eHAlign::CENTER:
		align |= MGUI::eAlign::H_CENTER;
		break;

	case eHAlign::STRETCH:
		align |= MGUI::eAlign::H_STRETCH;
		break;
	}

	switch(VAlign)
	{
	case eVAlign::TOP:
		align |= MGUI::eAlign::TOP;
		break;

	case eVAlign::BOTTOM:
		align |= MGUI::eAlign::BOTTOM;
		break;

	case eVAlign::CENTER:
		align |= MGUI::eAlign::V_CENTER;
		break;

	case eVAlign::STRETCH:
		align |= MGUI::eAlign::V_STRETCH;
		break;
	}

	Widget->SetPickFlag(PickFlag);
	Widget->SetEnable(Enable);
}
#include "PropertyFieldButton.h"
#include "AllLookFeel.h"

PropertyFieldButton::PropertyFieldButton(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyField(group, obj, prop)
{
	mButton = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mWidget_Value);
	mButton->SetAlign(MGUI::eAlign::H_STRETCH | MGUI::eAlign::V_CENTER);
	mButton->SetRect(0, 0, 0, 24);
}

PropertyFieldButton::~PropertyFieldButton()
{
}

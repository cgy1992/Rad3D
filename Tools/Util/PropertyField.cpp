#include "PropertyField.h"
#include "PropertyGroup.h"


PropertyField::PropertyField(PropertyGroup * group, IObject * obj, const Property * prop)
	: mPropertyGroup(group)
	, mObject(obj)
	, mProperty(prop)
{
	mWidget = new MGUI::Widget(NULL, group->GetClientWidget());

	mLabel = new MGUI::Label(NULL, mWidget);
	mWidget_Value = new MGUI::Widget(NULL, mWidget);

	mLabel->SetCaption(String(prop->displayName).c_wstr());
}

PropertyField::~PropertyField()
{
	delete mWidget;
}

float PropertyField::_updateHeight(float y)
{
	float w = mWidget->GetParent()->GetAbsClient().w;

	mWidget->SetRect(0, y, w, 28);

	mLabel->SetRect(0, 0, 80, 28);
	mWidget_Value->SetRect(80, 0, w - 80, 28);

	return 28;
}
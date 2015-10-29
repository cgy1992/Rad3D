#include "PropertyFieldEnum.h"

PropertyFieldEnum::PropertyFieldEnum(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldComboBox(group, obj, prop)
{
	for (int i = 0; i < prop->enumObj->GetEnumCount(); ++i)
	{
		mComboBox->Append(String(prop->enumObj->GetEnum(i)->name).c_wstr());
	}

	int v = prop->AsInt(obj->GetPropertyData(prop));
	for (int i = 0; i < prop->enumObj->GetEnumCount(); ++i)
	{
		if (v == prop->enumObj->GetEnum(i)->val)
		{
			mComboBox->SetSelectIndex(i);
			break;
		}
	}

	mComboBox->E_SelectChanged += new cListener2<PropertyFieldEnum, const MGUI::Event *, int>(this, &PropertyFieldEnum::OnSelChanged);
}

PropertyFieldEnum::~PropertyFieldEnum()
{
}

void PropertyFieldEnum::OnSelChanged(const MGUI::Event * e, int index)
{
	d_assert (index >= 0 && index < mProperty->enumObj->GetEnumCount());

	int v = mProperty->enumObj->GetEnum(index)->val;
	mObject->SetPropertyData(mProperty, &v);
}

#include "PropertyFieldEnum.h"
#include "AllLookFeel.h"

namespace Rad {

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

	PropertyFieldBitFlag::PropertyFieldBitFlag(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyField(group, obj, prop)
	{
		d_assert (prop->enumObj != NULL);

		mCheckBox = new MGUI::CheckBox(AllLookFeel::Instance()->GetCheckBox(), mWidget_Value);
		mCheckBox->SetAlign(MGUI::eAlign::V_CENTER);
		mCheckBox->E_Checked += new cListener2<PropertyFieldBitFlag, const MGUI::Event *, bool>(this, &PropertyFieldBitFlag::OnChecked);

		mComboBox = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mWidget_Value);
		mComboBox->SetRect(mCheckBox->GetRect().w + 2, 0, 0, 24);
		mComboBox->SetAlign(MGUI::eAlign::R_STRETCH | MGUI::eAlign::V_CENTER);
		mComboBox->SetOrder(mWidget_Value->GetOrder() + 1);

		for (int i = 0; i < prop->enumObj->GetEnumCount(); ++i)
		{
			mComboBox->Append(String(prop->enumObj->GetEnum(i)->name).c_wstr());
		}

		mComboBox->E_SelectChanged += new cListener2<PropertyFieldBitFlag, const MGUI::Event *, int>(this, &PropertyFieldBitFlag::OnSelChanged);

		if (mComboBox->GetCount() > 0)
		{
			mComboBox->SetSelectIndex(0);
		}
	}

	PropertyFieldBitFlag::~PropertyFieldBitFlag()
	{
	}

	void PropertyFieldBitFlag::OnSelChanged(const MGUI::Event * e, int index)
	{
		int v = mProperty->AsInt(mObject->GetPropertyData(mProperty));
		int flag = mProperty->enumObj->GetEnum(index)->val;

		mCheckBox->SetChecked((v & flag) != 0);
	}

	void PropertyFieldBitFlag::OnChecked(const MGUI::Event * e, bool checked)
	{
		int index = mComboBox->GetSelectIndex();
		int v = mProperty->AsInt(mObject->GetPropertyData(mProperty));
		int flag = mProperty->enumObj->GetEnum(index)->val;

		if (checked)
		{
			v |= flag;
		}
		else
		{
			v &= ~flag;
		}

		mObject->SetPropertyData(mProperty, &v);
	}

}


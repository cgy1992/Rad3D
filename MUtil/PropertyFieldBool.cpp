#include "PropertyFieldBool.h"

namespace Rad {

	PropertyFieldBool::PropertyFieldBool(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyFieldComboBox(group, obj, prop)
	{
		mComboBox->Append(WSTR_("True"));
		mComboBox->Append(WSTR_("False"));

		bool b = prop->AsBool(obj->GetPropertyData(prop));

		mComboBox->SetSelectIndex(b ? 0 : 1);

		mComboBox->E_SelectChanged += new cListener2<PropertyFieldBool, const MGUI::Event *, int>(this, &PropertyFieldBool::OnSelChanged);
	}

	PropertyFieldBool::~PropertyFieldBool()
	{
	}

	void PropertyFieldBool::OnSelChanged(const MGUI::Event * e, int index)
	{
		bool v = (index == 0);

		mObject->SetPropertyData(mProperty, &v);
	}

}



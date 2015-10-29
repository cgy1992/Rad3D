#include "PropertyFieldClass.h"

PropertyFieldClass::PropertyFieldClass(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldComboBox(group, obj, prop)
{
	const char * str = prop->editorName;
	String temp, baseClass;
	str = str_substring(temp.c_str(), 128, str);
	str = str_substring(baseClass.c_str(), 128, str);

	d_assert (temp == "PT_Class" && baseClass != "");

	Root::ENUM_OBJECT_FACTORY(mClassNames, baseClass.c_str(), true);

	for (int i = 0; i < mClassNames.Size(); ++i)
	{
		mComboBox->Append(mClassNames[i].c_wstr());
	}

	FixedString32 myClass = mProperty->AsFixedString(mObject->GetPropertyData(mProperty));
	for (int i = 0; i < mClassNames.Size(); ++i)
	{
		if (mClassNames[i] == myClass)
		{
			mComboBox->SetSelectIndex(i);
			break;
		}
	}

	mComboBox->E_SelectChanged += new cListener2<PropertyFieldClass, const MGUI::Event *, int>(this, &PropertyFieldClass::OnSelChanged);
}

PropertyFieldClass::~PropertyFieldClass()
{
}

void PropertyFieldClass::OnSelChanged(const MGUI::Event * e, int index)
{
	FixedString32 myClass = mClassNames[index];

	mObject->SetPropertyData(mProperty, &myClass);
}
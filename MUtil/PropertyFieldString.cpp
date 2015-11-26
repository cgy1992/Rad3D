#include "PropertyFieldString.h"

namespace Rad {

	PropertyFieldFixedString::PropertyFieldFixedString(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyFieldEditBox(group, obj, prop)
	{
		mEditBox->E_KeyLostFocus += new cListener1<PropertyFieldFixedString, const MGUI::FocusEvent *>(this, &PropertyFieldFixedString::OnTextChanged);

		const void * data = obj->GetPropertyData(prop);
		if (data)
		{
			String text = prop->AsFixedString(data);
			mEditBox->SetCaption(text.c_wstr());
		}
	}

	PropertyFieldFixedString::~PropertyFieldFixedString()
	{
	}

	void PropertyFieldFixedString::OnTextChanged(const MGUI::FocusEvent *)
	{
		FixedString<1024> text;
		text.FromUnicode(mEditBox->GetCaption().c_str());

		mObject->SetPropertyData(mProperty, &text);
	}



	PropertyFieldString::PropertyFieldString(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyFieldEditBox(group, obj, prop)
	{
		mEditBox->E_KeyLostFocus += new cListener1<PropertyFieldString, const MGUI::FocusEvent *>(this, &PropertyFieldString::OnTextChanged);

		if (obj->GetPropertyData(prop))
		{
			String data = prop->AsString(obj->GetPropertyData(prop));
			String text = data.c_str();
			mEditBox->SetCaption(text.c_wstr());
		}
	}

	PropertyFieldString::~PropertyFieldString()
	{
	}

	void PropertyFieldString::OnTextChanged(const MGUI::FocusEvent *)
	{
		String text;
	
		text.FromUnicode(mEditBox->GetCaption().c_str());

		mObject->SetPropertyData(mProperty, &text);
	}

}

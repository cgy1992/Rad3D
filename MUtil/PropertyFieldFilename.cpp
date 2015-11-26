#include "PropertyFieldFilename.h"

namespace Rad {

	PropertyFieldFilename::PropertyFieldFilename(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyFieldButton(group, obj, prop)
	{
		mButton->E_Click += new cListener1<PropertyFieldFilename, const MGUI::ClickEvent *>(this, &PropertyFieldFilename::OnClick);

		char temp[256];
		const char * str = str_substring(temp, 256, prop->editorName);
		str = str_skipwhite(str);

		memset(&mFilter[0], 0, sizeof(mFilter));
		strcpy(&mFilter[0], str);
		mFilter.Replace('|', '\0');

		if (obj->GetPropertyData(prop))
		{
			String text;

			if (prop->type == PT_String)
			{
				text = prop->AsString(obj->GetPropertyData(prop));
			}
			else if (prop->type == PT_FixedString)
			{
				text = prop->AsFixedString(obj->GetPropertyData(prop));
			}

			mButton->SetCaption(text.c_wstr());
		}
	}

	PropertyFieldFilename::~PropertyFieldFilename()
	{
	}

	void PropertyFieldFilename::OnClick(const MGUI::ClickEvent * e)
	{
		String data;

		if (mProperty->type == PT_String)
		{
			data = mProperty->AsString(mObject->GetPropertyData(mProperty));;
		}
		else if (mProperty->type == PT_FixedString)
		{
			data = mProperty->AsFixedString(mObject->GetPropertyData(mProperty));;
		}
		else
		{
			d_assert (0);
		}

		data = ResourceManager::Instance()->GetAbsoluteSourceName(data);

		String filename = Util_GetOpenFile(mFilter.c_str(), FileHelper::GetFileDir(data));
		if (filename == "")
			return ;

		String file = ResourceManager::Instance()->GetRelativeSourceName(filename);;

		if (mProperty->type == PT_String)
		{
			mObject->SetPropertyData(mProperty, &file);
		}
		else
		{
			FixedString256 fixstring = file.c_str();
			mObject->SetPropertyData(mProperty, &fixstring);
		}

		mButton->SetCaption(file.c_wstr());
	}

}



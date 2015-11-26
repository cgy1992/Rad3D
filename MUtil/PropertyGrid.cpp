#include "PropertyGrid.h"
#include "PropertyFieldInt.h"
#include "PropertyFieldFloat.h"
#include "PropertyFieldQuaternion.h"
#include "PropertyFieldFilename.h"
#include "PropertyFieldString.h"
#include "PropertyFieldColor.h"
#include "PropertyFieldBool.h"
#include "PropertyFieldEnum.h"
#include "PropertyFieldLongText.h"
#include "PropertyFieldClass.h"
#include "AllLookFeel.h"

namespace Rad {

	PropertyGrid::PropertyGrid(MGUI::Widget * parent, bool nogroup)
		: MGUI::Widget(AllLookFeel::Instance()->GetPanel(), parent)
		, mObject(NULL)
		, mNoGroup(nogroup)
	{
	}

	PropertyGrid::~PropertyGrid()
	{
		Clear();
	}

	void PropertyGrid::Clear()
	{
		for (int i = 0; i < mGroups.Size(); ++i)
		{
			delete mGroups[i];
		}

		mGroups.Clear();

		mObject = NULL;
	}

	void PropertyGrid::Attach(IObject * obj, IPropertyGrid_UserCreator * userCreator)
	{
		Clear();

		if (obj == NULL)
			return ;

		mObject = obj;

		int size = obj->GetPropertySize();

		for (int i = 0; i < size; ++i)
			_createProperty(obj->GetProperty(i), userCreator);
	}

	PropertyField * PropertyGrid::_createProperty(const Property * prop, IPropertyGrid_UserCreator * userCreator)
	{
		PropertyField * field = NULL;
		PropertyGroup * pg = NULL;

		if (strcmp(prop->group, "") == 0)
			pg = _getPropertyGroup("Default");
		else
			pg = _getPropertyGroup(prop->group);

		if (userCreator != NULL)
		{
			field = userCreator->CreatePropertyField(pg, mObject, prop);
		}

		if (field == NULL)
		{
			if (strcmp(prop->editorName, "PT_Quaternion") == 0)
			{
				field = new PropertyFieldQuaternion(pg, mObject, prop);
			}
			else if (strcmp(prop->editorName, "PT_Color") == 0)
			{
				field = new PropertyFieldColor(pg, mObject, prop);
			}
			else if (strstr(prop->editorName, "PT_Filename") == prop->editorName)
			{
				field = new PropertyFieldFilename(pg, mObject, prop);
			}
			else if (strcmp(prop->editorName, "PT_LongText") == 0)
			{
				field = new PropertyFieldLongText(pg, mObject, prop);
			}
			else if (strcmp(prop->editorName, "PT_Script") == 0)
			{
				field = new PropertyFieldLongText(pg, mObject, prop);
			}
			else if (strstr(prop->editorName, "PT_Class") == prop->editorName)
			{
				field = new PropertyFieldClass(pg, mObject, prop);
			}
			else if (strcmp(prop->editorName, "PT_BitFlag") == 0)
			{
				field = new PropertyFieldBitFlag(pg, mObject, prop);
			}
		}

		if (field == NULL && stricmp(prop->editorName, "None") != 0)
		{
			switch (prop->type)
			{
			case PT_Bool:
				field = new PropertyFieldBool(pg, mObject, prop);
				break;

			case PT_Int:
				if (prop->enumObj == NULL)
					field = new PropertyFieldInt(pg, mObject, prop);
				else
					field = new PropertyFieldEnum(pg, mObject, prop);

				break;

			case PT_Int2:
				field = new PropertyFieldInt2(pg, mObject, prop);
				break;

			case PT_Int3:
				field = new PropertyFieldInt3(pg, mObject, prop);
				break;

			case PT_Int4:
				field = new PropertyFieldInt4(pg, mObject, prop);
				break;

			case PT_Float:
				field = new PropertyFieldFloat(pg, mObject, prop);
				break;

			case PT_Float2:
				field = new PropertyFieldFloat2(pg, mObject, prop);
				break;

			case PT_Float3:
				field = new PropertyFieldFloat3(pg, mObject, prop);
				break;

			case PT_Float4:
				field = new PropertyFieldFloat4(pg, mObject, prop);
				break;

			case PT_FixedString:
				field = new PropertyFieldFixedString(pg, mObject, prop);
				break;

			case PT_String:
				field = new PropertyFieldString(pg, mObject, prop);
				break;
			}
		}

		if (field)
			pg->Add(field);

		return field;
	}

	PropertyGroup * PropertyGrid::_getPropertyGroup(const char * gourpName)
	{
		for (int i = 0; i < mGroups.Size(); ++i)
		{
			if (mGroups[i]->GetName() == gourpName)
				return mGroups[i];
		}

		PropertyGroup * pg = new PropertyGroup(gourpName, this);

		mGroups.PushBack(pg);

		return pg;
	}

	void PropertyGrid::OnUpdate(float elapsedTime)
	{
		float y = 0;

		for (int i = 0; i < mGroups.Size(); ++i)
		{
			PropertyGroup * pg = mGroups[i];

			y += pg->_updateHeight(y);
		}

		MGUI::Rect rect = GetClient();
		rect.h = y;

		SetRect(MGUI::Helper::Instance()->GetExtendRect(rect, GetClientRect()));
	}

	void PropertyGrid::UpdateRenderItem(MGUI::Layout * _layout)
	{
		MGUI::Widget::UpdateRenderItem(_layout);
	}

}


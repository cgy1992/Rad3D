#include "PropertyFieldEditBox.h"
#include "AllLookFeel.h"

namespace Rad {

	PropertyFieldEditBox::PropertyFieldEditBox(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyField(group, obj, prop)
	{
		mEditBox = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mWidget_Value);
		mEditBox->SetAlign(MGUI::eAlign::H_STRETCH | MGUI::eAlign::V_CENTER);
		mEditBox->SetRect(0, 0, 0, 24);

		if (stricmp(String(prop->editorName).c_str(), "static") == 0)
		{
			mEditBox->SetStatic(true);
		}
	}

	PropertyFieldEditBox::~PropertyFieldEditBox()
	{
		safe_delete (mEditBox);
	}

}

#include "PropertyFieldComboBox.h"
#include "AllLookFeel.h"

namespace Rad {

	PropertyFieldComboBox::PropertyFieldComboBox(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyField(group, obj, prop)
	{
		mComboBox = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mWidget_Value);
		mComboBox->SetAlign(MGUI::eAlign::H_STRETCH | MGUI::eAlign::V_CENTER);
		mComboBox->SetRect(0, 0, 0, 24);
		mComboBox->SetOrder(mWidget_Value->GetOrder() + 1);
	}

	PropertyFieldComboBox::~PropertyFieldComboBox()
	{
	}

}


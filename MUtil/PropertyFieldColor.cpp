#include "PropertyFieldColor.h"
#include "ColorPickerDialog.h"
#include "AllLookFeel.h"

namespace Rad {

	PropertyFieldColor::PropertyFieldColor(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyField(group, obj, prop)
	{
		mWidget = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), mWidget_Value);
		mWidget->SetAlign(MGUI::eAlign::H_STRETCH | MGUI::eAlign::V_CENTER);
		mWidget->SetRect(0, 0, 0, 24);

		Float4 v;
		if (prop->type == PT_Float3)
		{
			Float3 v3 = prop->AsFloat3(obj->GetPropertyData(prop));
			v = Float4(v3.r, v3.g, v3.b, 1);
		}
		else if (prop->type == PT_Float4)
		{
			v = prop->AsFloat4(obj->GetPropertyData(prop));
		}
		else
		{
			d_assert (0);
		}

		mColor = Float4(v.r, v.g, v.b, v.a);
		mWidget->SetColor(mColor);
		mWidget->E_MouseClick += new cListener1<PropertyFieldColor, const MGUI::MouseEvent *>(this, &PropertyFieldColor::OnClick);
	}

	PropertyFieldColor::~PropertyFieldColor()
	{
	}

	void PropertyFieldColor::OnClick(const MGUI::MouseEvent *)
	{
		Float4 v;
		if (mProperty->type == PT_Float3)
		{
			Float3 v3 = mProperty->AsFloat3(mObject->GetPropertyData(mProperty));
			v = Float4(v3.r, v3.g, v3.b, 1);
		}
		else if (mProperty->type == PT_Float4)
		{
			v = mProperty->AsFloat4(mObject->GetPropertyData(mProperty));
		}

		mColor = Float4(v.r, v.g, v.b, v.a);

		ColorPickerDialog::Instance()->E_EndDialog += new cListener1<PropertyFieldColor, bool>(this, &PropertyFieldColor::OnEndDialog_);
		ColorPickerDialog::Instance()->E_Preview += new cListener0<PropertyFieldColor>(this, &PropertyFieldColor::OnPreview_);

		ColorPickerDialog::Instance()->DoModal(mColor, this);
	}

	void PropertyFieldColor::OnEndDialog_(bool _ok)
	{
		if (_ok)
			mColor = ColorPickerDialog::Instance()->GetCurrentColor();

		mColor.Saturate();

		mWidget->SetColor(mColor);

		mObject->SetPropertyData(mProperty, &mColor);
	}

	void PropertyFieldColor::OnPreview_()
	{
		Float4 v = ColorPickerDialog::Instance()->GetCurrentColor();

		mObject->SetPropertyData(mProperty, &v);
	}

}

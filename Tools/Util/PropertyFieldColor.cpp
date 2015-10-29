#include "PropertyFieldColor.h"
#include "ColorPickerDialog.h"
#include "AllLookFeel.h"

PropertyFieldColor3::PropertyFieldColor3(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyField(group, obj, prop)
{
	mWidget = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), mWidget_Value);
	mWidget->SetAlign(MGUI::eAlign::H_STRETCH | MGUI::eAlign::V_CENTER);
	mWidget->SetRect(0, 0, 0, 24);

	Float3 v = prop->AsFloat3(obj->GetPropertyData(prop));

	mColor = Float4(v.r, v.g, v.b, 1);
	mWidget->SetColor(mColor);
	mWidget->E_MouseClick += new cListener1<PropertyFieldColor3, const MGUI::MouseEvent *>(this, &PropertyFieldColor3::OnClick);
}

PropertyFieldColor3::~PropertyFieldColor3()
{
}

void PropertyFieldColor3::OnClick(const MGUI::MouseEvent *)
{
	Float3 v = mProperty->AsFloat3(mObject->GetPropertyData(mProperty));
	mColor = Float4(v.r, v.g, v.b, 1);

	ColorPickerDialog::Instance()->E_EndDialog += new cListener1<PropertyFieldColor3, bool>(this, &PropertyFieldColor3::OnEndDialog_);
	ColorPickerDialog::Instance()->E_Preview += new cListener0<PropertyFieldColor3>(this, &PropertyFieldColor3::OnPreview_);

	ColorPickerDialog::Instance()->DoModal(mColor, this);
}

void PropertyFieldColor3::OnEndDialog_(bool _ok)
{
	if (_ok)
		mColor = ColorPickerDialog::Instance()->GetCurrentColor();

	mWidget->SetColor(Float4(mColor.r, mColor.g, mColor.b, 1));

	mObject->SetPropertyData(mProperty, &mColor);
}

void PropertyFieldColor3::OnPreview_()
{
	Float4 v = ColorPickerDialog::Instance()->GetCurrentColor();

	mObject->SetPropertyData(mProperty, &v);
}




PropertyFieldColor4::PropertyFieldColor4(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyField(group, obj, prop)
{
	mWidget = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), mWidget_Value);
	mWidget->SetAlign(MGUI::eAlign::H_STRETCH | MGUI::eAlign::V_CENTER);
	mWidget->SetRect(0, 0, 0, 24);

	Float4 v = prop->AsFloat4(obj->GetPropertyData(prop));

	mColor = Float4(v.r, v.g, v.b, v.a);
	mWidget->SetColor(mColor);
	mWidget->E_MouseClick += new cListener1<PropertyFieldColor4, const MGUI::MouseEvent *>(this, &PropertyFieldColor4::OnClick);
}

PropertyFieldColor4::~PropertyFieldColor4()
{
}

void PropertyFieldColor4::OnClick(const MGUI::MouseEvent *)
{
	Float4 v = mProperty->AsFloat4(mObject->GetPropertyData(mProperty));
	mColor = Float4(v.r, v.g, v.b, v.a);

	ColorPickerDialog::Instance()->E_EndDialog += new cListener1<PropertyFieldColor4, bool>(this, &PropertyFieldColor4::OnEndDialog_);
	ColorPickerDialog::Instance()->E_Preview += new cListener0<PropertyFieldColor4>(this, &PropertyFieldColor4::OnPreview_);

	ColorPickerDialog::Instance()->DoModal(mColor, this);
}

void PropertyFieldColor4::OnEndDialog_(bool _ok)
{
	if (_ok)
		mColor = ColorPickerDialog::Instance()->GetCurrentColor();

	mColor.Saturate();

	mWidget->SetColor(mColor);

	mObject->SetPropertyData(mProperty, &mColor);
}

void PropertyFieldColor4::OnPreview_()
{
	Float4 v = ColorPickerDialog::Instance()->GetCurrentColor();

	mObject->SetPropertyData(mProperty, &v);
}
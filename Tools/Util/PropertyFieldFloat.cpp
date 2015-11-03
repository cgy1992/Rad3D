#include "PropertyFieldFloat.h"

PropertyFieldFloat::PropertyFieldFloat(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldEditBox(group, obj, prop)
{
	float data = prop->AsFloat(obj->GetPropertyData(prop));
	String text = f2str(data, true);

	mEditBox->SetCaption(text.c_wstr());
	mEditBox->E_KeyLostFocus += new cListener1<PropertyFieldFloat, const MGUI::FocusEvent *>(this, &PropertyFieldFloat::OnTextChanged);
}

PropertyFieldFloat::~PropertyFieldFloat()
{
}

void PropertyFieldFloat::OnTextChanged(const MGUI::FocusEvent *)
{
	String text;
	text.FromUnicode(mEditBox->GetCaption().c_str());

	float val = text.ToFloat();

	mObject->SetPropertyData(mProperty, &val);
}


//
PropertyFieldFloat2::PropertyFieldFloat2(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldButton(group, obj, prop)
{
	Float2 data = mProperty->AsFloat2(mObject->GetPropertyData(mProperty));
	String text = 
		f2str(data.x, true) + " " +
		f2str(data.y, true);

	mButton->SetCaption(text.c_wstr());
	mButton->E_Click += new cListener1<PropertyFieldFloat2, const MGUI::ClickEvent *>(this, &PropertyFieldFloat2::OnClick);

	mLayout = MGUI::Layout::Load("PropertyFieldEditBox2.layout", NULL);
	mLayout->SetOrder(MGUI::eOrder::TOP - 100);
	mLayout->SetVisible(false);

	mEditBoxX = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("x");
	mEditBoxY = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("y");

	mLayout->GetChild(0)->GetChildT<MGUI::Button>("OK")->E_Click +=
		new cListener1<PropertyFieldFloat2, const MGUI::ClickEvent *>(this, &PropertyFieldFloat2::OnOK);
	mLayout->GetChild(0)->GetChildT<MGUI::Button>("Cancel")->E_Click +=
		new cListener1<PropertyFieldFloat2, const MGUI::ClickEvent *>(this, &PropertyFieldFloat2::OnCancel);
}

PropertyFieldFloat2::~PropertyFieldFloat2()
{
	delete mLayout;
}

void PropertyFieldFloat2::OnClick(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(true);

	Float2 data = mProperty->AsFloat2(mObject->GetPropertyData(mProperty));
	String x_text = f2str(data.x, true);
	String y_text = f2str(data.y, true);

	mEditBoxX->SetCaption(x_text.c_wstr());
	mEditBoxY->SetCaption(y_text.c_wstr());
}

void PropertyFieldFloat2::OnOK(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);

	String x_text, y_text;
	
	x_text.FromUnicode(mEditBoxX->GetCaption().c_str());
	y_text.FromUnicode(mEditBoxY->GetCaption().c_str());

	Float2 val;
	val.x = x_text.ToFloat();
	val.y = y_text.ToFloat();

	mObject->SetPropertyData(mProperty, &val);

	String text = 
		f2str(val.x, true) + " " +
		f2str(val.y, true);

	mButton->SetCaption(text.c_wstr());
}

void PropertyFieldFloat2::OnCancel(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);
}


//
PropertyFieldFloat3::PropertyFieldFloat3(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldButton(group, obj, prop)
{
	Float3 data = mProperty->AsFloat3(mObject->GetPropertyData(mProperty));
	String text = 
		f2str(data.x, true) + " " +
		f2str(data.y, true) + " " +
		f2str(data.z, true);

	mButton->SetCaption(text.c_wstr());
	mButton->E_Click += new cListener1<PropertyFieldFloat3, const MGUI::ClickEvent *>(this, &PropertyFieldFloat3::OnClick);

	mLayout = MGUI::Layout::Load("PropertyFieldEditBox3.layout", NULL);
	mLayout->SetOrder(MGUI::eOrder::TOP - 100);
	mLayout->SetVisible(false);

	mEditBoxX = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("x");
	mEditBoxY = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("y");
	mEditBoxZ = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("z");

	mLayout->GetChild(0)->GetChildT<MGUI::Button>("OK")->E_Click +=
		new cListener1<PropertyFieldFloat3, const MGUI::ClickEvent *>(this, &PropertyFieldFloat3::OnOK);
	mLayout->GetChild(0)->GetChildT<MGUI::Button>("Cancel")->E_Click +=
		new cListener1<PropertyFieldFloat3, const MGUI::ClickEvent *>(this, &PropertyFieldFloat3::OnCancel);
}

PropertyFieldFloat3::~PropertyFieldFloat3()
{
	delete mLayout;
}

void PropertyFieldFloat3::OnClick(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(true);

	Float3 data = mProperty->AsFloat3(mObject->GetPropertyData(mProperty));
	String x_text = f2str(data.x, true);
	String y_text = f2str(data.y, true);
	String z_text = f2str(data.z, true);

	mEditBoxX->SetCaption(x_text.c_wstr());
	mEditBoxY->SetCaption(y_text.c_wstr());
	mEditBoxZ->SetCaption(z_text.c_wstr());
}

void PropertyFieldFloat3::OnOK(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);

	String x_text, y_text, z_text;
	x_text.FromUnicode(mEditBoxX->GetCaption().c_str());
	y_text.FromUnicode(mEditBoxY->GetCaption().c_str());
	z_text.FromUnicode(mEditBoxZ->GetCaption().c_str());

	Float3 val;
	val.x = x_text.ToFloat();
	val.y = y_text.ToFloat();
	val.z = z_text.ToFloat();

	mObject->SetPropertyData(mProperty, &val);

	String text = 
		f2str(val.x, true) + " " +
		f2str(val.y, true) + " " +
		f2str(val.z, true);

	mButton->SetCaption(text.c_wstr());
}

void PropertyFieldFloat3::OnCancel(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);
}


//
PropertyFieldFloat4::PropertyFieldFloat4(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldButton(group, obj, prop)
{
	Float4 data = mProperty->AsFloat4(mObject->GetPropertyData(mProperty));
	String text = 
		f2str(data.x, true) + " " +
		f2str(data.y, true) + " " +
		f2str(data.z, true) + " " +
		f2str(data.w, true);

	mButton->SetCaption(text.c_wstr());
	mButton->E_Click += new cListener1<PropertyFieldFloat4, const MGUI::ClickEvent *>(this, &PropertyFieldFloat4::OnClick);

	mLayout = MGUI::Layout::Load("PropertyFieldEditBox4.layout", NULL);
	mLayout->SetOrder(MGUI::eOrder::TOP - 100);
	mLayout->SetVisible(false);

	mEditBoxX = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("x");
	mEditBoxY = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("y");
	mEditBoxZ = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("z");
	mEditBoxW = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("w");

	mLayout->GetChild(0)->GetChildT<MGUI::Button>("OK")->E_Click +=
		new cListener1<PropertyFieldFloat4, const MGUI::ClickEvent *>(this, &PropertyFieldFloat4::OnOK);
	mLayout->GetChild(0)->GetChildT<MGUI::Button>("Cancel")->E_Click +=
		new cListener1<PropertyFieldFloat4, const MGUI::ClickEvent *>(this, &PropertyFieldFloat4::OnCancel);
}

PropertyFieldFloat4::~PropertyFieldFloat4()
{
	delete mLayout;
}

void PropertyFieldFloat4::OnClick(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(true);

	Float4 data = mProperty->AsFloat4(mObject->GetPropertyData(mProperty));
	String x_text = f2str(data.x, true);
	String y_text = f2str(data.y, true);
	String z_text = f2str(data.z, true);
	String w_text = f2str(data.w, true);

	mEditBoxX->SetCaption(x_text.c_wstr());
	mEditBoxY->SetCaption(y_text.c_wstr());
	mEditBoxZ->SetCaption(z_text.c_wstr());
	mEditBoxW->SetCaption(w_text.c_wstr());
}

void PropertyFieldFloat4::OnOK(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);

	String x_text, y_text, z_text, w_text;
	x_text.FromUnicode(mEditBoxX->GetCaption().c_str());
	y_text.FromUnicode(mEditBoxY->GetCaption().c_str());
	z_text.FromUnicode(mEditBoxZ->GetCaption().c_str());
	w_text.FromUnicode(mEditBoxW->GetCaption().c_str());

	Float4 val;
	val.x = x_text.ToFloat();
	val.y = y_text.ToFloat();
	val.z = z_text.ToFloat();
	val.w = w_text.ToFloat();

	mObject->SetPropertyData(mProperty, &val);

	String text = 
		f2str(val.x, true) + " " +
		f2str(val.y, true) + " " +
		f2str(val.z, true) + " " +
		f2str(val.w, true);

	mButton->SetCaption(text.c_wstr());
}

void PropertyFieldFloat4::OnCancel(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);
}
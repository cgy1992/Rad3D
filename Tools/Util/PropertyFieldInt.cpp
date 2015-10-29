#include "PropertyFieldInt.h"

PropertyFieldInt::PropertyFieldInt(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldEditBox(group, obj, prop)
{
	int data = prop->AsInt(obj->GetPropertyData(prop));
	String text;

	if (stricmp(prop->editorName, "%x") == 0)
	{
		text.Format("%x", data);
	}
	else if (stricmp(prop->editorName, "%b") == 0)
	{
		text.Format("%b", data);
	}
	else
	{
		text.Format("%d", data);
	}

	mEditBox->SetCaption(text.c_wstr());
	mEditBox->E_KeyLostFocus += new cListener1<PropertyFieldInt, const MGUI::FocusEvent *>(this, &PropertyFieldInt::OnTextChanged);
}

PropertyFieldInt::~PropertyFieldInt()
{
}

void PropertyFieldInt::OnTextChanged(const MGUI::FocusEvent *)
{
	String text;
	text.FromUnicode(mEditBox->GetCaption().c_str());

	int val = 0;
	if (stricmp(mProperty->editorName, "%x") == 0)
	{
		sscanf(text.c_str(), "%x", &val);
	}
	else if (stricmp(mProperty->editorName, "%b") == 0)
	{
		sscanf(text.c_str(), "%b", &val);
	}
	else
	{
		sscanf(text.c_str(), "%d", &val);
	}

	mObject->SetPropertyData(mProperty, &val);
}


//
PropertyFieldInt2::PropertyFieldInt2(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldButton(group, obj, prop)
{
	Int2 data = mProperty->AsInt2(mObject->GetPropertyData(mProperty));
	String text = 
		i2str(data.x) + " " +
		i2str(data.y);

	mButton->SetCaption(text.c_wstr());
	mButton->E_Click += new cListener1<PropertyFieldInt2, const MGUI::ClickEvent *>(this, &PropertyFieldInt2::OnClick);

	mLayout = MGUI::Layout::Load("PropertyFieldEditBox2.layout", NULL);
	mLayout->SetOrder(MGUI::eOrder::TOP - 100);
	mLayout->SetVisible(false);

	mEditBoxX = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("x");
	mEditBoxY = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("y");

	mLayout->GetChild(0)->GetChildT<MGUI::Button>("OK")->E_Click +=
		new cListener1<PropertyFieldInt2, const MGUI::ClickEvent *>(this, &PropertyFieldInt2::OnOK);
	mLayout->GetChild(0)->GetChildT<MGUI::Button>("Cancel")->E_Click +=
		new cListener1<PropertyFieldInt2, const MGUI::ClickEvent *>(this, &PropertyFieldInt2::OnCancel);
}

PropertyFieldInt2::~PropertyFieldInt2()
{
	delete mLayout;
}

void PropertyFieldInt2::OnClick(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(true);

	Int2 data = mProperty->AsInt2(mObject->GetPropertyData(mProperty));
	String x_text = i2str(data.x);
	String y_text = i2str(data.y);

	mEditBoxX->SetCaption(x_text.c_wstr());
	mEditBoxY->SetCaption(y_text.c_wstr());
}

void PropertyFieldInt2::OnOK(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);

	String x_text, y_text;
	
	x_text.FromUnicode(mEditBoxX->GetCaption().c_str());
	y_text.FromUnicode(mEditBoxY->GetCaption().c_str());

	Int2 val;
	val.x = x_text.ToInt();
	val.y = y_text.ToInt();

	mObject->SetPropertyData(mProperty, &val);

	String text = 
		i2str(val.x) + " " +
		i2str(val.y);

	mButton->SetCaption(text.c_wstr());
}

void PropertyFieldInt2::OnCancel(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);
}


//
PropertyFieldInt3::PropertyFieldInt3(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldButton(group, obj, prop)
{
	Int3 data = mProperty->AsInt3(mObject->GetPropertyData(mProperty));
	String text = 
		i2str(data.x) + " " +
		i2str(data.y) + " " +
		i2str(data.z);

	mButton->SetCaption(text.c_wstr());
	mButton->E_Click += new cListener1<PropertyFieldInt3, const MGUI::ClickEvent *>(this, &PropertyFieldInt3::OnClick);

	mLayout = MGUI::Layout::Load("PropertyFieldEditBox3.layout", NULL);
	mLayout->SetOrder(MGUI::eOrder::TOP - 100);
	mLayout->SetVisible(false);

	mEditBoxX = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("x");
	mEditBoxY = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("y");
	mEditBoxZ = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("z");

	mLayout->GetChild(0)->GetChildT<MGUI::Button>("OK")->E_Click +=
		new cListener1<PropertyFieldInt3, const MGUI::ClickEvent *>(this, &PropertyFieldInt3::OnOK);
	mLayout->GetChild(0)->GetChildT<MGUI::Button>("Cancel")->E_Click +=
		new cListener1<PropertyFieldInt3, const MGUI::ClickEvent *>(this, &PropertyFieldInt3::OnCancel);
}

PropertyFieldInt3::~PropertyFieldInt3()
{
	delete mLayout;
}

void PropertyFieldInt3::OnClick(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(true);

	Int3 data = mProperty->AsInt3(mObject->GetPropertyData(mProperty));
	String x_text = i2str(data.x);
	String y_text = i2str(data.y);
	String z_text = i2str(data.z);

	mEditBoxX->SetCaption(x_text.c_wstr());
	mEditBoxY->SetCaption(y_text.c_wstr());
	mEditBoxZ->SetCaption(z_text.c_wstr());
}

void PropertyFieldInt3::OnOK(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);

	String x_text, y_text, z_text;

	x_text.FromUnicode(mEditBoxX->GetCaption().c_str());
	y_text.FromUnicode(mEditBoxY->GetCaption().c_str());
	z_text.FromUnicode(mEditBoxZ->GetCaption().c_str());

	Int3 val;
	val.x = x_text.ToInt();
	val.y = y_text.ToInt();
	val.z = z_text.ToInt();

	mObject->SetPropertyData(mProperty, &val);

	String text = 
		i2str(val.x) + " " +
		i2str(val.y) + " " +
		i2str(val.z);

	mButton->SetCaption(text.c_wstr());
}

void PropertyFieldInt3::OnCancel(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);
}


//
PropertyFieldInt4::PropertyFieldInt4(PropertyGroup * group, IObject * obj, const Property * prop)
	: PropertyFieldButton(group, obj, prop)
{
	Int4 data = mProperty->AsInt4(mObject->GetPropertyData(mProperty));
	String text = 
		i2str(data.x) + " " +
		i2str(data.y) + " " +
		i2str(data.z) + " " +
		i2str(data.w);

	mButton->SetCaption(text.c_wstr());
	mButton->E_Click += new cListener1<PropertyFieldInt4, const MGUI::ClickEvent *>(this, &PropertyFieldInt4::OnClick);

	mLayout = MGUI::Layout::Load("PropertyFieldEditBox4.layout", NULL);
	mLayout->SetOrder(MGUI::eOrder::TOP - 100);
	mLayout->SetVisible(false);

	mEditBoxX = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("x");
	mEditBoxY = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("y");
	mEditBoxZ = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("z");
	mEditBoxW = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("w");

	mLayout->GetChild(0)->GetChildT<MGUI::Button>("OK")->E_Click +=
		new cListener1<PropertyFieldInt4, const MGUI::ClickEvent *>(this, &PropertyFieldInt4::OnOK);
	mLayout->GetChild(0)->GetChildT<MGUI::Button>("Cancel")->E_Click +=
		new cListener1<PropertyFieldInt4, const MGUI::ClickEvent *>(this, &PropertyFieldInt4::OnCancel);
}

PropertyFieldInt4::~PropertyFieldInt4()
{
	delete mLayout;
}

void PropertyFieldInt4::OnClick(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(true);

	Int4 data = mProperty->AsInt4(mObject->GetPropertyData(mProperty));
	String x_text = i2str(data.x);
	String y_text = i2str(data.y);
	String z_text = i2str(data.z);
	String w_text = i2str(data.w);

	mEditBoxX->SetCaption(x_text.c_wstr());
	mEditBoxY->SetCaption(y_text.c_wstr());
	mEditBoxZ->SetCaption(z_text.c_wstr());
	mEditBoxW->SetCaption(w_text.c_wstr());
}

void PropertyFieldInt4::OnOK(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);

	String x_text, y_text, z_text, w_text;
	x_text.FromUnicode(mEditBoxX->GetCaption().c_str());
	y_text.FromUnicode(mEditBoxY->GetCaption().c_str());
	z_text.FromUnicode(mEditBoxZ->GetCaption().c_str());
	w_text.FromUnicode(mEditBoxW->GetCaption().c_str());

	Int4 val;
	val.x = x_text.ToInt();
	val.y = y_text.ToInt();
	val.z = z_text.ToInt();
	val.w = z_text.ToInt();

	mObject->SetPropertyData(mProperty, &val);

	String text = 
		i2str(val.x) + " " +
		i2str(val.y) + " " +
		i2str(val.z) + " " +
		i2str(val.w);

	mButton->SetCaption(text.c_wstr());
}

void PropertyFieldInt4::OnCancel(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);
}
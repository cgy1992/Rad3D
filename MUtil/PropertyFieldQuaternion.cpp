#include "PropertyFieldQuaternion.h"

namespace Rad {

	PropertyFieldQuaternion::PropertyFieldQuaternion(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyFieldButton(group, obj, prop)
	{
		Float4 v = mProperty->AsFloat4(mObject->GetPropertyData(mProperty));
		Quat q(v.x, v.y, v.z, v.w);
		Float3 data = q.ToEulerAngle();
		String text = 
			f2str(data.x, true) + " " +
			f2str(data.y, true) + " " +
			f2str(data.z, true);

		mButton->SetCaption(text.c_wstr());
		mButton->E_Click += new cListener1<PropertyFieldQuaternion, const MGUI::ClickEvent *>(this, &PropertyFieldQuaternion::OnClick);

		mLayout = MGUI::Layout::Load("PropertyFieldEditBox3.layout", NULL);
		mLayout->SetOrder(MGUI::eOrder::TOP - 100);
		mLayout->SetVisible(false);

		mEditBoxX = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("x");
		mEditBoxY = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("y");
		mEditBoxZ = mLayout->GetChild(0)->GetChildT<MGUI::EditBox>("z");

		mLayout->GetChild(0)->GetChildT<MGUI::Button>("OK")->E_Click +=
			new cListener1<PropertyFieldQuaternion, const MGUI::ClickEvent *>(this, &PropertyFieldQuaternion::OnOK);
		mLayout->GetChild(0)->GetChildT<MGUI::Button>("Cancel")->E_Click +=
			new cListener1<PropertyFieldQuaternion, const MGUI::ClickEvent *>(this, &PropertyFieldQuaternion::OnCancel);
	}

	PropertyFieldQuaternion::~PropertyFieldQuaternion()
	{
		delete mLayout;
	}

	void PropertyFieldQuaternion::OnClick(const MGUI::ClickEvent * e)
	{
		mLayout->SetVisible(true);

		Float4 v = mProperty->AsFloat4(mObject->GetPropertyData(mProperty));
		Quat q(v.x, v.y, v.z, v.w);
		Float3 data = q.ToEulerAngle();
		String x_text = f2str(data.x, true);
		String y_text = f2str(data.y, true);
		String z_text = f2str(data.z, true);

		mEditBoxX->SetCaption(x_text.c_wstr());
		mEditBoxY->SetCaption(y_text.c_wstr());
		mEditBoxZ->SetCaption(z_text.c_wstr());
	}

	void PropertyFieldQuaternion::OnOK(const MGUI::ClickEvent * e)
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

		Quat q;
		q.FromEulerAngle(val);
		mObject->SetPropertyData(mProperty, &q);

		String text = 
			f2str(val.x, true) + " " +
			f2str(val.y, true) + " " +
			f2str(val.z, true);

		mButton->SetCaption(text.c_wstr());
	}

	void PropertyFieldQuaternion::OnCancel(const MGUI::ClickEvent * e)
	{
		mLayout->SetVisible(false);
	}

}

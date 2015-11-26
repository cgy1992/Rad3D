#include "PropertyFieldLongText.h"
#include "AllLookFeel.h"
#include "RadC.h"

namespace Rad {

	PropertyFieldLongText::PropertyFieldLongText(PropertyGroup * group, IObject * obj, const Property * prop)
		: PropertyFieldButton(group, obj, prop)
	{
		String text = prop->AsString(obj->GetPropertyData(prop));
		FixedString16 caption;
		strncpy(caption.c_str(), text.c_str(), 15);
		caption[15] = 0;

		int e_ascii = 0;
		for (int i = 0; i < caption.Length(); ++i)
		{
			if (caption[i] & 0x80)
			{
				e_ascii += 1;
			}
		}

		if (e_ascii & 1)
		{
			caption[14] = 0;
		}

		mButton->SetCaption(caption.c_wstr());
		mButton->E_Click += new cListener1<PropertyFieldLongText, const MGUI::ClickEvent *>(this, &PropertyFieldLongText::OnClick);

		mDialog = new MGUI::Layout(AllLookFeel::Instance()->GetWhite(), NULL);
		mDialog->SetOrder(MGUI::eOrder::TOP - 100);
		mDialog->SetAlign(MGUI::eAlign::STRETCH);
		mDialog->SetColor(Float4(0.3f, 0.3f, 0.3f, 0.5f));

		MGUI::Layout * layout = MGUI::Layout::Load("PropertyFieldLongText.layout", mDialog);
		d_assert (layout != NULL);
		layout->SetInheritColor(false);
		layout->SetInheritAlpha(false);
		layout->SetAlign(MGUI::eAlign::CENTER);

		MGUI::Widget * editor = layout->GetChild("Editor");
		MGUI::Widget * libs = layout->GetChild("RadCLibs");

		MGUI::Button * bnOK = (MGUI::Button *)editor->GetChild("OK");
		MGUI::Button * bnCancel = (MGUI::Button *)editor->GetChild("Cancel");
		MGUI::Button * bnCompile = (MGUI::Button *)editor->GetChild("Compile");

		bnOK->E_Click += new cListener1<PropertyFieldLongText, const MGUI::ClickEvent *>(this, &PropertyFieldLongText::OnOK);
		bnCancel->E_Click += new cListener1<PropertyFieldLongText, const MGUI::ClickEvent *>(this, &PropertyFieldLongText::OnCancel);
		bnCompile->E_Click += new cListener1<PropertyFieldLongText, const MGUI::ClickEvent *>(this, &PropertyFieldLongText::OnCompile);

		mEditView = new MGUI::EditView(AllLookFeel::Instance()->GetEditBox(), editor->GetChild("Script"));
		mEditView->SetAlign(MGUI::eAlign::STRETCH);

		mDialog->SetVisible(false);

		mLibBox = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), libs->GetChild("Libs"));
		mFunctionBox = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), libs->GetChild("Functions"));
		mStatusLabel = layout->GetChild("Status")->GetChildT<MGUI::Label>("Label");

		mLibBox->SetAlign(MGUI::eAlign::STRETCH);
		mFunctionBox->SetAlign(MGUI::eAlign::STRETCH);

		Set<FixedString32> libset;
		for (int i = 0; i < radc_getRegSize(); ++i)
		{
			FixedString32 libname = radc_getLibName(radc_getReg(i)._lib);
			if (libset.Find(libname) == -1)
			{
				mLibBox->Append(libname.c_wstr());
				libset.Insert(libname);
			}
		}

		mLibBox->E_SelectChanged += new cListener2<PropertyFieldLongText, const MGUI::Event *, int>(this, &PropertyFieldLongText::OnLibChanged);
		mFunctionBox->E_SelectChanged += new cListener2<PropertyFieldLongText, const MGUI::Event *, int>(this, &PropertyFieldLongText::OnFunctionChanged);
	}

	PropertyFieldLongText::~PropertyFieldLongText()
	{
		delete mDialog;
	}

	void PropertyFieldLongText::OnClick(const MGUI::ClickEvent * e)
	{
		mDialog->SetVisible(true);

		String text = mProperty->AsString(mObject->GetPropertyData(mProperty));
		mEditView->SetCaption(text.c_wstr());
	}

	void PropertyFieldLongText::OnOK(const MGUI::ClickEvent * e)
	{
		String text;
		text.FromUnicode(mEditView->GetCaption().c_str());

		mObject->SetPropertyData(mProperty, &text);

		FixedString16 caption;
		strncpy(caption.c_str(), text.c_str(), 15);
		caption[15] = 0;

		int e_ascii = 0;
		for (int i = 0; i < caption.Length(); ++i)
		{
			if (caption[i] & 0x80)
			{
				e_ascii += 1;
			}
		}

		if (e_ascii & 1)
		{
			caption[14] = 0;
		}

		mButton->SetCaption(caption.c_wstr());

		mDialog->SetVisible(false);
	}

	void PropertyFieldLongText::OnCancel(const MGUI::ClickEvent * e)
	{
		mDialog->SetVisible(false);
	}

	void PropertyFieldLongText::OnCompile(const MGUI::ClickEvent * e)
	{
		String text;
		text.FromUnicode(mEditView->GetCaption().c_str());

		const char * str = str_skipwhite(text.c_str());
		if (*str)
		{
			radc_do(str, RADC_OPATION_COMPILE_ONLY, NULL, NULL);
		}
	}

	void PropertyFieldLongText::OnLibChanged(const MGUI::Event * e, int index)
	{
		d_assert (index >= 0);

		mFunctionBox->Clear();
		mStatusLabel->SetCaption(WS(""));

		FixedString32 libname;
		libname.FromUnicode(mLibBox->GetText(index).c_str());

		for (int i = 0; i < radc_getRegSize(); ++i)
		{
			const radc_reg & reg = radc_getReg(i);

			if (radc_getLibName(reg._lib) == libname)
			{
				String text = reg.name;
				mFunctionBox->Append(text.c_wstr(), (void *)i);
			}
		}
	}

	void PropertyFieldLongText::OnFunctionChanged(const MGUI::Event * e, int index)
	{
		mStatusLabel->SetCaption(WS(""));

		if (index >= 0)
		{
			int i = (int)mFunctionBox->GetUserData(index);

			const radc_reg & reg = radc_getReg(i);
			if (reg.desc)
			{
				String text = reg.desc;
				mStatusLabel->SetCaption(text.c_wstr());
			}
		}
	}

}

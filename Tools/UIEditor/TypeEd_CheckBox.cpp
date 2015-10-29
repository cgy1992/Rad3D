#include "TypeEd_CheckBox.h"
#include "AllLookFeel.h"

TypeEd_CheckBox::TypeEd_CheckBox(MGUI::Widget * widget, MGUI::Widget * panel)
{
	d_assert (TYPE_OF(MGUI::CheckBox, widget));

	mCheckBox = (MGUI::CheckBox *)widget;

	float top = 0;
	const float K_Space = 8;

	mLabel_Checked = new MGUI::Label(NULL, panel);
	mLabel_Checked->SetCaption(L"Checked:");
	mLabel_Checked->SetRect(0, top, 64, 24);

	mComboBox_Checked = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), panel);
	mComboBox_Checked->SetRect(80, top, 162, 24);
	mComboBox_Checked->Append(L"False");
	mComboBox_Checked->Append(L"True");

	top += 24 + K_Space;

	// Init
	mComboBox_Checked->SetSelectIndex(mCheckBox->GetChecked() ? 1 : 0);

	mComboBox_Checked->E_SelectChanged += new cListener2<TypeEd_CheckBox, const MGUI::Event *, int>(this, &TypeEd_CheckBox::OnCheckedChanged);
}

TypeEd_CheckBox::~TypeEd_CheckBox()
{
	delete mLabel_Checked;
	delete mComboBox_Checked;
}

void TypeEd_CheckBox::OnCheckedChanged(const MGUI::Event * e, int index)
{
	if (index == 0)
	{
		mCheckBox->SetChecked(false);
	}
	else if (index == 1)
	{
		mCheckBox->SetChecked(true);
	}
}

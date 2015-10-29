#include "ParticleFXNewDialog.h"
#include "AllLookFeel.h"

ParticleFXNewDialog::ParticleFXNewDialog()
{
	mDialog = new MGUI::Layout(NULL, NULL);
	mDialog->SetOrder(MGUI::eOrder::TOP - 100);
	mDialog->SetColor(Float4(0.15f, 0.15f, 0.15f, 0.5f));
	mDialog->SetAlign(MGUI::eAlign::STRETCH);
	mDialog->SetVisible(false);

	mLayout = MGUI::Layout::Load("ParticleFXType.layout", mDialog);
	mLayout->SetAlign(MGUI::eAlign::CENTER);
	mLayout->SetInheritColor(false);
	mLayout->SetInheritAlpha(false);

	mComboBox_Type = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout->GetChild("Type"));
	mComboBox_Type->SetAlign(MGUI::eAlign::STRETCH);
	
	mButton_OK = (MGUI::Button *)mLayout->GetChild("OK");
	mButton_Cancel = (MGUI::Button *)mLayout->GetChild("Cancel");

	mButton_OK->E_Click += new cListener1<ParticleFXNewDialog, const MGUI::ClickEvent *>(this, &ParticleFXNewDialog::OnOK);
	mButton_Cancel->E_Click += new cListener1<ParticleFXNewDialog, const MGUI::ClickEvent *>(this, &ParticleFXNewDialog::OnCancel);
}

ParticleFXNewDialog::~ParticleFXNewDialog()
{
	delete mDialog;
}

void ParticleFXNewDialog::DoModal()
{
	if (mComboBox_Type->GetCount() > 0)
		mComboBox_Type->SetSelectIndex(0);

	mDialog->SetVisible(true);
}

String ParticleFXNewDialog::GetTypeName()
{
	int index = mComboBox_Type->GetSelectIndex();
	if (index < 0)
		return "";

	return (const char *)mComboBox_Type->GetUserData(index);
}

void ParticleFXNewDialog::OnOK(const MGUI::ClickEvent * e)
{
	E_EndDialog(true);
	E_EndDialog.DetachAll();
	mDialog->SetVisible(false);
}

void ParticleFXNewDialog::OnCancel(const MGUI::ClickEvent * e)
{
	E_EndDialog(false);
	E_EndDialog.DetachAll();
	mDialog->SetVisible(false);
}

//
ParticleFXNewSetDialog::ParticleFXNewSetDialog()
{
	Root::ENUM_OBJECT_FACTORY(mNames, "PS_Set", false);

	for (int i = 0; i < mNames.Size(); ++i)
	{
		mComboBox_Type->Append(mNames[i].c_wstr(), (void *)mNames[i].c_str());
	}
}

ParticleFXNewSetDialog::~ParticleFXNewSetDialog()
{
}

//
ParticleFXNewEmitterDialog::ParticleFXNewEmitterDialog()
{
	Root::ENUM_OBJECT_FACTORY(mNames, "PS_Emitter", false);

	for (int i = 0; i < mNames.Size(); ++i)
	{
		mComboBox_Type->Append(mNames[i].c_wstr(), (void *)mNames[i].c_str());
	}
}

ParticleFXNewEmitterDialog::~ParticleFXNewEmitterDialog()
{
}

//
ParticleFXNewModifierDialog::ParticleFXNewModifierDialog()
{
	Root::ENUM_OBJECT_FACTORY(mNames, "PS_Modifier", false);

	for (int i = 0; i < mNames.Size(); ++i)
	{
		mComboBox_Type->Append(mNames[i].c_wstr(), (void *)mNames[i].c_str());
	}
}

ParticleFXNewModifierDialog::~ParticleFXNewModifierDialog()
{
}

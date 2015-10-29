#include "ProgressDialog.h"
#include "AllLookFeel.h"

ImplementSingleton(ProgressDialog);

ProgressDialog::ProgressDialog()
{
	mLayout = MGUI::Layout::Load("ProgressBar.layout", NULL);
	mLayout->SetOrder(100);

	mText = (MGUI::Label *)mLayout->GetChild("Text");

	MGUI::Widget * widget = new MGUI::Widget(AllLookFeel::Instance()->GetProgressBar(), mLayout->GetChild("ProgressBar"));
	widget->SetAlign(MGUI::eAlign::H_STRETCH | MGUI::eAlign::V_CENTER);

	mProgressBar = new MGUI::ProgressBarH(AllLookFeel::Instance()->GetProgressBar()->GetChild(0), widget);
	mProgressBar->SetAlign(MGUI::eAlign::STRETCH);

	mLayout->SetVisible(false);
}

ProgressDialog::~ProgressDialog()
{
	delete mLayout;
}

void ProgressDialog::Show(bool _show)
{
	mLayout->SetVisible(_show);

	mText->SetCaption(WSTR_(""));
	mProgressBar->SetPercentage(0);
}

void ProgressDialog::SetText(const String & text)
{
	mText->SetCaption(text.c_wstr());
}

void ProgressDialog::SetPercentage(int percentage)
{
	mProgressBar->SetPercentage(percentage);
}

int ProgressDialog::GetPercentage()
{
	return mProgressBar->GetPrecentage();
}
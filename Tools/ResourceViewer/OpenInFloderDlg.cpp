#include "OpenInFloderDlg.h"

OpenInFloderDlg::OpenInFloderDlg()
{
	const MGUI::LookFeel * lk = MGUI::LookFeelManager::Instance()->Find("MGUI::Widget", "AllWhite");

	mDialog = new MGUI::Layout(lk, NULL);
	mDialog->SetOrder(MGUI::eOrder::TOP - 10);
	mDialog->SetColor(Float4(0.15f, 0.15f, 0.15f, 0.5f));
	mDialog->SetAlign(MGUI::eAlign::STRETCH);
	mDialog->SetVisible(false);

	mLayout = MGUI::Layout::Load("OpenInFloder.layout", mDialog);
	mLayout->SetAlign(MGUI::eAlign::STRETCH);
	mLayout->SetInheritColor(false);
	mLayout->SetInheritAlpha(false);

	mButton = (MGUI::Button *)mLayout->GetChild("Button");

	mButton->E_Click += new cListener1<OpenInFloderDlg, const MGUI::ClickEvent *>(this, &OpenInFloderDlg::OnClick);
	mLayout->E_MouseUp += new cListener1<OpenInFloderDlg, const MGUI::MouseEvent *>(this, &OpenInFloderDlg::OnCancel);
}

OpenInFloderDlg::~OpenInFloderDlg()
{
	delete mDialog;
}

void OpenInFloderDlg::DoModal(float x, float y, const String & filename)
{
	mFilename = filename;

	float w = mButton->GetRect().w / 2;
	float h = mButton->GetRect().h / 2;

	mButton->SetRectPosition(x - w, y - h);

	mDialog->SetVisible(true);
}

void OpenInFloderDlg::OnClick(const MGUI::ClickEvent * e)
{
	String cmd = "/select, " + mFilename;
	ShellExecute(NULL, "open", "explorer.exe", cmd.c_str(), NULL, SW_SHOWDEFAULT);

	mDialog->SetVisible(false);
}

void OpenInFloderDlg::OnCancel(const MGUI::MouseEvent * e)
{
	mDialog->SetVisible(false);
}
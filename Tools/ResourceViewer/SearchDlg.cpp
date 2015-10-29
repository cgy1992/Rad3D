#include "SearchDlg.h"
#include "RenderViewer.h"

SearchDlg::SearchDlg()
{
	const MGUI::LookFeel * lk = MGUI::LookFeelManager::Instance()->Find("MGUI::Widget", "AllWhite");

	mDialog = new MGUI::Layout(lk, NULL);
	mDialog->SetOrder(MGUI::eOrder::TOP - 10);
	mDialog->SetColor(Float4(0.15f, 0.15f, 0.15f, 0.5f));
	mDialog->SetAlign(MGUI::eAlign::STRETCH);
	mDialog->SetVisible(false);

	mLayout = MGUI::Layout::Load("Search.layout", mDialog);
	mLayout->SetAlign(MGUI::eAlign::CENTER);
	mLayout->SetInheritColor(false);
	mLayout->SetInheritAlpha(false);

	MGUI::Button * bnOK = (MGUI::Button *)mLayout->GetChild("OK");
	MGUI::Button * bnCancel = (MGUI::Button *)mLayout->GetChild("Cancel");
	bnOK->E_Click += new cListener1<SearchDlg, const MGUI::ClickEvent *>(this, &SearchDlg::OnOK);
	bnCancel->E_Click += new cListener1<SearchDlg, const MGUI::ClickEvent *>(this, &SearchDlg::OnCancel);

	mEditBoxKey = (MGUI::EditBox *)mLayout->GetChild("Key");
}

SearchDlg::~SearchDlg()
{
	delete mDialog;
}

void SearchDlg::DoModal()
{
	mDialog->SetVisible(true);
}

void SearchDlg::OnOK(const MGUI::ClickEvent * e)
{
	String key;
	key.FromUnicode(mEditBoxKey->GetCaption().c_str());
	RenderViewer::Instance()->OnSearch(key);

	mDialog->SetVisible(false);
}

void SearchDlg::OnCancel(const MGUI::ClickEvent * e)
{
	mDialog->SetVisible(false);
}
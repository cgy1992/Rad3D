#include "MessageBoxDialog.h"

namespace Rad {

	ImplementSingleton(MessageBoxDialog);

	MessageBoxDialog::MessageBoxDialog()
	{
		mDialog = new MGUI::Layout(NULL, NULL);
		mDialog->SetOrder(MGUI::eOrder::TOP - 100);
		mDialog->SetColor(Float4(0.15f, 0.15f, 0.15f, 0.5f));
		mDialog->SetAlign(MGUI::eAlign::STRETCH);
		mDialog->SetVisible(false);

		mLayout = MGUI::Layout::Load("MessageBox.layout", mDialog);
		mLayout->SetAlign(MGUI::eAlign::CENTER);
		mLayout->SetInheritColor(false);
		mLayout->SetInheritAlpha(false);

		mCaption = new MGUI::TextBox(NULL, mLayout->GetChild("Text"));
		mCaption->SetAlign(MGUI::eAlign::CENTER);

		mOK = (MGUI::Button *)mLayout->GetChild("OK");
		mCancel = (MGUI::Button *)mLayout->GetChild("Cancel");
		mClose = (MGUI::Button *)mLayout->GetChild("Close");

		mOK->SetCaption(L"OK");
		mCancel->SetCaption(L"Cancel");

		mOK->E_MouseClick += new cListener1<MessageBoxDialog, const MGUI::MouseEvent *>(this, &MessageBoxDialog::OnOK);
		mCancel->E_MouseClick += new cListener1<MessageBoxDialog, const MGUI::MouseEvent *>(this, &MessageBoxDialog::OnCancel);
		mClose->E_MouseClick += new cListener1<MessageBoxDialog, const MGUI::MouseEvent *>(this, &MessageBoxDialog::OnCancel);

		mIconRect[MessageBoxType::Info] = MGUI::Rect(1, 1, 50, 50);
		mIconRect[MessageBoxType::Asking] = MGUI::Rect(50, 1, 50, 50);
		mIconRect[MessageBoxType::Error] = MGUI::Rect(100, 1, 50, 50);
		mIconRect[MessageBoxType::Warning] = MGUI::Rect(150, 1, 50, 50);

		mIcon = new MGUI::ImageBox(NULL, mLayout->GetChild("Icon"));
		mIcon->SetAlign(MGUI::eAlign::CENTER);
		mIcon->SetSkin("MessageBoxIcons.png");
		mIcon->SetRect(0, 0, mIconRect[MessageBoxType::Info].w, mIconRect[MessageBoxType::Info].h);
	}

	MessageBoxDialog::~MessageBoxDialog()
	{
		delete mDialog;
	}

	void MessageBoxDialog::DoModal(const String & caption, int type, void * userData)
	{
		mCaption->SetCaption(caption.c_wstr());
		mIcon->SetUVRect(MGUI::Helper::Instance()->MapUVRect(mIconRect[type], mIcon->GetSkin().c_ptr()));
		mUserData = userData;

		mDialog->SetVisible(true);
	}

	void MessageBoxDialog::OnOK(const MGUI::MouseEvent *)
	{
		E_EndDialog(true);
		E_EndDialog.DetachAll();
		mDialog->SetVisible(false);
	}

	void MessageBoxDialog::OnCancel(const MGUI::MouseEvent *)
	{
		E_EndDialog(false);
		E_EndDialog.DetachAll();
		mDialog->SetVisible(false);
	}

}


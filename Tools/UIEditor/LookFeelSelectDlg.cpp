#include "AllLookFeel.h"
#include "LookFeelSelectDlg.h"

LookFeelSelectDlg::LookFeelSelectDlg()
	: mWidget(NULL)
{
	mLayout = new MGUI::Layout(AllLookFeel::Instance()->GetWhite());
	mLayout->SetAlign(MGUI::eAlign::STRETCH);
	mLayout->SetOrder(MGUI::eOrder::TOP - 100);
	mLayout->SetColor(Float4(0.15f, 0.15f, 0.15f, 0.5f));

	// Dialog
	mDialog = new MGUI::Widget(AllLookFeel::Instance()->GetDialog(), mLayout);
	mDialog->SetAlign(MGUI::eAlign::CENTER);
	mDialog->SetRect(0, 0, 712, 573);
	mDialog->SetInheritAlpha(false);
	mDialog->SetInheritColor(false);

	mLabel_Caption = new MGUI::Label(AllLookFeel::Instance()->GetDialogCaption(), mDialog);
	mLabel_Caption->SetCaption(L"LookFeelSelector");
	mLabel_Caption->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);

	// Left
	float x_base = 0;
	float y_base = mLabel_Caption->GetRect().h + 5;

	mComboBox_WidgetType = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mDialog);
	mComboBox_WidgetType->SetRect(0, y_base, 200, 24);
	y_base += 24 + 2;

	mListBox_LookFeel = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), mDialog);
	mListBox_LookFeel->SetRect(0, y_base, 200, 500);
	y_base += 256;
	x_base += 200 + 5;

	// Right
	y_base = mLabel_Caption->GetRect().h + 5;

	mWidget_Priview = new MGUI::Widget(AllLookFeel::Instance()->GetPanel(), mDialog);
	mWidget_Priview->SetRect(x_base, y_base, 500, 500);
	y_base += 500 + 2;

	mLabel_NewName = new MGUI::Label(NULL, mDialog);
	mLabel_NewName->SetRect(x_base, y_base, 75, 24);
	mLabel_NewName->SetCaption(L"New Name:");

	mEditBox_NewName = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mDialog);
	mEditBox_NewName->SetRect(x_base + 75 + 5, y_base, 145, 24);

	x_base += 235;

	mButton_OK = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mDialog);
	mButton_OK->SetRect(x_base, y_base, 128, 24);
	mButton_OK->SetCaption(L"OK");
	x_base += 128 + 5;

	mButton_Cancel = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mDialog);
	mButton_Cancel->SetRect(x_base, y_base, 128, 24);
	mButton_Cancel->SetCaption(L"Cancel");
	y_base += 24 + 2;

	mLayout->SetVisible(false);


	// Init
	for (int i = 0; i < MGUI::LookFeelManager::Instance()->GetGroupSize(); ++i)
		mComboBox_WidgetType->Append(MGUI::LookFeelManager::Instance()->GetGroup(i)->TypeName().c_wstr());

	mComboBox_WidgetType->E_SelectChanged += new cListener2<LookFeelSelectDlg, const MGUI::Event *, int>(this, &LookFeelSelectDlg::OnWidgetTypeChanged_);
	mListBox_LookFeel->E_SelectChanged += new cListener2<LookFeelSelectDlg, const MGUI::Event *, int>(this, &LookFeelSelectDlg::OnLookFeelChanged_);
	mButton_OK->E_MouseClick += new cListener1<LookFeelSelectDlg, const MGUI::MouseEvent *>(this, &LookFeelSelectDlg::OnOK_);
	mButton_Cancel->E_MouseClick += new cListener1<LookFeelSelectDlg, const MGUI::MouseEvent *>(this, &LookFeelSelectDlg::OnCancel_);

	mComboBox_WidgetType->SetSelectIndex(0);
}

LookFeelSelectDlg::~LookFeelSelectDlg()
{
	delete mLayout;
}

void LookFeelSelectDlg::DoModal()
{
	MGUI::Event e(NULL);
	e.sender = mComboBox_WidgetType;
	OnWidgetTypeChanged_(&e, mComboBox_WidgetType->GetSelectIndex());

	mLayout->SetVisible(true);
}

int LookFeelSelectDlg::GetSelectType()
{
	return mComboBox_WidgetType->GetSelectIndex();
}

MGUI::LookFeel * LookFeelSelectDlg::GetSelectLookFeel()
{
	if (mListBox_LookFeel->GetSelectItem())
	{
		return (MGUI::LookFeel *)mListBox_LookFeel->GetSelectItem()->GetUserData();
	}

	return NULL;
}

String LookFeelSelectDlg::GetNewName()
{
	String newName;

	newName.FromUnicode(mEditBox_NewName->GetCaption().c_str());

	return newName;
}

void LookFeelSelectDlg::OnWidgetTypeChanged_(const MGUI::Event * e, int index)
{
	OnLookFeelChanged_(NULL, -1);

	mListBox_LookFeel->Clear();

	if (index == -1)
		return ;

	for (int i = 0; i < MGUI::LookFeelManager::Instance()->GetGroup(index)->Size(); ++i)
	{
		MGUI::LookFeel * lk = MGUI::LookFeelManager::Instance()->GetGroup(index)->At(i);

		mListBox_LookFeel->Append(lk->GetName().c_wstr(), lk);
	}
}

void LookFeelSelectDlg::OnLookFeelChanged_(const MGUI::Event * e, int index)
{
	safe_delete(mWidget);

	if (index == -1)
		return ;

	int type = mComboBox_WidgetType->GetSelectIndex();
	if (type == -1)
		return ;

	const char * typeName = MGUI::LookFeelManager::Instance()->GetGroup(type)->TypeName().c_str();
	MGUI::LookFeel * lookfeel = (MGUI::LookFeel *)mListBox_LookFeel->GetUserData(index);

	mWidget = MGUI::NEW_WIDGET<MGUI::Widget>(typeName, lookfeel, mWidget_Priview);

	if (mWidget)
	{
		mWidget->SetAlign(MGUI::eAlign::CENTER);
	}

	mEditBox_NewName->SetCaption(lookfeel->GetName().c_wstr());
}

void LookFeelSelectDlg::OnOK_(const MGUI::MouseEvent * e)
{
	E_EndDialog(true);
	mLayout->SetVisible(false);
}

void LookFeelSelectDlg::OnCancel_(const MGUI::MouseEvent * e)
{
	E_EndDialog(false);
	mLayout->SetVisible(false);
}
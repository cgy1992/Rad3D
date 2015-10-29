#include "AllLookFeel.h"
#include "WidgetCreateDlg.h"

WidgetCreateDlg::WidgetCreateDlg()
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
	mLabel_Caption->SetCaption(L"WidgetCreator");
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

	Map<FixedString32, MGUI::LookFeelGroup *> groups;
	MGUI::LookFeelManager::Instance()->GetGroupMap(groups);
	for (int i = 0; i < groups.Size(); ++i)
	{
		mComboBox_WidgetType->Append(groups[i].value->TypeName().c_wstr(), groups[i].value);
	}

	mComboBox_WidgetType->E_SelectChanged += new cListener2<WidgetCreateDlg, const MGUI::Event *, int>(this, &WidgetCreateDlg::OnWidgetTypeChanged_);
	mListBox_LookFeel->E_SelectChanged += new cListener2<WidgetCreateDlg, const MGUI::Event *, int>(this, &WidgetCreateDlg::OnLookFeelChanged_);
	mButton_OK->E_MouseClick += new cListener1<WidgetCreateDlg, const MGUI::MouseEvent *>(this, &WidgetCreateDlg::OnOK_);
	mButton_Cancel->E_MouseClick += new cListener1<WidgetCreateDlg, const MGUI::MouseEvent *>(this, &WidgetCreateDlg::OnCancel_);

	mComboBox_WidgetType->SetSelectIndex(0);
}

WidgetCreateDlg::~WidgetCreateDlg()
{
	delete mLayout;
}

void WidgetCreateDlg::DoModal(bool _root)
{
	if (_root)
	{
		for (int i = 0; i < mComboBox_WidgetType->GetCount(); ++i)
		{
			if (mComboBox_WidgetType->GetText(i) == WS("Widget"))
			{
				mComboBox_WidgetType->SetSelectIndex(i);
				break;
			}
		}
	}
	else
	{
		MGUI::Event e(NULL);
		e.sender = mComboBox_WidgetType;
		OnWidgetTypeChanged_(&e, mComboBox_WidgetType->GetSelectIndex());
	}

	mLayout->SetVisible(true);
}

FixedString32 WidgetCreateDlg::GetSelectType()
{
	int index = mComboBox_WidgetType->GetSelectIndex();
	if (index >= 0)
	{
		MGUI::LookFeelGroup * group = (MGUI::LookFeelGroup *)mComboBox_WidgetType->GetUserData(index);
		return group->TypeName();
	}
	else
	{
		return "";
	}
}

MGUI::LookFeel * WidgetCreateDlg::GetSelectLookFeel()
{
	if (mListBox_LookFeel->GetSelectItem())
	{
		return (MGUI::LookFeel *)mListBox_LookFeel->GetSelectItem()->GetUserData();
	}

	return NULL;
}

void WidgetCreateDlg::OnWidgetTypeChanged_(const MGUI::Event * e, int _index)
{
	OnLookFeelChanged_(NULL, -1);

	mListBox_LookFeel->Clear();

	if (_index == -1)
		return ;

	MGUI::LookFeelGroup * group = (MGUI::LookFeelGroup *)mComboBox_WidgetType->GetUserData(_index);

	Map<FixedString32, MGUI::LookFeel *> lookfeels;
	group->GetLookFeelMap(lookfeels);

	for (int i = 0; i < lookfeels.Size(); ++i)
	{
		MGUI::LookFeel * lk = lookfeels[i].value;

		mListBox_LookFeel->Append(lk->GetName().c_wstr(), lk);
	}
}

void WidgetCreateDlg::OnLookFeelChanged_(const MGUI::Event * e, int _index)
{
	safe_delete(mWidget);

	if (_index == -1)
		return ;

	int type = mComboBox_WidgetType->GetSelectIndex();
	if (type == -1)
		return ;

	MGUI::LookFeelGroup * group = (MGUI::LookFeelGroup *)mComboBox_WidgetType->GetUserData(type);

	const char * typeName = group->TypeName().c_str();
	MGUI::LookFeel * lookfeel = (MGUI::LookFeel *)mListBox_LookFeel->GetUserData(_index);

	mWidget = MGUI::NEW_WIDGET<MGUI::Widget>(typeName, lookfeel, mWidget_Priview);
	if (mWidget)
	{
		mWidget->SetAlign(MGUI::eAlign::CENTER);
	}
}

void WidgetCreateDlg::OnOK_(const MGUI::MouseEvent * e)
{
	E_EndDialog(true);
	mLayout->SetVisible(false);
}

void WidgetCreateDlg::OnCancel_(const MGUI::MouseEvent * e)
{
	E_EndDialog(false);
	mLayout->SetVisible(false);
}
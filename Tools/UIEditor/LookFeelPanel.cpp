#include "LookFeelPanel.h"
#include "LookFeelEditor.h"
#include "AllLookFeel.h"


LookFeelPanel::LookFeelPanel()
{
	mLayout = new MGUI::Layout(AllLookFeel::Instance()->GetPanel());

	mComboBox_WidgetType = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
	mComboBox_WidgetType->SetRect(0, 0, 200, 24);
	mComboBox_WidgetType->SetAlign(MGUI::eAlign::H_STRETCH);

	Map<FixedString32, MGUI::LookFeelGroup *> groups;
	MGUI::LookFeelManager::Instance()->GetGroupMap(groups);
	for (int i = 0; i < groups.Size(); ++i)
	{
		mComboBox_WidgetType->Append(groups[i].value->TypeName().c_wstr(), groups[i].value);
	}

	mListBox_LookFeel = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), mLayout);
	mListBox_LookFeel->SetAlign(MGUI::eAlign::H_STRETCH);
	mListBox_LookFeel->SetRect(0, 24, 0, 256);
		
	MGUI::Button * bnCreate = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	bnCreate->SetAlign(MGUI::eAlign::H_STRETCH);
	bnCreate->SetRect(0, 24 + 256 + 5, 0, 30);
	bnCreate->SetCaption(L"Create");

	MGUI::Button * bnDelete = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	bnDelete->SetAlign(MGUI::eAlign::H_STRETCH);
	bnDelete->SetRect(0, 24 + 256 + 5 + 30 + 5, 0, 30);
	bnDelete->SetCaption(L"Delete");

	MGUI::Button * bnRename = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	bnRename->SetAlign(MGUI::eAlign::H_STRETCH);
	bnRename->SetRect(0, 24 + 256 + 5 + 30 + 5 + 30 + 5, 0, 30);
	bnRename->SetCaption(L"Rename");

	mComboBox_WidgetType->E_SelectChanged += new cListener2<LookFeelPanel, const MGUI::Event *, int>(this, &LookFeelPanel::OnWidgetTypeChanged_);
	mListBox_LookFeel->E_SelectChanged += new cListener2<LookFeelPanel, const MGUI::Event *, int>(this, &LookFeelPanel::OnLookFeelChanged_);

	bnCreate->E_MouseClick += new cListener1<LookFeelPanel, const MGUI::MouseEvent *>(this, &LookFeelPanel::OnCreate_);
	bnDelete->E_MouseClick += new cListener1<LookFeelPanel, const MGUI::MouseEvent *>(this, &LookFeelPanel::OnDelete_);

	LookFeelEditor::Instance()->E_NameChanged += new cListener0<LookFeelPanel>(this, &LookFeelPanel::OnNameChanged_);
	LookFeelEditor::Instance()->E_OpenLookFeel += new cListener0<LookFeelPanel>(this, &LookFeelPanel::OnOpenLookFeel_);

	mComboBox_WidgetType->SetSelectIndex(0);
}

LookFeelPanel::~LookFeelPanel()
{
	delete mLayout;
}

void LookFeelPanel::Layout()
{
	MGUI::Rect rect;

	rect.x = 0;
	rect.y = 32;
	rect.w = 256;
	rect.h = MGUI::Engine::Instance()->GetRect().h - 32 - 32;

	mLayout->SetRect(rect);
}

int LookFeelPanel::GetEditWidgetType()
{
	return mComboBox_WidgetType->GetSelectIndex();
}

void LookFeelPanel::OnWidgetTypeChanged_(const MGUI::Event * e, int _index)
{
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

void LookFeelPanel::OnLookFeelChanged_(const MGUI::Event * e, int _index)
{
	if (_index == -1)
	{
		LookFeelEditor::Instance()->SetEditLookFeel(NULL);
		return ;
	}

	MGUI::LookFeel * lookfeel = (MGUI::LookFeel *)mListBox_LookFeel->GetSelectItem()->GetUserData();

	LookFeelEditor::Instance()->SetEditLookFeel(lookfeel);
}

void LookFeelPanel::OnNameChanged_()
{
	MGUI::LookFeel * lookfeel = LookFeelEditor::Instance()->GetEditLookFeel();

	if (lookfeel == NULL)
		return ;

	for (int i = 0; i < mListBox_LookFeel->GetCount(); ++i)
	{
		if (mListBox_LookFeel->GetUserData(i) == lookfeel)
		{
			mListBox_LookFeel->GetItem(i)->SetText(lookfeel->GetName().c_wstr());
			break;
		}
	}
}

void LookFeelPanel::OnOpenLookFeel_()
{
	mListBox_LookFeel->Clear();

	MGUI::Event e(NULL);
	e.sender = mComboBox_WidgetType;
	OnWidgetTypeChanged_(&e, mComboBox_WidgetType->GetSelectIndex());
}


void LookFeelPanel::OnCreate_(const MGUI::MouseEvent * e)
{
	int type = mComboBox_WidgetType->GetSelectIndex();
	if (type == -1)
		return ;

	MGUI::LookFeelGroup * group = (MGUI::LookFeelGroup *)mComboBox_WidgetType->GetUserData(type);
	int index = 0;
	FixedString32 name;

	do {
		const char * typeName = group->TypeName().c_str();

		name.Format("%s%d", typeName, index++);

	} while (group->Find(name) != NULL);

	MGUI::LookFeel * lookfeel = new MGUI::LookFeel(name);

	lookfeel->SetSkin(LookFeelEditor::Instance()->GetEditSkinName());
		
	group->Append(lookfeel);

	mListBox_LookFeel->Append(lookfeel->GetName().c_wstr(), lookfeel);
}

void LookFeelPanel::OnDelete_(const MGUI::MouseEvent * e)
{
	int type = mComboBox_WidgetType->GetSelectIndex();
	if (type == -1)
		return ;

	int index = mListBox_LookFeel->GetSelectIndex();
	if (index == -1)
		return ;

	MGUI::LookFeelGroup * group = (MGUI::LookFeelGroup *)mComboBox_WidgetType->GetUserData(type);
	MGUI::LookFeel * lookfeel = (MGUI::LookFeel *)mListBox_LookFeel->GetUserData(index);

	group->Remove(lookfeel);

	mListBox_LookFeel->Remove(index);
}



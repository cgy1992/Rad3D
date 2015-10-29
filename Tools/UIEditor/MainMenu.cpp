#include "MainMenu.h"
#include "MainFrame.h"
#include "AllLookFeel.h"
#include "LookFeelEditor.h"
#include "LayoutEditor.h"

MainMenu::MainMenu()
{
	mLayout = new MGUI::Layout(AllLookFeel::Instance()->GetMenu());

	mBn_Open = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	mBn_Save = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);

	mBn_Open->SetCaption(L"Open");
	mBn_Save->SetCaption(L"Save");

	mBn_Open->SetRect(0, 0, 50, 24);
	mBn_Save->SetRect(55, 0, 50, 24);

	mBn_Open->SetAlign(MGUI::eAlign::V_CENTER);
	mBn_Save->SetAlign(MGUI::eAlign::V_CENTER);

	mBn_Open->E_MouseClick += new cListener1<MainMenu, const MGUI::MouseEvent *>(this, &MainMenu::OnOpen_);
	mBn_Save->E_MouseClick += new cListener1<MainMenu, const MGUI::MouseEvent *>(this, &MainMenu::OnSave_);

	mLayout_EditMode = new MGUI::Layout(NULL);
	mLayout_EditMode->SetAlign(MGUI::eAlign::STRETCH);
	mLayout_EditMode->SetPickFlag(MGUI::ePickFlag::CHILD);
	mLayout_EditMode->SetOrder(MGUI::eOrder::TEXT_CURSOR + 100);

	mCb_EditMode = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout_EditMode);
	mCb_EditMode->SetRect(256, 5, 256, 24);

	mCb_EditMode->Append(L"LookFeel");
	mCb_EditMode->Append(L"Layout");
	mCb_EditMode->SetSelectIndex(0);

	mCb_EditMode->E_SelectChanged += new cListener2<MainMenu, const MGUI::Event *, int>(this, &MainMenu::OnEditModeChanged_);
}

MainMenu::~MainMenu()
{
	delete mLayout;
	delete mLayout_EditMode;
}

void MainMenu::Layout()
{
	MGUI::Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = MGUI::Engine::Instance()->GetRect().w;
	rect.h = 32;

	mLayout->SetRect(rect);
}

void MainMenu::OnOpen_(const MGUI::MouseEvent * e)
{
	int index = mCb_EditMode->GetSelectIndex();

	if (index == 0) // LookFeel
	{
		LookFeelEditor::Instance()->Open();
	}
	else if (index == 1) // Layout
	{
		LayoutEditor::Instance()->Open();
	}
}

void MainMenu::OnSave_(const MGUI::MouseEvent * e)
{
	int index = mCb_EditMode->GetSelectIndex();

	if (index == 0) // LookFeel
	{
		LookFeelEditor::Instance()->Save();
	}
	else if (index == 1) // Layout
	{
		LayoutEditor::Instance()->Save();
	}
}

void MainMenu::OnEditModeChanged_(const MGUI::Event * e, int index)
{
	if (index == 0) // LookFeel
	{
		MainFrame::Instance()->SetEditorMode(EM_Lookfeel);
	}
	else // Layout
	{
		MainFrame::Instance()->SetEditorMode(EM_Layout);
	}
}


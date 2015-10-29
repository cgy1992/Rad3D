#include "MainMenu.h"
#include "FloderPanel.h"
#include "RenderViewer.h"
#include "ToolUtil.h"

MainMenu::MainMenu()
{
	mLayout = MGUI::Layout::Load("MainMenu.layout", NULL);

	MGUI::Button * bnOpen = (MGUI::Button *)mLayout->GetChild("Open");
	bnOpen->E_Click += new cListener1<MainMenu, const MGUI::ClickEvent *>(this, &MainMenu::OnOpen);

	MGUI::Button * bnSearch = (MGUI::Button *)mLayout->GetChild("Search");
	bnSearch->E_Click += new cListener1<MainMenu, const MGUI::ClickEvent *>(this, &MainMenu::OnSearch);

	MGUI::Button * bnForward = (MGUI::Button *)mLayout->GetChild("Buttons")->GetChild("Forward");
	MGUI::Button * bnBack = (MGUI::Button *)mLayout->GetChild("Buttons")->GetChild("Back");

	bnForward->E_Click += new cListener1<MainMenu, const MGUI::ClickEvent *>(this, &MainMenu::OnNext);
	bnBack->E_Click += new cListener1<MainMenu, const MGUI::ClickEvent *>(this, &MainMenu::OnPrev);

	MGUI::InputManager::Instance()->E_KeyUp += new cListener1<MainMenu, const MGUI::KeyEvent *>(this, &MainMenu::OnKeydown);

	mSearchDlg = new SearchDlg;
}

MainMenu::~MainMenu()
{
	delete mSearchDlg;

	safe_delete (mLayout);
}

void MainMenu::Layout()
{
}

void MainMenu::Update()
{
	MGUI::Button * bnForward = (MGUI::Button *)mLayout->GetChild("Buttons")->GetChild("Forward");
	MGUI::Button * bnBack = (MGUI::Button *)mLayout->GetChild("Buttons")->GetChild("Back");

	bnForward->SetEnable(RenderViewer::Instance()->CanNext());
	bnBack->SetEnable(RenderViewer::Instance()->CanPrev());
}

void MainMenu::OnOpen(const MGUI::ClickEvent * e)
{
	String floder = Util_GetOpenFloder();
	if (floder != "")
	{
		FloderPanel::Instance()->SetFloder(floder);
	}
}

void MainMenu::OnSearch(const MGUI::ClickEvent * e)
{
	mSearchDlg->DoModal();
}

void MainMenu::OnNext(const MGUI::ClickEvent * e)
{
	RenderViewer::Instance()->OnNextGroup();
}

void MainMenu::OnPrev(const MGUI::ClickEvent * e)
{
	RenderViewer::Instance()->OnPrevGroup();
}

void MainMenu::OnKeydown(const MGUI::KeyEvent * e)
{
	if (e->key == InputCode::KC_LEFTARROW)
	{
		RenderViewer::Instance()->OnPrevGroup();
	}
	else if (e->key == InputCode::KC_RIGHTARROW)
	{
		RenderViewer::Instance()->OnNextGroup();
	}
}

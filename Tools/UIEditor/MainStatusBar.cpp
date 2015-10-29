#include "MainStatusBar.h"
#include "MainFrame.h"
#include "AllLookFeel.h"

ImplementSingleton(MainStatusBar);

MainStatusBar::MainStatusBar()
{
	mLayout = new MGUI::Layout(AllLookFeel::Instance()->GetPanel());
	mLabel_Text = new MGUI::Label(NULL, mLayout);
	mLabel_Text->SetAlign(MGUI::eAlign::STRETCH);

	MainFrame::Instance()->E_EditorModeChanged += new cListener1<MainStatusBar, int>(this, &MainStatusBar::OnEditorModeChanged_);
}

MainStatusBar::~MainStatusBar()
{
	delete mLayout;
}

void MainStatusBar::Layout()
{
	MGUI::Rect rect;

	rect.x = 0;
	rect.y = MGUI::Engine::Instance()->GetRect().h - 32;
	rect.w = MGUI::Engine::Instance()->GetRect().w;
	rect.h = 32;

	mLayout->SetRect(rect);
}

void MainStatusBar::SetText(const String & text)
{
	mLabel_Text->SetCaption(text.c_wstr());
}

void MainStatusBar::OnEditorModeChanged_(int mode)
{
	if (mode == EM_Layout)
		SetText(LayoutEditor::Instance()->GetLayoutFile());
	else if (mode == EM_Lookfeel)
		SetText(LookFeelEditor::Instance()->GetLookFeelFile());
}
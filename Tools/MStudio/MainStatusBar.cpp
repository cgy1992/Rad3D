#include "MainStatusBar.h"
#include "MainFrame.h"
#include "Editor.h"

MainStatusBar::MainStatusBar()
{
	mLayout = new MGUI::Layout(AllLookFeel::Instance()->GetPanel(), NULL);

	mTextBox = new MGUI::TextBox(NULL, mLayout);
	mTextBox->SetAlign(MGUI::eAlign::V_CENTER);

	mLayout->E_Update += new cListener1<MainStatusBar, const MGUI::Event *>(this, &MainStatusBar::OnUpdate);
}

MainStatusBar::~MainStatusBar()
{
	delete mLayout;
}

void MainStatusBar::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	mLayout->SetRect(0, rect.h - D_MAINSTATUSBAR_H, rect.w, D_MAINSTATUSBAR_H);
}

void MainStatusBar::OnUpdate(const MGUI::Event *)
{
	int batchCount = RenderSystem::Instance()->_getBatchCount();
	int primCount = RenderSystem::Instance()->_getPrimitiveCount();
	int fps = (int)Root::Instance()->GetFPS();

	String caption;
	caption.Format("%s. TC: %d, BC: %d, FPS: %d.",
		Editor::Instance()->GetSceneFilename().c_str(), 
		primCount, batchCount, fps);

	mTextBox->SetCaption(caption.c_wstr());
}

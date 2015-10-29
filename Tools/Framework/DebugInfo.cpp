#include "DebugInfo.h"

ImplementSingleton(DebugInfo);

DebugInfo::DebugInfo()
{
	mLayout = new MGUI::Layout(NULL);
	mLayout->SetRect(0, 0, 256, 128);

	mLayout->SetPickFlag(MGUI::ePickFlag::NONE);

#ifdef IN_EDITOR
	mLayout->SetAlign(MGUI::eAlign::LEFT | MGUI::eAlign::BOTTOM);
#else
	mLayout->SetAlign(MGUI::eAlign::LEFT | MGUI::eAlign::TOP);
#endif

	mLogo = new MGUI::ImageBox(NULL, mLayout);
	mLogo->SetAlign(MGUI::eAlign::LEFT | MGUI::eAlign::TOP);
	mLogo->SetSkin("Logo.png");
	mLogo->SetRect(0, 0, 128, 128);

	mBatch = new MGUI::TextBox(NULL, mLayout);
	mBatch->SetRect(0, 0, 256, 27);

	mPrimitive = new MGUI::TextBox(NULL, mLayout);
	mPrimitive->SetRect(0, 27 + 10, 256, 27);

	mFPS = new MGUI::TextBox(NULL, mLayout);
	mFPS->SetRect(0, 27 + 10 + 27 + 10, 256, 27);

	Show(false);
}

DebugInfo::~DebugInfo()
{
	delete mLayout;
}

void DebugInfo::Update()
{
	if (!mLayout->IsVisible())
		return ;

	int batchCount = RenderSystem::Instance()->_getBatchCount();
	int primCount = RenderSystem::Instance()->_getPrimitiveCount();
	int fps = (int)Root::Instance()->GetFPS();

	String text_batch, text_prim, text_fps;

	text_batch.Format("BC: %d", batchCount);
	text_prim.Format("TC: %d", primCount);
	text_fps.Format("FPS: %d", fps);

	mBatch->SetCaption(text_batch.c_wstr());
	mPrimitive->SetCaption(text_prim.c_wstr());
	mFPS->SetCaption(text_fps.c_wstr());
}

void DebugInfo::Show(bool _show)
{
	mLayout->SetVisible(_show);
}
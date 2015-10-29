#include "ParticleFXUserPanel.h"
#include "ParticleFXPropertyPanel.h"
#include "MainFrame.h"

ImplementSingleton(ParticleFXUserPanel);

ParticleFXUserPanel::ParticleFXUserPanel()
{
	mLayout = new MGUI::Layout(NULL);
	mLayout->SetPickFlag(MGUI::ePickFlag::CHILD);
}

ParticleFXUserPanel::~ParticleFXUserPanel()
{
	delete mLayout;
}

void ParticleFXUserPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float x = ParticleFXPropertyPanel::Instance()->GetLayout()->GetAbsRect().x + ParticleFXPropertyPanel::Instance()->GetLayout()->GetAbsRect().w;
	float y = rect.h - D_MAINSTATUSBAR_H - 200;
	float w = rect.w - x;;

	mLayout->SetRect(x, y, w, 200);
}

void ParticleFXUserPanel::Show()
{
	mLayout->SetVisible(true);
}

void ParticleFXUserPanel::Hide()
{
	mLayout->SetVisible(false);
}

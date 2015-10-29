#include "ComponentUserPanel.h"
#include "ComponentPropertyPanel.h"
#include "MainFrame.h"

ImplementSingleton(ComponentUserPanel)

ComponentUserPanel::ComponentUserPanel()
{
	mLayout = new MGUI::Layout(NULL);
	mLayout->SetPickFlag(MGUI::ePickFlag::CHILD);
}

ComponentUserPanel::~ComponentUserPanel()
{
	delete mLayout;
}

void ComponentUserPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float x = ComponentPropertyPanel::Instance()->GetLayout()->GetAbsRect().x + ComponentPropertyPanel::Instance()->GetLayout()->GetAbsRect().w;
	float y = rect.h - D_MAINSTATUSBAR_H - 200;
	float w = rect.w - x;;

	mLayout->SetRect(x, y, w, 200);
}

void ComponentUserPanel::Show()
{
	mLayout->SetVisible(true);
}

void ComponentUserPanel::Hide()
{
	mLayout->SetVisible(false);
}

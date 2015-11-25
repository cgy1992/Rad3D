#include "EntityPropertyPanel.h"
#include "MainFrame.h"
#include "EntityPanel.h"
#include "Editor.h"

ImplementSingleton(EntityPropertyPanel);

EntityPropertyPanel::EntityPropertyPanel()
{
	mLayout = MGUI::Layout::Load("EntityPropertyPanel.layout", NULL);
	d_assert(mLayout != NULL);

	mLayout->SetVisible(false);

	mTopWidget = mLayout->GetChild("Caption");
	mTopWidget->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mTopWidget->SetDockable(true);

	MGUI::TextBox * pCaption = new MGUI::TextBox(NULL, mTopWidget);
	pCaption->SetAlign(MGUI::eAlign::CENTER);
	pCaption->SetCaption(L"Property");

	mPropertyGrid = new PropertyGrid(mLayout);
	mPropertyGrid->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGrid->SetDockable(true);

	Editor::Instance()->E_NodeSelect += new cListener0<EntityPropertyPanel>(this, &EntityPropertyPanel::OnNodeSelectedChanged);
}

EntityPropertyPanel::~EntityPropertyPanel()
{
	delete mPropertyGrid;
	safe_delete (mLayout);
}

void EntityPropertyPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(EntityPanel::Instance()->GetLayout()->GetAbsRect().w, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void EntityPropertyPanel::Show()
{
	mLayout->SetVisible(true);
}

void EntityPropertyPanel::Hide()
{
	mLayout->SetVisible(false);
}

void EntityPropertyPanel::OnNodeSelectedChanged()
{
	Node * node = Editor::Instance()->GetSelectNode();

	mPropertyGrid->Attach(node);
}

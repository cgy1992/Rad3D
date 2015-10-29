#include "EntityModule.h"
#include "MainRenderView.h"
#include "Editor.h"

ImplementSingleton(EntityModule);

EntityModule::EntityModule()
	: EditorModule("Entity")
{
	mEntityPanel = new EntityPanel;
	mEntityPropertyPanel = new EntityPropertyPanel;
	mEntityBound = new EntityBound;
	mGizmoBar = new GizmoBar;
}

EntityModule::~EntityModule()
{
	delete mGizmoBar;
	delete mEntityBound;
	delete mEntityPanel;
	delete mEntityPropertyPanel;
}

void EntityModule::Layout()
{
	mEntityPanel->Layout();
	mEntityPropertyPanel->Layout();

	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();
	float x = EntityPropertyPanel::Instance()->GetLayout()->GetAbsRect().x + EntityPropertyPanel::Instance()->GetLayout()->GetAbsRect().w;
	float h = mGizmoBar->GetLayout()->GetRect().h;

	mGizmoBar->GetLayout()->SetRect(x, D_MAINMENU_H, rect.w - x, h);
}

void EntityModule::Show()
{
	mEntityPanel->Show();
	mEntityPropertyPanel->Show();
	mGizmoBar->Show();

	Editor::Instance()->SetFlags(FLAG_DRAG_ABLE | FLAG_PICK_ABLE);
}

void EntityModule::Hide()
{
	mGizmoBar->Hide();
	mEntityPanel->Hide();
	mEntityPropertyPanel->Hide();
}
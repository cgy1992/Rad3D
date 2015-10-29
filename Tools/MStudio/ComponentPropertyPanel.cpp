#include "ComponentPropertyPanel.h"
#include "MainFrame.h"
#include "Editor.h"
#include "ComponentPanel.h"
#include "ComponentUserPanel.h"
#include "PropertyField_KeyFrame.h"

class Component_PropertyFieldCreator : public IPropertyGrid_UserCreator
{
public:
	Component_PropertyFieldCreator() {}
	virtual ~Component_PropertyFieldCreator() {}

	PropertyField * CreatePropertyField(PropertyGroup * group, IObject * obj, const Property * prop)
	{
		if (strcmp(prop->editorName, "PT_KeyController") == 0)
		{
			return new PropertyFiled_KeyFrame(group, obj, prop, ComponentUserPanel::Instance()->GetLayout());
		}

		return NULL;
	}
};

Component_PropertyFieldCreator gComponentPropertyFieldCreator;

ImplementSingleton(ComponentPropertyPanel)

ComponentPropertyPanel::ComponentPropertyPanel()
{
	mLayout = MGUI::Layout::Load("ComponentPropertyPanel.layout", NULL);
	d_assert(mLayout != NULL);

	mLayout->SetVisible(false);

	mTopWidget = mLayout->GetChild("Top");

	mPropertyGrid = new PropertyGrid(mLayout);
	mPropertyGrid->SetAlign(MGUI::eAlign::H_STRETCH);
	mPropertyGrid->SetVisible(true);
}

ComponentPropertyPanel::~ComponentPropertyPanel()
{
	delete mPropertyGrid;
	safe_delete (mLayout);
}

void ComponentPropertyPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(ComponentPanel::Instance()->GetLayout()->GetRect().w, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);

	MGUI::Rect clRect = mLayout->GetClient();
	MGUI::Rect tlRect = mTopWidget->GetRect();

	mPropertyGrid->SetRect(0, tlRect.h, clRect.w, clRect.h - tlRect.h);
}

void ComponentPropertyPanel::Show()
{
	mLayout->SetVisible(true);
}

void ComponentPropertyPanel::Hide()
{
	mLayout->SetVisible(false);
}

void ComponentPropertyPanel::Attach(IObject * p)
{
	mPropertyGrid->Attach(p, &gComponentPropertyFieldCreator);
}


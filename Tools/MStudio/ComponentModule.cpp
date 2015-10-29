#include "ComponentModule.h"

ComponentModule::ComponentModule()
	: EditorModule("Component")
{
	mPanel = new ComponentPanel;
	mPropertyPanel = new ComponentPropertyPanel;
	mUserPanel = new ComponentUserPanel;
}

ComponentModule::~ComponentModule()
{
	delete mPropertyPanel;
	delete mUserPanel;
	delete mPanel;
}

void ComponentModule::Layout()
{
	mPanel->Layout();
	mPropertyPanel->Layout();
	mUserPanel->Layout();
}

void ComponentModule::Show()
{
	mPanel->Show();
	mPropertyPanel->Show();
	mUserPanel->Show();
}

void ComponentModule::Hide()
{
	mPanel->Hide();
	mPropertyPanel->Hide();
	mUserPanel->Hide();
}

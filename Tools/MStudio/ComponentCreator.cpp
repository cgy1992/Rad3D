#include "ComponentCreator.h"
#include "ComponentPanel.h"
#include "Editor.h"

ComponentCreator::ComponentCreator()
{
	mDialog = new MGUI::Layout(NULL, NULL);
	mDialog->SetOrder(MGUI::eOrder::TOP - 10);
	mDialog->SetColor(Float4(0.15f, 0.15f, 0.15f, 0.5f));
	mDialog->SetAlign(MGUI::eAlign::STRETCH);
	mDialog->SetVisible(false);

	mLayout = MGUI::Layout::Load("ComponentCreator.layout", mDialog);
	mLayout->SetAlign(MGUI::eAlign::CENTER);
	mLayout->SetInheritColor(false);
	mLayout->SetInheritAlpha(false);

	MGUI::Button * bnOK = (MGUI::Button *)mLayout->GetChild("OK");
	MGUI::Button * bnCancel = (MGUI::Button *)mLayout->GetChild("Cancel");

	bnOK->E_Click += new cListener1<ComponentCreator, const MGUI::ClickEvent *>(this, &ComponentCreator::OnOK);
	bnCancel->E_Click += new cListener1<ComponentCreator, const MGUI::ClickEvent *>(this, &ComponentCreator::OnCancel);

	mListBox = (MGUI::ListBox *)mLayout->GetChild("List");

	Array<ComponentFactory *> & factroys = EditorFactory::Instance()->GetComponentFactorys(); 
	for (int i = 0; i < factroys.Size(); ++i)
	{
		ComponentFactory * factory = factroys[i];

		mListBox->Append(factory->GetTypeName().c_wstr(), factory);
	}
}

ComponentCreator::~ComponentCreator()
{
	delete mDialog;
}

void ComponentCreator::DoModal()
{
	mDialog->SetVisible(true);
}

void ComponentCreator::OnOK(const MGUI::ClickEvent *)
{
	int index = mListBox->GetSelectIndex();

	if (index >= 0)
	{
		ComponentFactory * factory = (ComponentFactory *)mListBox->GetUserData(index);

		ComponentPanel::Instance()->OnCreateComponent(factory);
	}

	mDialog->SetVisible(false);
}

void ComponentCreator::OnCancel(const MGUI::ClickEvent *)
{
	mDialog->SetVisible(false);
}
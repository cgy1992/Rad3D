#include "ComponentPanel.h"
#include "ComponentPropertyPanel.h"
#include "MainFrame.h"
#include "Editor.h"

ImplementSingleton(ComponentPanel);

ComponentPanel::ComponentPanel()
{
	mLayout = MGUI::Layout::Load("ComponentPanel.layout", NULL);
	d_assert(mLayout != NULL);

	MGUI::Button * bnAdd = (MGUI::Button *)mLayout->GetChild("Add");
	MGUI::Button * bnRemove = (MGUI::Button *)mLayout->GetChild("Remove");
	d_assert (bnAdd && bnRemove);

	bnAdd->E_Click += new cListener1<ComponentPanel, const MGUI::ClickEvent *>(this, &ComponentPanel::OnAdd);
	bnRemove->E_Click += new cListener1<ComponentPanel, const MGUI::ClickEvent *>(this, &ComponentPanel::OnRemove);

	mListBox = (MGUI::ListBox *)mLayout->GetChild("List");
	d_assert (mListBox);

	mListBox->E_SelectChanged += new cListener2<ComponentPanel, const MGUI::Event *, int>(this, &ComponentPanel::OnComponentSelChanged);

	mCreator = new ComponentCreator;

	Editor::Instance()->E_NodeSelect += new cListener0<ComponentPanel>(this, &ComponentPanel::OnNodeChanged);

	Hide();
}

ComponentPanel::~ComponentPanel()
{
	delete mCreator;

	safe_delete (mLayout);
}

void ComponentPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void ComponentPanel::Show()
{
	mLayout->SetVisible(true);
}

void ComponentPanel::Hide()
{
	mLayout->SetVisible(false);
}

void ComponentPanel::OnCreateComponent(ComponentFactory * factory)
{
	Node * pSelectNode = Editor::Instance()->GetSelectNode();
	if (pSelectNode != NULL)
	{
		IComponent * cp = factory->NewObject();

		pSelectNode->AddComponent(cp);

		mListBox->Append(String(cp->GetRTTI()->Name()).c_wstr(), cp);
	}
}

void ComponentPanel::OnAdd(const MGUI::ClickEvent * e)
{
	Node * pSelectNode = Editor::Instance()->GetSelectNode();
	if (pSelectNode != NULL)
	{
		mCreator->DoModal();
	}
}

void ComponentPanel::OnRemove(const MGUI::ClickEvent * e)
{
	int index = mListBox->GetSelectIndex();
	if (index < 0)
		return ;

	IComponent * cp = (IComponent *)mListBox->GetUserData(index);

	Node * pSelectNode = Editor::Instance()->GetSelectNode();
	if (pSelectNode != NULL && cp->GetOwner() == pSelectNode)
	{
		pSelectNode->RemoveComponent(cp);

		mListBox->Remove(index);
	}
}

void ComponentPanel::OnNodeChanged()
{
	mListBox->Clear();
	ComponentPropertyPanel::Instance()->Attach(NULL);

	Node * pSelectNode = Editor::Instance()->GetSelectNode();
	if (pSelectNode != NULL)
	{
		IComponent * cp = pSelectNode->FirstComponent();
		while (cp)
		{
			mListBox->Append(String(cp->GetRTTI()->Name()).c_wstr(), cp);

			cp = pSelectNode->NextComponent(cp);
		}
	}
}

void ComponentPanel::OnComponentSelChanged(const MGUI::Event * e, int index)
{
	IComponent * cp = NULL;

	if (index >= 0)
	{
		cp = (IComponent *)mListBox->GetUserData(index);
	}

	ComponentPropertyPanel::Instance()->Attach(cp);
}

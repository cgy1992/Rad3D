#include "WorldModule.h"
#include "MainFrame.h"
#include "Editor.h"

ImplementSingleton(WorldModule);

WorldModule::WorldModule()
	: EditorModule("World")
{
	mLayout = MGUI::Layout::Load("Panel.layout", NULL);
	mLayout->SetVisible(false);

	mListBox = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), mLayout);
	mListBox->SetRect(0, 0, 256, 256);
	mListBox->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mListBox->SetDockable(true);
	mListBox->E_SelectChanged += new cListener2<WorldModule, const MGUI::Event *, int>(this, &WorldModule::OnSelectChanged);

	MGUI::Button * bnAdd = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	bnAdd->SetCaption(WS("Add"));
	bnAdd->SetRect(0, 0, 256, 24);
	bnAdd->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	bnAdd->SetDockable(true);
	bnAdd->E_Click += new cListener1<WorldModule, const MGUI::ClickEvent *>(this, &WorldModule::OnAdd);

	MGUI::Button * bnRemove = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	bnRemove->SetCaption(WS("Remove"));
	bnRemove->SetRect(0, 0, 256, 24);
	bnRemove->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	bnRemove->SetDockable(true);
	bnRemove->E_Click += new cListener1<WorldModule, const MGUI::ClickEvent *>(this, &WorldModule::OnRemove);

	mPropertyGrid = new PropertyGrid(mLayout);
	mPropertyGrid->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGrid->SetDockable(true);

	mSelectLayout = new MGUI::Layout(NULL, NULL);
	mSelectLayout->SetAlign(MGUI::eAlign::STRETCH);
	mSelectLayout->SetOrder(MGUI::eOrder::TOP - 100);
	mSelectLayout->SetVisible(false);

	MGUI::Layout * layout = MGUI::Layout::Load("ComponentSelect.layout", mSelectLayout);
	layout->SetAlign(MGUI::eAlign::CENTER);

	MGUI::Button * bnOK = layout->GetChildT<MGUI::Button>("OK");
	bnOK->E_Click += new cListener1<WorldModule, const MGUI::ClickEvent *>(this, &WorldModule::OnAdd_OK);

	MGUI::Button * bnCancel = layout->GetChildT<MGUI::Button>("Cancel");
	bnCancel->E_Click += new cListener1<WorldModule, const MGUI::ClickEvent *>(this, &WorldModule::OnAdd_Cancel);

	mSelectType = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), layout->GetChild("Type"));
	mSelectType->SetAlign(MGUI::eAlign::STRETCH);

	Editor::Instance()->E_SceneLoaded += new cListener0<WorldModule>(this, &WorldModule::OnSceneLoaded);
	Editor::Instance()->E_SceneUnload += new cListener0<WorldModule>(this, &WorldModule::OnSceneUnload);

	MainRenderView::Instance()->GetLayout()->E_MouseDoubleClick += new cListener1<WorldModule, const MGUI::MouseEvent *>(this, &WorldModule::OnPick);
}

WorldModule::~WorldModule()
{
	delete mSelectLayout;
	delete mPropertyGrid;
	delete mLayout;
}

void WorldModule::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void WorldModule::Show()
{
	mLayout->SetVisible(true);

	Editor::Instance()->SetSelectNode(NULL);
	Editor::Instance()->ClearFlags(FLAG_ALL);
}

void WorldModule::Hide()
{
	mLayout->SetVisible(false);
}

void WorldModule::OnSceneLoaded()
{
	for (int i = 0; i < World::Instance()->GetComponentCount(); ++i)
	{
		WorldComponent * cp = World::Instance()->GetComponent(i);

		mListBox->Append(cp->GetName().c_wstr(), cp);
	}
}

void WorldModule::OnSceneUnload()
{
	mListBox->Clear();
}

void WorldModule::OnSelectChanged(const MGUI::Event * e, int index)
{
	mPropertyGrid->Attach(NULL);

	if (index >= 0)
	{
		WorldComponent * cp = (WorldComponent *)mListBox->GetUserData(index);

		mPropertyGrid->Attach(cp);
	}
}

void WorldModule::OnAdd(const MGUI::ClickEvent * e)
{
	if (World::Instance()->GetFilename() == "")
		return ;

	mSelectType->Clear();

	mClassNames.Clear();
	Root::ENUM_OBJECT_FACTORY(mClassNames, "WorldComponent", false);

	for (int i = 0; i < mClassNames.Size(); ++i)
	{
		mSelectType->Append(mClassNames[i].c_wstr());
	}

	mSelectLayout->SetVisible(true);
}

void WorldModule::OnAdd_OK(const MGUI::ClickEvent * e)
{
	int index = mSelectType->GetSelectIndex();
	if (index >= 0)
	{
		WorldComponent * cp = Root::NEW_OBJECT_T<WorldComponent>(mClassNames[index].c_str());
		if (cp != NULL)
		{
			World::Instance()->AddComponent(cp);

			mListBox->Append(cp->GetName().c_wstr(), cp);

			mListBox->SetSelectIndex(mListBox->GetCount() - 1);
		}
	}

	mSelectLayout->SetVisible(false);
}

void WorldModule::OnAdd_Cancel(const MGUI::ClickEvent * e)
{
	mSelectLayout->SetVisible(false);
}

void WorldModule::OnRemove(const MGUI::ClickEvent * e)
{
	int index = mListBox->GetSelectIndex();
	if (index >= 0)
	{
		WorldComponent * cp = (WorldComponent *)mListBox->GetUserData(index);

		World::Instance()->RemoveComponent(cp, true);

		mListBox->Remove(index);
	}
}

void WorldModule::OnPick(const MGUI::MouseEvent * e)
{
	if (!mLayout->IsVisible())
		return ;

	Float2 pt = IMouse::Instance()->GetPosition();
	Editor::Instance()->MapScreenUnit(pt.x, pt.y);
	if (pt.x <= 0 || pt.x >= 1 ||
		pt.y <= 0 || pt.y >= 1)
		return ;

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);

	Float3 pos;
	if (Terrain::Instance()->RayCheck(&pos, ray))
	{
		GrassManager::Instance()->PushWave(pos.x, pos.z, 1.5f);
	}
}
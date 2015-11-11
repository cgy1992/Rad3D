#include "EnvironmentModule.h"
#include "MainFrame.h"
#include "Editor.h"

ImplementSingleton(EnvironmentModule);

EnvironmentModule::EnvironmentModule()
	: EditorModule("Environment")
{
	mLayout = MGUI::Layout::Load("EnvironmentPanel.layout", NULL);
	mLayout->SetVisible(false);

	mTopWidget = mLayout->GetChild("Caption");

	mPropertyGrid = new PropertyGrid(mLayout);
	mPropertyGrid->SetAlign(MGUI::eAlign::H_STRETCH);

	Editor::Instance()->E_SceneLoaded += new cListener0<EnvironmentModule>(this, &EnvironmentModule::OnSceneAfterLoad);

	MainRenderView::Instance()->GetLayout()->E_MouseDoubleClick += new cListener1<EnvironmentModule, const MGUI::MouseEvent *>(this, &EnvironmentModule::OnPick);
}

EnvironmentModule::~EnvironmentModule()
{
	delete mPropertyGrid;
	delete mLayout;
}

void EnvironmentModule::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);

	MGUI::Rect clRect = mLayout->GetClient();
	MGUI::Rect tlRect = mTopWidget->GetRect();

	mPropertyGrid->SetRect(0, tlRect.h, clRect.w, clRect.h - tlRect.h);
}

void EnvironmentModule::Show()
{
	mLayout->SetVisible(true);

	Editor::Instance()->SetSelectNode(NULL);
	Editor::Instance()->ClearFlags(FLAG_ALL);
}

void EnvironmentModule::Hide()
{
	mLayout->SetVisible(false);
}

void EnvironmentModule::OnSceneAfterLoad()
{
	mPropertyGrid->Attach(World::Instance()->GetEvInfo());
}

void EnvironmentModule::OnPick(const MGUI::MouseEvent * e)
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
#include "NavPathModule.h"
#include "MainFrame.h"
#include "Editor.h"

ImplementSingleton(NavPathModule);

NavPathModule::NavPathModule()
	: EditorModule("NavPath")
{
	mLayout = MGUI::Layout::Load("NavPath.layout", NULL);
	mLayout->SetVisible(false);

	MGUI::CheckBox * cbEditable = (MGUI::CheckBox *)mLayout->GetChild("Editable");
	MGUI::Button * bnBuild = (MGUI::Button *)mLayout->GetChild("Build");
	MGUI::Button * bnLookError = (MGUI::Button *)mLayout->GetChild("LookError");

	cbEditable->E_Checked += new cListener2<NavPathModule, const MGUI::Event *, bool>(this, &NavPathModule::OnEditable);
	bnBuild->E_Click += new cListener1<NavPathModule, const MGUI::ClickEvent *>(this, &NavPathModule::OnBuild);
	bnLookError->E_Click += new cListener1<NavPathModule, const MGUI::ClickEvent *>(this, &NavPathModule::OnLookError);

	mGizmo = new NavGizmo;
	mGizmo->E_PositionChanged += new cListener1<NavPathModule, Float3>(this, &NavPathModule::OnGizmoChanged);

	Editor::Instance()->E_SceneLoaded += new cListener0<NavPathModule>(this, &NavPathModule::OnSceneLoaded);
	Editor::Instance()->E_SceneUnload += new cListener0<NavPathModule>(this, &NavPathModule::OnSceneUnload);
	Editor::Instance()->E_Update += new cListener0<NavPathModule>(this, &NavPathModule::OnUpdate);
	World::Instance()->E_RenderSolidEnd += new cListener0<NavPathModule>(this, &NavPathModule::OnRender);

	MainRenderView::Instance()->GetLayout()->E_MouseDoubleClick += new cListener1<NavPathModule, const MGUI::MouseEvent *>(this, &NavPathModule::OnDBClick);
}

NavPathModule::~NavPathModule()
{
	delete mGizmo;
	delete mLayout;
}

void NavPathModule::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void NavPathModule::Show()
{
	mLayout->SetVisible(true);

	Editor::Instance()->ClearFlags(FLAG_ALL);
	Editor::Instance()->SetSelectNode(NULL);
}

void NavPathModule::Hide()
{
	mLayout->SetVisible(false);
}

void NavPathModule::OnSceneLoaded()
{
}

void NavPathModule::OnSceneUnload()
{
}

void NavPathModule::OnUpdate()
{
	if (mLayout->IsVisible() && mGenerator.IsEditable())
	{
		if (IKeyboard::Instance()->KeyDown(InputCode::KC_DELETE))
		{
			mGenerator.DeleteCurrentPoint();
		}

		Float2 pt = IMouse::Instance()->GetPosition();

		Editor::Instance()->MapScreenUnit(pt.x, pt.y);
		if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
			return ;

		mGizmo->Update(pt.x, pt.y);
	}

	if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LALT))
	{
		mGizmo->SetEnable(false);
	}
	else
	{
		mGizmo->SetEnable(true);
	}
}

void NavPathModule::OnRender()
{
	if (!mLayout->IsVisible())
		return ;

	mGenerator.OnDebugRender();
}

void NavPathModule::OnDBClick(const MGUI::MouseEvent *)
{
	if (!mLayout->IsVisible())
		return ;

	if (mGenerator.IsEditable())
	{
		if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LALT))
		{
			Float2 pt = IMouse::Instance()->GetPosition();
			Editor::Instance()->MapScreenUnit(pt.x, pt.y);
			if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
				return ;

			Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
			int index = mGenerator.PickPoint(ray);
			if (index != -1)
			{
				mGenerator.Connect(index);
			}
		}
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LSHIFT))
		{
			Float2 pt = IMouse::Instance()->GetPosition();

			Editor::Instance()->MapScreenUnit(pt.x, pt.y);
			if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
				return ;

			float dist = 0;
			Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
			if (Editor::Instance()->RayCheckEx(dist, ray, 100 * METER_LEN, EDITOR_NODE_PICK))
			{
				Float3 point = ray.orig + ray.dir * dist;

				int index = mGenerator.PickPoint(ray);
				if (index == -1)
				{
					mGenerator.AddPoint(point, false);
				}
			}
		}
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LCONTROL))
		{
			Float2 pt = IMouse::Instance()->GetPosition();

			Editor::Instance()->MapScreenUnit(pt.x, pt.y);
			if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
				return ;

			float dist = 0;
			Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
			if (Editor::Instance()->RayCheckEx(dist, ray, 100 * METER_LEN, EDITOR_NODE_PICK))
			{
				Float3 point = ray.orig + ray.dir * dist;

				int index = mGenerator.PickPoint(ray);
				if (index == -1)
				{
					mGenerator.AddPoint(point, true);
				}
			}
		}
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_R))
		{
			Float2 pt = IMouse::Instance()->GetPosition();

			Editor::Instance()->MapScreenUnit(pt.x, pt.y);
			if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
				return ;

			Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);

			mGenerator.DeleteEdge(ray);
		}
		else
		{
			Float2 pt = IMouse::Instance()->GetPosition();

			Editor::Instance()->MapScreenUnit(pt.x, pt.y);
			if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
				return ;

			Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
			int index = mGenerator.PickPoint(ray);
			if (index != -1)
				mGenerator.SelectPoint(index);
		}
	}
	else
	{
		Float2 pt = IMouse::Instance()->GetPosition();
		Editor::Instance()->MapScreenUnit(pt.x, pt.y);
		if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
			return ;

		float dist = 0;
		Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
		if (Editor::Instance()->RayCheckEx(dist, ray, 100 * METER_LEN, EDITOR_NODE_PICK))
		{
			Float3 point = ray.orig + ray.dir * dist;

			mGenerator.SetTestPoint(point);
		}
	}
}

void NavPathModule::OnGizmoChanged(Float3 position)
{
	mGenerator.SetSelectPoint(position);
}

void NavPathModule::OnEditable(const MGUI::Event * e, bool checked)
{
	mGenerator.SetEditable(checked ? TRUE : FALSE);
}

void NavPathModule::OnBuild(const MGUI::ClickEvent * e)
{
	mGenerator.Build();
}

void NavPathModule::OnLookError(const MGUI::ClickEvent * e)
{
	mGenerator.LookAtError();
}
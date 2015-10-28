#include "App.h"

Mesh * gMesh = NULL;

MGUI::CheckBox * gCheckBox_Idle1 = NULL;
MGUI::CheckBox * gCheckBox_Idle2 = NULL;
MGUI::CheckBox * gCheckBox_Idle3 = NULL;
MGUI::CheckBox * gCheckBox_Run = NULL;
MGUI::CheckBox * gCheckBox_Walk = NULL;

void UI_OnIdle1(const MGUI::Event * e, bool checked);
void UI_OnIdle2(const MGUI::Event * e, bool checked);
void UI_OnIdle3(const MGUI::Event * e, bool checked);
void UI_OnRun(const MGUI::Event * e, bool checked);
void UI_OnWalk(const MGUI::Event * e, bool checked);

void SetupTextBox(MGUI::Layout * layout, const uchar_t * caption, const String & parentName)
{
	d_assert (layout != NULL);

	MGUI::Widget * parentWidget = layout->GetChild(parentName.c_str());
	d_assert (parentWidget != NULL);

	MGUI::TextBox * textBox = new MGUI::TextBox(NULL, parentWidget);
	textBox->SetAlign(MGUI::eAlign::LEFT | MGUI::eAlign::V_CENTER);
	textBox->SetCaption(caption);
}

void SetupUI()
{
	MGUI::FontManager::Instance()->Load("Sample.font");
	MGUI::LookFeelManager::Instance()->Load("Sample.lookfeel");

	MGUI::Layout * layout = MGUI::Layout::Load("Demo03/MainFrame.layout", NULL);

	SetupTextBox(layout, WSTR_("Idle1"), "T_Idle1");
	SetupTextBox(layout, WSTR_("Idle2"), "T_Idle2");
	SetupTextBox(layout, WSTR_("Idle3"), "T_Idle3");
	SetupTextBox(layout, WSTR_("Run"), "T_Run");
	SetupTextBox(layout, WSTR_("Walk"), "T_Walk");

	gCheckBox_Idle1 = (MGUI::CheckBox *)layout->GetChild("Idle1");
	gCheckBox_Idle2 = (MGUI::CheckBox *)layout->GetChild("Idle2");
	gCheckBox_Idle3 = (MGUI::CheckBox *)layout->GetChild("Idle3");
	gCheckBox_Run = (MGUI::CheckBox *)layout->GetChild("Run");
	gCheckBox_Walk = (MGUI::CheckBox *)layout->GetChild("Walk");

	gCheckBox_Idle1->SetChecked(true);

	gCheckBox_Idle1->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnIdle1);
	gCheckBox_Idle2->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnIdle2);
	gCheckBox_Idle3->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnIdle3);
	gCheckBox_Run->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnRun);
	gCheckBox_Walk->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnWalk);
}

void UI_OnIdle1(const MGUI::Event * e, bool checked)
{
	gCheckBox_Idle1->SetChecked(true);
	gCheckBox_Idle2->SetChecked(false);
	gCheckBox_Idle3->SetChecked(false);
	gCheckBox_Run->SetChecked(false);
	gCheckBox_Walk->SetChecked(false);

	gMesh->PlayAnimation("Idle1");
}

void UI_OnIdle2(const MGUI::Event * e, bool checked)
{
	gCheckBox_Idle1->SetChecked(false);
	gCheckBox_Idle2->SetChecked(true);
	gCheckBox_Idle3->SetChecked(false);
	gCheckBox_Run->SetChecked(false);
	gCheckBox_Walk->SetChecked(false);

	gMesh->PlayAnimation("Idle2");
}

void UI_OnIdle3(const MGUI::Event * e, bool checked)
{
	gCheckBox_Idle1->SetChecked(false);
	gCheckBox_Idle2->SetChecked(false);
	gCheckBox_Idle3->SetChecked(true);
	gCheckBox_Run->SetChecked(false);
	gCheckBox_Walk->SetChecked(false);

	gMesh->PlayAnimation("Idle3");
}

void UI_OnRun(const MGUI::Event * e, bool checked)
{
	gCheckBox_Idle1->SetChecked(false);
	gCheckBox_Idle2->SetChecked(false);
	gCheckBox_Idle3->SetChecked(false);
	gCheckBox_Run->SetChecked(true);
	gCheckBox_Walk->SetChecked(false);

	gMesh->PlayAnimation("Run");
}

void UI_OnWalk(const MGUI::Event * e, bool checked)
{
	gCheckBox_Idle1->SetChecked(false);
	gCheckBox_Idle2->SetChecked(false);
	gCheckBox_Idle3->SetChecked(false);
	gCheckBox_Run->SetChecked(false);
	gCheckBox_Walk->SetChecked(true);

	gMesh->PlayAnimation("Walk");
}

class Demo03_SkeletonAnimation : public App
{
public:
	Demo03_SkeletonAnimation() {}
	virtual ~Demo03_SkeletonAnimation() {}

	virtual void OnSetupResource()
	{
#ifdef M_PLATFORM_WIN32
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample"));
#endif

#ifdef M_PLATFORM_ANDROID
		ResourceManager::Instance()->AddArchive(new APKArchive("Core"));
		ResourceManager::Instance()->AddArchive(new APKArchive("Sample"));
#endif
	}

	void LookMesh(Mesh* entity)
	{
		Aabb bound = entity->GetWorldAabb();

		float size = 0;
		size = Max(size, bound.GetSize().x);
		size = Max(size, bound.GetSize().y);
		size = Max(size, bound.GetSize().z);

		Node * pCamera = World::Instance()->MainCamera();
		pCamera->SetPosition(bound.GetCenter() + Float3(0, 0, -2.0f) * size);
		pCamera->SetDirection(bound.GetCenter() - pCamera->GetPosition());
	}

	virtual void OnInit()
	{
		SetupUI();

		MeshSourcePtr pMeshSource = MeshManager::Instance()->LoadMesh("Mesh/fox.mesh");

		gMesh = new Mesh;
		gMesh->SetSource(pMeshSource);
		gMesh->LoadAnimation("Idle1", "Mesh/fox_idle1.anim");
		gMesh->LoadAnimation("Idle2", "Mesh/fox_idle2.anim");
		gMesh->LoadAnimation("Idle3", "Mesh/fox_idle3.anim");
		gMesh->LoadAnimation("Run", "Mesh/fox_run.anim");
		gMesh->LoadAnimation("Walk", "Mesh/fox_walk.anim");

		gMesh->PlayAnimation("Idle1");

		LookMesh(gMesh);

		/*
		for (int i = 0; i < 250; ++i)
		{
			Mesh * mesh = new Mesh;

			mesh->SetSource(pMeshSource);
			mesh->PlayAnimation("Idle1");
		}
		*/

		World::Instance()->MainLight()->SetDirection(Float3(-1, 0, 1));
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
		delete gMesh;
	}

	virtual void OnPause()
	{
	}

	virtual void OnResume()
	{
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new Demo03_SkeletonAnimation;
}

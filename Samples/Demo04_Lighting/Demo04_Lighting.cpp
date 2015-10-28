#include "App.h"

Mesh * gMesh = NULL;

Light * gLightDir = NULL;
Light * gLightPoint = NULL;
Light * gLightSpot = NULL;

Billboard * gBillboard = NULL;

MGUI::CheckBox * gCheckBox_DirLight = NULL;
MGUI::CheckBox * gCheckBox_PointLight = NULL;
MGUI::CheckBox * gCheckBox_SpotLight = NULL;

void LookMesh(Mesh * entity)
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

void SetupTextBox(MGUI::Layout * layout, const uchar_t * caption, const String & parentName)
{
	d_assert (layout != NULL);

	MGUI::Widget * parentWidget = layout->GetChild(parentName.c_str());
	d_assert (parentWidget != NULL);

	MGUI::TextBox * textBox = new MGUI::TextBox(NULL, parentWidget);
	textBox->SetAlign(MGUI::eAlign::LEFT | MGUI::eAlign::V_CENTER);
	textBox->SetCaption(caption);
}

void UI_OnDirection(const MGUI::Event * e, bool checked);
void UI_OnPoint(const MGUI::Event * e, bool checked);
void UI_OnSpot(const MGUI::Event * e, bool checked);

void SetupUI()
{
	MGUI::FontManager::Instance()->Load("Sample.font");
	MGUI::LookFeelManager::Instance()->Load("Sample.lookfeel");

	MGUI::Layout * layout = MGUI::Layout::Load("Demo04/MainFrame.layout", NULL);

	SetupTextBox(layout, WSTR_("Direction"), "T_Direction");
	SetupTextBox(layout, WSTR_("Point"), "T_Point");
	SetupTextBox(layout, WSTR_("Spot"), "T_Spot");

	gCheckBox_DirLight = (MGUI::CheckBox *)layout->GetChild("Direction");
	gCheckBox_PointLight = (MGUI::CheckBox *)layout->GetChild("Point");
	gCheckBox_SpotLight = (MGUI::CheckBox *)layout->GetChild("Spot");
	gCheckBox_DirLight->SetChecked(true);

	gCheckBox_DirLight->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnDirection);
	gCheckBox_PointLight->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnPoint);
	gCheckBox_SpotLight->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnSpot);
}

void UI_OnDirection(const MGUI::Event * e, bool checked)
{
	gLightDir->SetVisible(checked);
}

void UI_OnPoint(const MGUI::Event * e, bool checked)
{
	gLightPoint->SetVisible(checked);
}

void UI_OnSpot(const MGUI::Event * e, bool checked)
{
	gLightSpot->SetVisible(checked);
}




class Demo04_Lighting : public App
{
public:
	Demo04_Lighting() {}
	virtual ~Demo04_Lighting() {}

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

	virtual void OnInit()
	{
		SetupUI();

		// Create Mesh
		MeshSourcePtr pMeshSource = MeshManager::Instance()->LoadMesh("Mesh/fox.mesh");
		
		gMesh = new Mesh;
		gMesh->SetSource(pMeshSource);
		gMesh->LoadAnimation("Idle1", "Mesh/fox_idle1.anim");
		gMesh->SetLighting(true);

		gMesh->PlayAnimation("Idle1");

		LookMesh(gMesh);

		// Setup Main Light
		World::Instance()->MainLight()->SetDirection(Float3(-1, 0, 1));
		World::Instance()->MainLight()->SetAmbient(Float3(0.2f, 0.2f, 0.2f));
		World::Instance()->MainLight()->SetDiffuse(Float3(0.2f, 0.2f, 0.2f));

		// Create Lights
		gLightDir = new Light(eLightType::DIRECTION);
		gLightPoint = new Light(eLightType::POINT);
		gLightSpot = new Light(eLightType::SPOT);

		gLightDir->SetDiffuse(Float3(1, 0, 0));
		gLightPoint->SetDiffuse(Float3(0, 1, 0));
		gLightSpot->SetDiffuse(Float3(0, 0, 1));

		gLightDir->SetVisible(true);
		gLightPoint->SetVisible(false);
		gLightSpot->SetVisible(false);

		// Create Billboard
		gBillboard = new Billboard;
		gBillboard->SetSize(Float2(1, 1));
		gBillboard->SetTexture("flare.png");
		gBillboard->GetMaterial()->blendMode = eBlendMode::ADD;
	}

	virtual void OnUpdate()
	{
		float k = Root::Instance()->GetTime();
		float r = 3.0f;

		float x = Math::Cos(k) * r;
		float z = Math::Sin(k) * r;

		gBillboard->SetPosition(Float3(x, 5, z));

		gLightDir->SetPosition(Float3(x, 5, z));
		gLightDir->SetDirection(-Float3(x, 0, z));

		gLightPoint->SetPosition(Float3(x, 5, z));
		gLightPoint->SetDirection(-Float3(x, 0, z));

		gLightSpot->SetPosition(Float3(x, 5, z));
		gLightSpot->SetDirection(-Float3(x, 0, z));
	}

	virtual void OnShutdown()
	{
		delete gLightDir;
		delete gLightPoint;
		delete gLightSpot;
		delete gBillboard;
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
	*ppApp = new Demo04_Lighting;
}

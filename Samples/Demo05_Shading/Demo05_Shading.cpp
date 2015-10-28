#include "App.h"

Mesh * gMesh = NULL;

ShaderFX * gShaderFX_Base = NULL;
ShaderFX * gShaderFX_Normal = NULL;
ShaderFX * gShaderFX_Transparent = NULL;
ShaderFX * gShaderFX_Carton = NULL;

MGUI::CheckBox * gCheckBox_Base = NULL;
MGUI::CheckBox * gCheckBox_Normal = NULL;
MGUI::CheckBox * gCheckBox_Transparent = NULL;
MGUI::CheckBox * gCheckBox_Cartoon = NULL;

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

void UI_OnBase(const MGUI::Event * e, bool checked);
void UI_OnNormal(const MGUI::Event * e, bool checked);
void UI_OnTransparent(const MGUI::Event * e, bool checked);
void UI_OnCartoon(const MGUI::Event * e, bool checked);

void SetupUI()
{
	MGUI::FontManager::Instance()->Load("Sample.font");
	MGUI::LookFeelManager::Instance()->Load("Sample.lookfeel");

	MGUI::Layout * layout = MGUI::Layout::Load("Demo05/MainFrame.layout", NULL);

	SetupTextBox(layout, WSTR_("Base"), "T_Base");
	SetupTextBox(layout, WSTR_("Normal"), "T_Normal");
	SetupTextBox(layout, WSTR_("Transparent"), "T_Transparent");
	SetupTextBox(layout, WSTR_("Cartoon"), "T_Cartoon");

	gCheckBox_Base = (MGUI::CheckBox *)layout->GetChild("Base");
	gCheckBox_Normal = (MGUI::CheckBox *)layout->GetChild("Normal");
	gCheckBox_Transparent = (MGUI::CheckBox *)layout->GetChild("Transparent");
	gCheckBox_Cartoon = (MGUI::CheckBox *)layout->GetChild("Cartoon");
	gCheckBox_Base->SetChecked(true);

	gCheckBox_Base->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnBase);
	gCheckBox_Normal->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnNormal);
	gCheckBox_Transparent->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnTransparent);
	gCheckBox_Cartoon->E_Checked += new ncListener2<const MGUI::Event *, bool>(UI_OnCartoon);
}

void UI_OnBase(const MGUI::Event * e, bool checked)
{
	gCheckBox_Base->SetChecked(true);
	gCheckBox_Normal->SetChecked(false);
	gCheckBox_Transparent->SetChecked(false);
	gCheckBox_Cartoon->SetChecked(false);

	gMesh->SetShaderFX(gShaderFX_Base);
}

void UI_OnNormal(const MGUI::Event * e, bool checked)
{
	gCheckBox_Base->SetChecked(false);
	gCheckBox_Normal->SetChecked(true);
	gCheckBox_Transparent->SetChecked(false);
	gCheckBox_Cartoon->SetChecked(false);

	gMesh->SetShaderFX(gShaderFX_Normal);
}

void UI_OnTransparent(const MGUI::Event * e, bool checked)
{
	gCheckBox_Base->SetChecked(false);
	gCheckBox_Normal->SetChecked(false);
	gCheckBox_Transparent->SetChecked(true);
	gCheckBox_Cartoon->SetChecked(false);

	gMesh->SetShaderFX(gShaderFX_Transparent);
}

void UI_OnCartoon(const MGUI::Event * e, bool checked)
{
	gCheckBox_Base->SetChecked(false);
	gCheckBox_Normal->SetChecked(false);
	gCheckBox_Transparent->SetChecked(false);
	gCheckBox_Cartoon->SetChecked(true);

	gMesh->SetShaderFX(gShaderFX_Carton);
}

class Demo05_Shading : public App
{
public:
	Demo05_Shading() {}
	virtual ~Demo05_Shading() {}

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
		World::Instance()->MainLight()->SetDirection(Float3(-1, -1, 1));

		// Load ShaderFX
		gShaderFX_Base = ShaderFXManager::Instance()->Load("Demo05_Base", "Demo05/Base.mfx");
		gShaderFX_Normal = ShaderFXManager::Instance()->Load("Demo05_Normal", "Demo05/Normal.mfx");
		gShaderFX_Transparent = ShaderFXManager::Instance()->Load("Demo05_Transparent", "Demo05/Transparent.mfx");
		gShaderFX_Carton = ShaderFXManager::Instance()->Load("Demo05_CelShading", "Demo05/CelShading.mfx");

		d_assert (gShaderFX_Base && gShaderFX_Normal && gShaderFX_Transparent && gShaderFX_Carton);

		gMesh->SetShaderFX(gShaderFX_Base);
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
	*ppApp = new Demo05_Shading;
}

#include "App.h"
#include "MShadow.h"

MGUI::Layout * gLayout = NULL;
Mesh * gMesh = NULL;
Mesh * gPlane = NULL;
Shadow * gShadow;

void LookMesh(Mesh * entity)
{
	Aabb bound = entity->GetWorldAabb();

	float size = 0;
	size = Max(size, bound.GetSize().x);
	size = Max(size, bound.GetSize().y);
	size = Max(size, bound.GetSize().z);

	Node * pCamera = World::Instance()->MainCamera();
	pCamera->SetPosition(bound.GetCenter() + Float3(0, 1, -2) * size);
	pCamera->SetDirection(bound.GetCenter() - pCamera->GetPosition());
}

class Demo17_Shadow: public App
{
public:
	Demo17_Shadow() {}
	virtual ~Demo17_Shadow() {}

	virtual void OnSetupResource()
	{
#ifdef M_PLATFORM_WIN32
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample/RenderProcess"));
#endif

#ifdef M_PLATFORM_ANDROID
		ResourceManager::Instance()->AddArchive(new APKArchive("Core"));
		ResourceManager::Instance()->AddArchive(new APKArchive("Sample"));
#endif
	}

	virtual void OnInit()
	{
		MGUI::FontManager::Instance()->Load("Sample.font");

		// Setup Main Light
		World::Instance()->MainLight()->SetDirection(Float3(-1, -1, 1));

		// Create Mesh
		MeshSourcePtr pMeshSource = MeshManager::Instance()->LoadMesh("Mesh/fox.mesh");

		gMesh = new Mesh;
		gMesh->SetSource(pMeshSource);
		gMesh->LoadAnimation("Idle1", "Mesh/fox_idle1.anim");
		gMesh->SetLighting(true);
		gMesh->SetCastShadow(true);

		gMesh->PlayAnimation("Idle1");

		LookMesh(gMesh);

		gPlane = MeshManager::Instance()->NewPlane(Plane(0, 1, 0, 0), 500, 500);
		gPlane->SetReceiveShadow(true);
		gPlane->GetSubMesh(0)->GetMaterial()->diffuse = Float3(0.2f, 0.2f, 0.2f);

		gShadow = new Shadow(World::Instance()->MainRenderContext(), POST_PROCESS_MASK - 1);
		gShadow->SetMapSize(512);
		gShadow->SetDistance(50);
		gShadow->SetFadeRatio(0.8f);
		gShadow->SetOffset(200);
		gShadow->SetColor(Float4(0.5f, 0.5f, 0.5f));
		gShadow->SetEnable(true);

		gLayout = new MGUI::Layout(NULL);
		gLayout->SetAlign(MGUI::eAlign::STRETCH);
		gLayout->E_DoubleClick += new cListener1<Demo17_Shadow, const MGUI::ClickEvent *>(this, &Demo17_Shadow::OnDoubleClick);

		//MGUI::ImageBox * imageBox = new MGUI::ImageBox(NULL, gLayout);
		//imageBox->SetSkinAlignedEx(gShadow->GetShadowContext()->GetRenderTarget()->GetTexture());
	}

	void OnDoubleClick(const MGUI::ClickEvent * e)
	{
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
		delete gMesh;
		delete gPlane;
		delete gLayout;
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
	*ppApp = new Demo17_Shadow;
}

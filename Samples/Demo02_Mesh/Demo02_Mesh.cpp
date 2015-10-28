#include "App.h"

Mesh * gMesh = NULL;

void LookMesh(Mesh* entity)
{
	Aabb bound = entity->GetWorldAabb();

	float size = 0;
	size = Max(size, bound.GetSize().x);
	size = Max(size, bound.GetSize().y);
	size = Max(size, bound.GetSize().z);

	Node * pCamera = World::Instance()->MainCamera();
	pCamera->SetPosition(bound.GetCenter() + Float3(0, 1.0f, -2.0f) * size);
	pCamera->SetDirection(bound.GetCenter() - pCamera->GetPosition());
}

class Demo02_Mesh : public App
{
public:
	Demo02_Mesh() {}
	virtual ~Demo02_Mesh() {}

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
		MGUI::FontManager::Instance()->Load("Sample.font");

		gMesh = new Mesh;
		gMesh->SetFilename("Mesh/fox.mesh");

		LookMesh(gMesh);

		World::Instance()->MainLight()->SetDirection(Float3(-1, 0, 1));
		World::Instance()->MainLight()->SetSpecular(Float3(1, 1, 1));
	}

	virtual void OnUpdate()
	{
		Quat q;
		q.FromAxis(Float3(0, 1, 0), 0.5f * Root::Instance()->GetFrameTime());
		
		gMesh->Rotate(q);
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
	*ppApp = new Demo02_Mesh;
}

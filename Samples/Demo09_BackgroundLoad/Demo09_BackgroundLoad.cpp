#include "App.h"

Mesh * gMesh = NULL;

void LookMesh(Mesh* entity)
{
	entity->_updateTM();

	Aabb bound = entity->GetWorldAabb();

	float size = 0;
	size = Max(size, bound.GetSize().x);
	size = Max(size, bound.GetSize().y);
	size = Max(size, bound.GetSize().z);

	Node * pCamera = World::Instance()->MainCamera();
	pCamera->SetPosition(bound.GetCenter() + Float3(0, 0, -2.0f) * size);
	pCamera->SetDirection(bound.GetCenter() - pCamera->GetPosition());
}

void OnMeshLoad(Node * sender)
{
	LookMesh((Mesh *)sender);
}


class Demo09_BackgroundLoad : public App
{
public:
	Demo09_BackgroundLoad() {}
	virtual ~Demo09_BackgroundLoad() {}

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
		World::Instance()->MainLight()->SetDirection(Float3(-1, 0, 1));

		World::Instance()->EnableMultiThreadLoad();

		MeshSourcePtr pMeshSource = MeshManager::Instance()->LoadMesh("Mesh/fox.mesh");

		gMesh = new Mesh;
		gMesh->SetSource(pMeshSource);
		gMesh->E_Loaded += new ncListener1<Node *>(OnMeshLoad);
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
	*ppApp = new Demo09_BackgroundLoad;
}

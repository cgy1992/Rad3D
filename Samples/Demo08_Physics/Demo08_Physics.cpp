#include "App.h"

#define FLAG_RAY 0x00000001
#define FLAG_FLOOR 0xFFFFFFFE
#define FLAG_BOX 0xFFFFFFFF

Mesh * gMeshBox = NULL;
Mesh * gMeshFloor = NULL;

PhyBox * gPhyEntityBox = NULL;
PhyPlane * gPhyEntityFloor = NULL;

void RayTraceCallBack(void * data, PhyContact * contacts, int count)
{
	PhyEntity * box = contacts->GetEntity2();

	box->SetLinearVel(Float3(0, 20, 0));
	//box->SetAngularVel(Float3(0, 2, 0));
}

void OnPick(const MGUI::MouseEvent * e)
{
	// Do Ray Trace
	const Viewport & vp = World::Instance()->MainRenderContext()->GetViewport();

	float x = (e->x - vp.x) / vp.w;
	float y = (e->y - vp.y) / vp.h;

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);

	PhyWorld::Instance()->RayCheck(ray, 100, FLAG_RAY, RayTraceCallBack, NULL);
}

void OnPickTE(const MGUI::TouchEvent * e)
{
	MGUI::MouseEvent e2(NULL);
	e2.id = 0;
	e2.x = e->x;
	e2.y = e->y;

	OnPick(&e2);
}

class Demo08_Physics : public App
{
public:
	Demo08_Physics() {}
	virtual ~Demo08_Physics() {}

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
		MGUI::InputManager::Instance()->E_MouseUp += new ncListener1<const MGUI::MouseEvent *>(OnPick);
		MGUI::InputManager::Instance()->E_TouchUp += new ncListener1<const MGUI::TouchEvent *>(OnPickTE);

		World::Instance()->MainRenderContext()->SetColorClear(eClearMode::ALL, Float4(0.15f, 0.15f, 0.15f));
		World::Instance()->MainCamera()->SetPosition(Float3(0, 5, -10));
		World::Instance()->MainCamera()->SetDirection(Float3(0, -5, 10));
		World::Instance()->MainLight()->SetDirection(Float3(0, -0.5f, 1));

		gMeshFloor = MeshManager::Instance()->NewPlane(Plane(0, 1, 0, 0), 10, 10);
		gMeshFloor->GetSubMesh(0)->GetMaterial()->ambient = Float3(0.3f, 0.0f, 0.3f);
		gMeshFloor->GetSubMesh(0)->GetMaterial()->diffuse = Float3(0.0f, 0.0f, 0.0f);

		gMeshBox = MeshManager::Instance()->NewBox(1, 1, 1);
		gMeshBox->GetSubMesh(0)->GetMaterial()->ambient = Float3(0.1f, 0.1f, 0.1f);
		gMeshBox->GetSubMesh(0)->GetMaterial()->diffuse = Float3(0.0f, 0.5f, 0.0f);
		gMeshBox->SetPosition(Float3(0, 3, 0));

		PhyWorld::Instance()->SetWorldGravity(0, -9.8f, 0);
		PhyWorld::Instance()->SetWorldContactSurfaceLayer(0.01f);

		gPhyEntityFloor = new PhyPlane;
		gPhyEntityFloor->SetCollisionFlag(FLAG_FLOOR);

		gPhyEntityBox = new PhyBox(PhyEntity::RIGID_BODY);
		gPhyEntityBox->SetBodyAutoDisable(false);
		gPhyEntityBox->SetLength(1, 1, 1);
		gPhyEntityBox->SetMass(5, 1, 1, 1);
		gPhyEntityBox->SetPosition(Float3(0, 3, 0));
		gPhyEntityBox->SetBounceParam(1.0f, 1.0f);
		gPhyEntityBox->SetCollisionFlag(FLAG_BOX);
		gPhyEntityBox->SetAngularVel(Float3(2, 2, 2));
	}

	virtual void OnUpdate()
	{
		gMeshBox->SetPosition(gPhyEntityBox->GetPosition());
		gMeshBox->SetRotation(gPhyEntityBox->GetRotation());
	}

	virtual void OnShutdown()
	{
		delete gPhyEntityBox;
		delete gPhyEntityFloor;

		delete gMeshBox;
		delete gMeshFloor;
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
	*ppApp = new Demo08_Physics;
}

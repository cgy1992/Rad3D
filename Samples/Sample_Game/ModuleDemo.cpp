#include "ModuleDemo.h"
#include "CameraController.h"
#include "Engine.h"

ModuleDemo::ModuleDemo()
{
	mPlayer = new Actor("Sqmo_236.mesh");
	mPlayer->MoveTo(Float3(6000, 0, 6000), true);
	mPlayer->RotateTo(0, true);
	mPlayer->_getNode()->SetScale(Float3(120, 120, 120));

	mPlayer->AddComponent(new CameraController);

	AddActor(mPlayer);

	MGUI::InputManager::Instance()->E_MouseDown += 
		new cListener1<ModuleDemo, const MGUI::MouseEvent *>(this, &ModuleDemo::OnMouseDown);

	MGUI::InputManager::Instance()->E_TouchDown += 
		new cListener1<ModuleDemo, const MGUI::TouchEvent *>(this, &ModuleDemo::OnTouchDown);
}

ModuleDemo::~ModuleDemo()
{
}

void ModuleDemo::Update(float frameTime)
{
	Module::Update(frameTime);
}

void ModuleDemo::OnMouseDown(const MGUI::MouseEvent * e)
{
	if (e->id != InputCode::MKC_LEFT)
		return ;

	float x = e->x, y = e->y;

	Engine::Instance()->MapScreenCoordUnit(x, y);

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);

	Float3 position;
	if (Terrain::Instance()->RayCheck(&position, ray))
	{
		mPlayer->MoveTo(position);
	}
}

void ModuleDemo::OnTouchDown(const MGUI::TouchEvent * e)
{
	float x = e->x, y = e->y;

	Engine::Instance()->MapScreenCoordUnit(x, y);

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);

	Float3 position;
	if (Terrain::Instance()->RayCheck(&position, ray))
	{
		mPlayer->MoveTo(position);
	}
}

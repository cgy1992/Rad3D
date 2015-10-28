#include "Engine.h"

ImplementSingleton(Engine);

Engine::Engine()
	: mModule(NULL)
{
}

Engine::~Engine()
{
	safe_delete (mModule);
}

void Engine::Update()
{
	if (mModule)
	{
		mModule->Update(Root::Instance()->GetFrameTime());
	}
}

void Engine::SetModule(Module * pModule)
{
	d_assert (mModule != pModule);

	safe_delete (mModule);

	mModule = pModule;

	if (pModule != NULL)
	{
		mModule = pModule;
	}
}

Module * Engine::GetModule()
{
	return mModule;
}

void Engine::MapScreenCoordUnit(float & x, float &y)
{
	const Float2 & invSize = MGUI::Engine::Instance()->GetInvSize();

	x = x * invSize.x;
	y = y * invSize.y;

	x = Math::Clamp(x, 0.0f, 1.0f);
	y = Math::Clamp(y, 0.0f, 1.0f);
}

float Engine::GetHeightAt(const Float3 & pos)
{
	float h = 0;

	Terrain::Instance()->GetHeightAt(h, pos.x, pos.z);

	return h;
}

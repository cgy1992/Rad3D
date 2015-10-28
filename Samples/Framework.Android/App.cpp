#include "App.h"

ImplementSingleton(App);

App::App()
{
	mRoot = NULL;
	mRenderSystem = NULL;
	mAudioSystem = NULL;
	mUIEngine = NULL;
	mParticleFX = NULL;
	mPhyWorld = NULL;
	mWorld = NULL;
	mDebugInfo = NULL;

	mPause = false;
}

App::~App()
{
}

void App::Init(int width, int height)
{
	d_printf("-: Cache path '%s'.", mCachePath.c_str());

	d_printf("-: Root Initialize...");
	FixedString256 logfile;
	logfile = mCachePath + "/log.txt";
	mRoot = new Root(NULL, logfile.c_str());

	d_printf("-: Setup Resource...");
	mResourceManager = new ResourceManager;
	OnSetupResource();

	d_printf("-: Engine Initialize...");
	RenderSystem::Config config;
	config.width = width;
	config.height = height;
	mRenderSystem = new GLRenderSystem(config);

	mAudioSystem = new SLAudioSystem;

	mUIEngine = new MGUI::Engine;

	mParticleFX = new ParticleFX;

	mPhyWorld = new PhyWorld;

	mWorld = new World;

	mIME = new IME;

	d_printf("-: On Initialize...");
	OnInit();

	d_printf("-: DebugInfo Initialize...");
	mDebugInfo = new DebugInfo;

	d_printf("-: App Init OK...");
}

void App::Update()
{
	mRoot->Update();

	if (!mPause)
	{
		float elapsedTime = mRoot->GetFrameTime();

		if (elapsedTime > 0.1f)
			elapsedTime = 0.1f;

		mPhyWorld->Update(elapsedTime);

		mWorld->Update(elapsedTime);

		mUIEngine->Update(elapsedTime);

		mAudioSystem->Update(World::Instance()->MainCamera()->GetWorldPosition());

		mDebugInfo->Update();

		mRenderSystem->Begin();

		mWorld->Render();

		mUIEngine->Render();

		mRenderSystem->End();

		OnUpdate();
	}
}

void App::Shutdown()
{
	d_printf("-: On Shutdown...");
	OnShutdown();

	d_printf("-: Engine Shutdown...");
	delete mIME;
	delete mDebugInfo;
	delete mParticleFX;
	delete mPhyWorld;
	delete mWorld;
	delete mUIEngine;
	delete mAudioSystem;
	delete mRenderSystem;
	delete mRoot;

	d_printf("-: Shutdown OK...");
}

void App::Pause()
{
	if (!mPause)
	{
		OnPause();

		d_printf("---: Lost device...");

		mRenderSystem->OnLostDevice();

		mPause = true;
	}
}

void App::Resume()
{
	if (mPause)
	{
		d_printf("---: Reset device...");

		mRenderSystem->OnResetDevice();

		OnResume();

		mPause = false;
	}
}

void App::Resize(int w, int h)
{
	if (mRoot)
	{
		mRenderSystem->OnResize(w, h);
		mUIEngine->OnResize(w, h);

		OnResize(w, h);
	}
}

void App::MapScreenUnit(float & x, float & y)
{
	Float2 sz = MGUI::Engine::Instance()->GetInvSize();

	x *= sz.x;
	y *= sz.y;
}

void App::InjectMouseMove(float _x, float _y)
{
	MGUI::InputManager::Instance()->_injectMouseMove(_x, _y);
}

void App::InjectMouseWheel(float _z)
{
	MGUI::InputManager::Instance()->_injectMouseWheel(_z);
}

void App::InjectMouseDown(int _id, float _x, float _y)
{
	MGUI::InputManager::Instance()->_injectMouseDown(_id, _x, _y);
}

void App::InjectMouseUp(int _id, float _x, float _y)
{
	MGUI::InputManager::Instance()->_injectMouseUp(_id, _x, _y);
}

void App::InjectKeyDown(int _key, uchar_t _text)
{
	MGUI::InputManager::Instance()->_injectKeyDown(_key, _text);
}

void App::InjectKeyUp(int _key)
{
	MGUI::InputManager::Instance()->_injectKeyUp(_key);
}

void App::InjectTouchDown(int _id, float _x, float _y)
{
	MGUI::InputManager::Instance()->_injectTouchDown(_id, _x, _y);
}

void App::InjectTouchUp(int _id, float _x, float _y)
{
	MGUI::InputManager::Instance()->_injectTouchUp(_id, _x, _y);
}

void App::InjectTouchMove(int _id, float _x, float _y)
{
	MGUI::InputManager::Instance()->_injectTouchMove(_id, _x, _y);
}

void App::InjectTouchCancel(int _id, float _x, float _y)
{
	MGUI::InputManager::Instance()->_injectTouchCancel(_id, _x, _y);
}

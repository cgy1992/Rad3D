#include "stdafx.h"
#include "App.h"
#include "Engine.h"
#include "ModuleDemo.h"

#ifdef M_PLATFORM_ANDROID

#include "NDK_Utils.h"

DataStreamPtr ReadApkFile(const String & filename)
{
	void * data = NULL;
	int size = NDK_Uitl::ReadAPKFile(&data, filename.c_str());
	if (size > 0)
		return new MemoryStream((byte *)data, size, true);

	return NULL;
}

#endif

class Sample_Game : public App
{
	Engine * mEngine;

public:
	Sample_Game() {}
	virtual ~Sample_Game() {}

	virtual void OnSetupResource()
	{
#ifdef M_PLATFORM_WIN32
		ResourceManager::Instance()->AddArchive(new MPKArchive("../Data.MPK", NULL));
		//ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample"));
		//ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample/Game/Audio"));
		//ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample/Game/Brush"));
		//ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample/Game/Mesh"));
		//ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample/Game/Scene"));
		//ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample/Game/Shader"));
#endif

#ifdef M_PLATFORM_ANDROID
		ResourceManager::Instance()->AddArchive(new MPKArchive("Data.MPK", ReadApkFile));
		ResourceManager::Instance()->AddArchive(new APKArchive("Sample"));
#endif
	}

	virtual void OnInit()
	{
		MGUI::FontManager::Instance()->Load("Sample.font");

		mEngine = new Engine;

		// enable Environment manage fog
		Environment::Instance()->SetFlags(eEnvFlag::FOG);
		
		// enable multi-thread load
		World::Instance()->EnableMultiThreadLoad();

		// load scene
		World::Instance()->Load("test002/test002.scene");

		// play bgm
		AudioSystem::Instance()->PlaySound("bgm.OGG", 0, AUDIO_FLAG_LOOPED, 0);

		mEngine->SetModule(new ModuleDemo);
	}

	virtual void OnUpdate()
	{
		mEngine->Update();
	}

	virtual void OnShutdown()
	{
		World::Instance()->Unload();

		delete mEngine;
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
	*ppApp = new Sample_Game;
}

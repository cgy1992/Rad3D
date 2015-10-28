#include "App.h"

ParticleSystem * gParticleSystem = NULL;

class Demo07_ParticleFX : public App
{
public:
	Demo07_ParticleFX() {}
	virtual ~Demo07_ParticleFX() {}

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
		d_printf("11111111111");
		gParticleSystem = new ParticleSystem();
		gParticleSystem->SetFilename("ribbon.particle");

		World::Instance()->MainCamera()->SetPosition(Float3(0, 200, -500));
		World::Instance()->MainCamera()->SetDirection(-World::Instance()->MainCamera()->GetPosition());
		World::Instance()->MainRenderContext()->SetColorClear(eClearMode::ALL, Float4(0.2f, 0.2f, 0.2f));
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
		delete gParticleSystem;
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
	*ppApp = new Demo07_ParticleFX;
}

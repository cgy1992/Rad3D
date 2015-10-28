#include "App.h"

class Demo01_Empty : public App
{
public:
	Demo01_Empty() {}
	virtual ~Demo01_Empty() {}

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
		MGUI::LookFeelManager::Instance()->Load("Sample.lookfeel");

		MGUI::Layout * layout = new MGUI::Layout(NULL, NULL);
		layout->SetAlign(MGUI::eAlign::STRETCH);

		MGUI::TextBox * text = new MGUI::TextBox(NULL, layout);
		text->SetAlign(MGUI::eAlign::CENTER);
		text->SetCaption(WSTR_("Hello World"));

		World::Instance()->MainRenderContext()->SetColorClear(eClearMode::ALL, Float4(0.1f, 0.1f, 0.3f));
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
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
	*ppApp = new Demo01_Empty;
}

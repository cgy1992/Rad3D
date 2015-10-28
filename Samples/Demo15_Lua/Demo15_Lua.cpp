#include "App.h"
#include "LuaHelper.h"

MGUI::TextBox * gTextBox = NULL;

int hello_lua(lua_State * LS)
{
	gTextBox->SetCaption(WSTR_("hello_lua"));

	return 0;
}

const char * lua_script = " \
	hello_lua(); \
";

class Demo05_Lua : public App
{
	LuaHelper * mLuaHelper;

public:
	Demo05_Lua() {}
	virtual ~Demo05_Lua() {}

	virtual void OnSetupResource()
	{
#ifdef M_PLATFORM_WIN32
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Media/particle"));
#endif

#ifdef M_PLATFORM_ANDROID
		ResourceManager::Instance()->AddArchive(new APKArchive("Core"));
		ResourceManager::Instance()->AddArchive(new APKArchive("Sample"));
#endif
	}

	virtual void OnInit()
	{
		mLuaHelper = new LuaHelper;

		MGUI::FontManager::Instance()->Load("Sample.font");

		MGUI::Layout * layout = new MGUI::Layout(NULL, NULL);
		layout->SetAlign(MGUI::eAlign::STRETCH);

		gTextBox = new MGUI::TextBox(NULL, layout);
		gTextBox->SetAlign(MGUI::eAlign::CENTER);

		World::Instance()->MainRenderContext()->SetColorClear(eClearMode::ALL, Float4(0.1f, 0.1f, 0.3f));

		// register function
		LuaRegFunction("hello_lua", hello_lua);

		LuaDoString(lua_script);
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
		delete mLuaHelper;
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
	*ppApp = new Demo05_Lua;
}

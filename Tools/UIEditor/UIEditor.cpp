#include "App.h"
#include "MainFrame.h"
#include "MGUI_Cursor.h"

class Editor : public App
{
protected:
	ConfigFile * mConfig;
	MainFrame * mMainFrame;

	static Editor * Instance() { return (Editor *)App::Instance(); }

public:
	Editor()
	{
	}

	virtual ~Editor()
	{
	}

	virtual void OnSetupResource()
	{
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core/Editor"));

		ResourceManager::Instance()->AddArchive(new FilePathArchive("../UIEditor"));
	}

	virtual void OnInit()
	{
		CharSet::SetCharSet("gb2312");

		MGUI::FontManager::Instance()->Load("Editor.font");

		mConfig = new ConfigFile;
		mConfig->Load("UIEditor.ini");

		rml_node * node = mConfig->first_node("Media");
		while (node != NULL)
		{
			String path = node->value();

			ResourceManager::Instance()->AddArchive(new FilePathArchive(path));

			node = node->next_sibling("Media");
		}

		node = mConfig->first_node("Font");
		while (node != NULL)
		{
			String source = node->value();

			MGUI::FontManager::Instance()->Load(source);

			node = node->next_sibling("Font");
		}

		mMainFrame = new MainFrame();
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
		delete mMainFrame;

		delete mConfig;
	}

	virtual void OnResize(int w, int h)
	{
		mMainFrame->Layout();
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new Editor;
}

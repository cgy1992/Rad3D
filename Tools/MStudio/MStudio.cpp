#pragma once

#include "App.h"
#include "Editor.h"
#include "MainFrame.h"
#include "Plugin.h"

class EditorApp : public App
{
	Editor * mEditor;
	MainFrame * mMainFrame;
	PluginManager mPluginManager;

public:
	EditorApp()
	{
		Root::SetInEditor(true);
	}

	virtual ~EditorApp()
	{
	}

	virtual void OnSetupResource()
	{
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core/Editor"));

		ResourceManager::Instance()->AddArchive(new FilePathArchive("../MStudio"));
	}

	virtual void OnInit()
	{
		MGUI::LookFeelManager::Instance()->Load("Editor.lookfeel");
		MGUI::FontManager::Instance()->Load("Editor.font");

		mEditor = new Editor;
		mMainFrame = new MainFrame;

		// Init Plugins
		FileSystem fs;
		if (!fs.Open("plugins"))
			return ;

		while (fs.NextFile())
		{
			String filename = String("plugins/") + fs.FileName();
			String extName = FileHelper::GetExternName(filename);
			extName.ToLower();

			if (extName != "dll")
				continue;

			if (LoadLibrary(filename.c_str()) == NULL)
			{
				d_log("?: Load plugin '%s' failed", fs.FileName());
			}
		}

		mPluginManager.InitAll();

		Editor::Instance()->E_Init();
	}

	virtual void OnUpdate()
	{
		mEditor->Update();
	}

	virtual void OnShutdown()
	{
		Editor::Instance()->E_Shutdown();

		mPluginManager.ShutdownAll();

		delete mMainFrame;
		delete mEditor;
	}

	virtual void OnResize(int w, int h)
	{
		mMainFrame->Layout();
		mEditor->E_Resize(w, h);
	}

	virtual void OnDragFile(const char * filename)
	{
		if (Editor::Instance()->IsLoaded())
		{
			Editor::Instance()->OnDragFile(filename);
		}
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new EditorApp;
}

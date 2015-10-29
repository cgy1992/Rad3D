#include "App.h"
#include "AllLookFeel.h"
#include "MainMenu.h"
#include "FloderPanel.h"
#include "RenderViewer.h"

class ResourceViewer : public App
{
	AllLookFeel * mAllLookFeel;
	MainMenu * mMenu;
	FloderPanel * mFloderPanel;
	RenderViewer * mRenderViewer;

	int mX, mY, mResolution;
	String mOpenFloder;

public:
	ResourceViewer()
	{
		mX = mY = 3;
		mResolution = 256;
	}

	virtual ~ResourceViewer()
	{
	}

	virtual void OnSetupResource()
	{
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core/Editor"));

		ResourceManager::Instance()->AddArchive(new FilePathArchive("../ResourceViewer"));

		ConfigFile config;
		if (config.Load("ResourceViewer.ini"))
		{
			rml_node * node = config.first_node("Media");
			while (node != NULL)
			{
				String path = node->value();

				ResourceManager::Instance()->AddArchive(new FilePathArchive(path));

				node = node->next_sibling("Media");
			}

			node = config.first_node("OpenFloder");
			if (node != NULL)
			{
				mOpenFloder = node->value();
			}

			node = config.first_node("Display");
			if (node != NULL)
			{
				const char * str = node->value();

				str = str_getint(mX, str);
				str = str_getint(mY, str);
				str = str_getint(mResolution, str);

				mX = Math::Clamp(mX, 2, 8);
				mY = Math::Clamp(mY, 2, 8);
				mResolution = Math::Clamp(mResolution, 64, 256);

				d_assert (mX * mY <= 25);
				if (mX * mY > 25)
				{
					mX = mY = 3;
					mResolution = 256;
				}
			}
		}
	}

	virtual void OnInit()
	{
		CharSet::SetCharSet("gb2312");

		MGUI::FontManager::Instance()->Load("Editor.font");
		MGUI::LookFeelManager::Instance()->Load("Editor.lookfeel");

		mAllLookFeel = new AllLookFeel;
		mMenu = new MainMenu;
		mFloderPanel = new FloderPanel;
		mRenderViewer = new RenderViewer(mX, mY, mResolution);

		if (mOpenFloder != "")
		{
			mFloderPanel->SetFloder(mOpenFloder);
		}

		OnResize(RenderSystem::Instance()->GetConfig().width, RenderSystem::Instance()->GetConfig().height);
	}

	virtual void OnUpdate()
	{
		mMenu->Update();
	}

	virtual void OnShutdown()
	{
		delete mRenderViewer;
		delete mFloderPanel;
		delete mMenu;
		delete mAllLookFeel;
	}

	virtual void OnResize(int w, int h)
	{
		mMenu->Layout();
		mFloderPanel->Layout();
		mRenderViewer->Layout();
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new ResourceViewer;
}

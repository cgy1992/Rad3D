#include "App.h"
#include "MVideoPlayer.h"

class Demo13_Video : public App
{
	VideoPlayer * mVideoPlayer;

public:
	Demo13_Video() {}
	virtual ~Demo13_Video() {}

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

		MGUI::Layout * layout = new MGUI::Layout(NULL, NULL);
		layout->SetAlign(MGUI::eAlign::STRETCH);

		mVideoPlayer = VDO_CreatePlayer("Demo13/konqi.ogg", ePixelFormat::R8G8B8, VIDEO_FLAG_AUDIO | VIDEO_FLAG_LOOPED);
		if (mVideoPlayer != NULL)
		{
			MGUI::ImageBox * img = new MGUI::ImageBox(NULL, layout);
			img->SetAlign(MGUI::eAlign::CENTER);
			img->SetSkinAlignedEx(mVideoPlayer->GetVideoTexture());

			mVideoPlayer->Start();
		}
	}

	virtual void OnUpdate()
	{
		if (mVideoPlayer != NULL)
		{
			mVideoPlayer->Track();
		}
	}

	virtual void OnShutdown()
	{
		safe_delete(mVideoPlayer);
	}

	virtual void OnPause()
	{
		if (mVideoPlayer)
		{
			mVideoPlayer->Pasue();
		}
	}

	virtual void OnResume()
	{
		if (mVideoPlayer)
		{
			mVideoPlayer->Resume();
		}
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new Demo13_Video;
}

#include "App.h"
#include "MVideoPlayer.h"

class Demo13_Sound : public App
{
public:
	Demo13_Sound() {}
	virtual ~Demo13_Sound() {}

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

		MGUI::TextBox * text = new MGUI::TextBox(NULL, layout);
		text->SetAlign(MGUI::eAlign::CENTER);
		text->SetCaption(WSTR_("Press to play sound."));

		AudioSystem::Instance()->PlaySound("Demo13/bgm.OGG", 0, AUDIO_FLAG_LOOPED);

		mSound = ResourceManager::Instance()->OpenResource("Demo13/wuya.wav");

		if (mSound != NULL)
		{
			MGUI::InputManager::Instance()->E_MouseDown += 
				new cListener1<Demo13_Sound, const MGUI::MouseEvent *>(this, &Demo13_Sound::OnMouseDown);

			MGUI::InputManager::Instance()->E_TouchDown += 
				new cListener1<Demo13_Sound, const MGUI::TouchEvent *>(this, &Demo13_Sound::OnTouchDown);
		}
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
		mSound = NULL;
	}

	virtual void OnPause()
	{
	}

	virtual void OnResume()
	{
	}

	void OnMouseDown(const MGUI::MouseEvent *)
	{
		// stream can't shared
		AudioSystem::Instance()->PlaySound(mSound->Clone());
	}

	void OnTouchDown(const MGUI::TouchEvent *)
	{
		AudioSystem::Instance()->PlaySound(mSound->Clone());
	}

private:
	DataStreamPtr mSound;
	VideoPlayer * mVideoPlayer;
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new Demo13_Sound;
}

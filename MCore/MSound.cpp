#include "MSound.h"
#include "MWorld.h"

namespace Rad {

	DF_PROPERTY_BEGIN(Sound)
		DF_PROPERTY_EX(Sound, mFilename, "", "Filename", "PT_Filename sound(.wav)|*.wav|", PT_String)
		DF_PROPERTY(Sound, mVolume, "", "Volume", PT_Float)
		DF_PROPERTY(Sound, mAttenStart, "Attention", "Start", PT_Float)
		DF_PROPERTY(Sound, mAttenEnd, "Attention", "End", PT_Float)
	DF_PROPERTY_END()

	void Sound::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mFilename")
		{
			SetFilename(mFilename);
		}
		else if (p->name == "mVolume")
		{
			SetVolume(mVolume);
		}
		else if (p->name == "mAttenStart" || p->name == "mAttenEnd")
		{
			SetAttenParam(mAttenStart, mAttenEnd);
		}
		else
		{
			Node::OnPropertyChanged(p);
		}
	}

	IObject * Sound::Clone()
	{
		Sound * sound = new Sound;

		sound->Copy(this);

		return sound;
	}

	ImplementRTTI(Sound, Node);

	Sound::Sound()
		: Node("Sound")
	{
		mVolume = 1;
		mAttenStart = 5 * METER_LEN;
		mAttenEnd = 20 * METER_LEN;

		mChannel = -1;
	}

	Sound::~Sound()
	{
		Unload();
	}

	void Sound::SetFilename(const String & filename)
	{
		mFilename = filename;

		SoundSourcePtr source = NULL;
		if (mFilename != "")
		{
			source = AudioSystem::Instance()->LoadSource(filename);
		}

		SetSource(source);
	}

	void Sound::SetSource(SoundSourcePtr source)
	{
		if (mSource != source)
		{
			Unload();

			mSource = source;

			if (mSource != NULL)
			{
				mSource->E_Loaded += L_SoundLoaded(this, &Sound::OnSoundLoaded);
				mSource->E_Unload += L_SoundUnload(this, &Sound::OnSoundUnload);

				if (mSource->GetLoadState() == Resource::LOADED)
				{
					OnSoundLoaded();
				}
			}
		}
	}

	void Sound::SetVolume(float v)
	{
		mVolume = v;

		if (mChannel != -1)
		{
			AudioSystem::Instance()->SetSoundVolume(mChannel, mVolume);
		}
	}

	void Sound::SetAttenParam(float start, float end)
	{
		mAttenStart = start;
		mAttenEnd = end;

		if (mChannel != -1)
		{
			AudioSystem::Instance()->SetSoundAttenParam(mChannel, mAttenStart, mAttenEnd);
		}
	}

	void Sound::Update(float elapsedTime)
	{
		Node::Update(elapsedTime);

		if (mChannel != -1)
		{
			AudioSystem::Instance()->SetSoundPosition(mChannel, mWorldPosition);
		}
	}

	void Sound::Load(float priority)
	{
		if (mSource != NULL && priority < 0)
		{
			mSource->EnsureLoad();
		}

		if (mSource == NULL && mFilename != "")
		{
			SetSource(AudioSystem::Instance()->LoadSource(mFilename, priority));
		}
	}

	void Sound::Unload()
	{
		OnSoundUnload();

		if (mSource != NULL)
		{
			mSource->E_Loaded -= L_SoundLoaded(this, &Sound::OnSoundLoaded);
			mSource->E_Unload -= L_SoundUnload(this, &Sound::OnSoundUnload);
			mSource = NULL;
		}
	}

	void Sound::OnSoundLoaded()
	{
		if (mChannel == -1 && mSource != NULL && mSource->GetDataStream() != NULL)
		{
			mChannel = AudioSystem::Instance()->PlaySound(mSource->GetDataStream(), 0, AUDIO_FLAG_LOOPED | AUDIO_FLAG_3DSOUND);

			if (mChannel != -1)
			{
				AudioSystem::Instance()->SetSoundVolume(mChannel, mVolume);
				AudioSystem::Instance()->SetSoundAttenParam(mChannel, mAttenStart, mAttenEnd);
			}
		}

		E_Loaded(this);
	}

	void Sound::OnSoundUnload()
	{
		E_Unload(this);

		if (mChannel != -1)
		{
			AudioSystem::Instance()->StopSound(mChannel);
			mChannel = -1;
		}
	}
}
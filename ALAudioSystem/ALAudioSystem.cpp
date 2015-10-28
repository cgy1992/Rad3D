#include "ALAudioSystem.h"
#include "ALSound.h"

namespace Rad {

#ifdef M_PLATFORM_WIN32
	static const int al_maxchannel = 32;
#else
	static const int al_maxchannel = 16;
#endif
	static ALSound al_sounds[al_maxchannel];

	int al_pick_sound(int off)
	{
		for (int i = off; i < al_maxchannel; ++i)
		{
			if (al_sounds[i].IsStoped())
				return i;
		}

		return -1;
	}

	void al_stop_all()
	{
		for (int i = 0; i < al_maxchannel; ++i)
		{
			al_sounds[i].Stop();
		}
	}

	//
	ALCcontext * al_context = 0;
	ALCdevice * al_device = 0;

	ALAudioSystem::ALAudioSystem()
	{
		al_device = alcOpenDevice(NULL);

		d_assert (al_device != NULL);

		al_context = alcCreateContext(al_device, 0);

		d_assert (al_context != NULL);

		alcMakeContextCurrent(al_context);

		alListener3f(AL_POSITION, 0, 0, 0);
	}

	ALAudioSystem::~ALAudioSystem()
	{
		for (int i = 0; i < mQueue.Size(); ++i)
			delete mQueue[i];

		al_stop_all();

		alcMakeContextCurrent(0);

		if (al_context)
			alcDestroyContext(al_context);

		if (al_device)
			alcCloseDevice(al_device);

		al_context = 0;
		al_device = 0;
	}

	int ALAudioSystem::PlaySound(IAudioPtr ado, int category, int flags, int channel)
	{
		if (channel != -1 && !al_sounds[channel].IsStoped() && (flags & AUDIO_FLAG_QUEUED))
		{
			al_sounds[channel].FadeOut();

			queued_info * qi = new queued_info;
			qi->audio = ado;
			qi->channel = channel;
			qi->category = category;
			qi->flags = flags & (~(AUDIO_FLAG_QUEUED));

			mQueue.PushBack(qi);

			return channel;
		}

		flags &= ~(AUDIO_FLAG_QUEUED);

		if (channel != -1)
			al_sounds[channel].Stop();
		else
			channel = al_pick_sound(1);

		if (channel == -1)
			return -1;

		if (!al_sounds[channel].Play(ado, category, flags))
			return -1;

		return channel;
	}

	void ALAudioSystem::PauseSound(int channel)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);

		al_sounds[channel].Pause();
	}

	void ALAudioSystem::ResumeSound(int channel)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);
		
		al_sounds[channel].Resume();
	}

	void ALAudioSystem::StopSound(int channel)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);
		
		al_sounds[channel].Stop();
	}

	void ALAudioSystem::SetSoundPosition(int channel, const Float3 & position)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);

		al_sounds[channel].SetPosition(position);
	}

	void ALAudioSystem::SetSoundVolume(int channel, float volume)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);
		
		al_sounds[channel].SetVolume(volume);
	}

	void ALAudioSystem::SetSoundAttenParam(int channel, float start, float end)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);

		al_sounds[channel].SetAttenParam(start, end);
	}

	float ALAudioSystem::GetSoundPlayTime(int channel)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);

		return al_sounds[channel].GetTime();
	}

	void ALAudioSystem::Update(const Float3 & listener)
	{
		for (int i = 0; i < al_maxchannel; ++i)
		{
			if (al_sounds[i].IsStoped())
			{
				al_sounds[i].Stop();
				continue;
			}

			al_sounds[i].Update(listener);
		}

		for (int i = 0; i < mQueue.Size(); ++i)
		{
			queued_info * qi = mQueue[i];
			if (al_sounds[qi->channel].IsStoped())
			{
				PlaySound(qi->audio, qi->category, qi->flags, qi->channel);

				delete qi;
				mQueue.Erase(i--);
			}
		}
	}
}


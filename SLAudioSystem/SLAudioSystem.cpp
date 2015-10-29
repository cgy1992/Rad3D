#include "SLAudioSystem.h"
#include "SLSound.h"

namespace Rad {

#ifdef M_PLATFORM_WIN32
	static const int al_maxchannel = 32;
#else
	static const int al_maxchannel = 16;
#endif
	static SLSound al_sounds[al_maxchannel];

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
	SLObjectItf gEngineObject = NULL;
	SLEngineItf gEngineEngine;

	SLObjectItf gOutputMixObject = NULL;
	SLEnvironmentalReverbItf gOutputMixEnvironmentalReverb = NULL;

	const SLEnvironmentalReverbSettings gReverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

	SLAudioSystem::SLAudioSystem()
	{
		SLresult result;
		result = slCreateEngine(&gEngineObject, 0, NULL, 0, NULL, NULL);
		d_assert(SL_RESULT_SUCCESS == result);

		result = (*gEngineObject)->Realize(gEngineObject, SL_BOOLEAN_FALSE);
		d_assert(SL_RESULT_SUCCESS == result);

		result = (*gEngineObject)->GetInterface(gEngineObject, SL_IID_ENGINE, &gEngineEngine);
		d_assert(SL_RESULT_SUCCESS == result);

		const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
		const SLboolean req[1] = {SL_BOOLEAN_FALSE};
		result = (*gEngineEngine)->CreateOutputMix(gEngineEngine, &gOutputMixObject, 1, ids, req);
		d_assert(SL_RESULT_SUCCESS == result);

		result = (*gOutputMixObject)->Realize(gOutputMixObject, SL_BOOLEAN_FALSE);
		d_assert(SL_RESULT_SUCCESS == result);

		result = (*gOutputMixObject)->GetInterface(gOutputMixObject, SL_IID_ENVIRONMENTALREVERB, &gOutputMixEnvironmentalReverb);
		if (SL_RESULT_SUCCESS == result) {
			result = (*gOutputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
				gOutputMixEnvironmentalReverb, &gReverbSettings);
		}
	}

	SLAudioSystem::~SLAudioSystem()
	{
		if (gOutputMixObject != NULL)
		{
			(*gOutputMixObject)->Destroy(gOutputMixObject);
			gOutputMixObject = NULL;
			gOutputMixEnvironmentalReverb = NULL;
		}

		if (gEngineObject != NULL)
		{
			(*gEngineObject)->Destroy(gEngineObject);
			gEngineObject = NULL;
			gEngineEngine = NULL;
		}
	}

	int SLAudioSystem::PlaySound(IAudioPtr ado, int category, int flags, int channel)
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

	void SLAudioSystem::PauseSound(int channel)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);
		
		al_sounds[channel].Pause();
	}

	void SLAudioSystem::ResumeSound(int channel)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);
		
		al_sounds[channel].Resume();
	}

	void SLAudioSystem::StopSound(int channel)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);
		
		al_sounds[channel].Stop();
	}

	void SLAudioSystem::SetSoundPosition(int channel, const Float3 & position)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);
			
		al_sounds[channel].SetPosition(position);
	}

	void SLAudioSystem::SetSoundVolume(int channel, float volume)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);
		
		al_sounds[channel].SetVolume(volume);
	}

	void SLAudioSystem::SetSoundAttenParam(int channel, float start, float end)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);

		al_sounds[channel].SetAttenParam(start, end);
	}

	float SLAudioSystem::GetSoundPlayTime(int channel)
	{
		d_assert (channel >= 0 && channel < al_maxchannel);

		return al_sounds[channel].GetTime();
	}

	void SLAudioSystem::Update(const Float3 & listener)
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
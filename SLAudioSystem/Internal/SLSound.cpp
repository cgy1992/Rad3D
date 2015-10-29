#include "SLSound.h"
#include "MRoot.h"
#include "MAudioSystem.h"

#define SL_FADETIME 1.5f

SLmillibel SL_VOLUME_TABLE[AUDIO_VOLUME_LEVEL] = {
	-4000, -3600,	-3200,	-2800,
	-2400, -2100,	-1800,	-1500,
	-1200, -1000,	-800,	-600,
	-400,  -200,	-100,	0
};

SLmillibel _getVolumeLevel(float volume)
{
	volume *= (AUDIO_VOLUME_LEVEL - 1);

	int k0 = (int)volume;
	int k1 = k0 < AUDIO_VOLUME_LEVEL - 1 ? (k0 + 1) : k0;
	float t = volume - k0;

	return SL_VOLUME_TABLE[k0] + (SLmillibel)((SL_VOLUME_TABLE[k1] - SL_VOLUME_TABLE[k0]) * t);
}

namespace Rad {

	extern SLObjectItf gEngineObject;
	extern SLEngineItf gEngineEngine;

	extern SLObjectItf gOutputMixObject;
	extern SLEnvironmentalReverbItf gOutputMixEnvironmentalReverb;

	struct SLSoundThread : public Thread
	{
		SLSound * i_sound;

		SLSoundThread(SLSound * sound) : Thread(false) { i_sound = sound; }
		virtual  ~SLSoundThread() {}

		virtual void Run()
		{
			while (mStatus != Thread::STOP)
			{
				i_sound->Track();
			}
		}
	};

	//
	SLSound::SLSound()
		: mCategory(0)
		, mFlags(0)
	{
		mBQObjectI = NULL;
		mBQPlayI = NULL;
		mVolumeI = NULL;
		mBQBufferQueueI = NULL;
		mBQEffectSendI = NULL;

		mPosition = Float3(0, 0, 0);
		mVolume = 1;
		mAttenStart = mAttenEnd = 0;
		mInvAttenDist = 1;

		mPlayTime = 0;
		mPlayOffset = 0;
		mPlayEnd = FALSE;

		mFadeMode = 0;
		mFadeTime = SL_FADETIME;

		i_thread = NULL;
	}

	SLSound::~SLSound()
	{
		Stop();
	}

	bool SLSound::Play(IAudioPtr ado, int category, int flags)
	{
		d_assert (IsStoped() && ado != NULL);

		// configure audio source
#ifdef M_PLATFORM_ANDROID
		SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, NUM_BUFFER };
#else
		SLDataLocator_BufferQueue loc_bufq = { SL_DATALOCATOR_BUFFERQUEUE, NUM_BUFFER };
#endif
		SLDataFormat_PCM format_pcm;
		format_pcm.formatType = SL_DATAFORMAT_PCM;
		format_pcm.numChannels = ado->i_channels;
		format_pcm.samplesPerSec = ado->i_sample_rate * 1000;
		format_pcm.bitsPerSample = ado->i_sample_size * 8;
		format_pcm.containerSize = ado->i_sample_size * 8;
		if(ado->i_channels == 2 )
			format_pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
		else
			format_pcm.channelMask = SL_SPEAKER_FRONT_CENTER;
		format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;

		SLDataSource audioSrc = {&loc_bufq, &format_pcm};

		// configure audio sink
		SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, gOutputMixObject};
		SLDataSink audioSnk = {&loc_outmix, NULL};

		// create audio player
		const SLInterfaceID ids[3] = { SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
		const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
		SLresult result = (*gEngineEngine)->CreateAudioPlayer(gEngineEngine, &mBQObjectI, &audioSrc, &audioSnk, 3, ids, req);
		if (result != SL_RESULT_SUCCESS)
			return false;

		mAudio = ado;
		mCategory = category;
		mFlags = flags;
		mVolume = 1;
		mVolumeAbs = 1;

		if (ado->i_channels == 1)
		{
			mBufferSize = ado->i_sample_rate / 2;
			mBufferSize -= mBufferSize % 2;
		}
		else
		{
			mBufferSize = ado->i_sample_rate;
			mBufferSize -= mBufferSize % 4;
		}
		mBuffer.Resize(NUM_BUFFER * mBufferSize);
		for (int i = 0; i < NUM_BUFFER; ++i)
		{
			mBufferFlag[i] = 0;
		}

		(*mBQObjectI)->Realize(mBQObjectI, SL_BOOLEAN_FALSE);

		(*mBQObjectI)->GetInterface(mBQObjectI, SL_IID_PLAY, &mBQPlayI);
		(*mBQObjectI)->GetInterface(mBQObjectI, SL_IID_BUFFERQUEUE, &mBQBufferQueueI);
		(*mBQObjectI)->GetInterface(mBQObjectI, SL_IID_EFFECTSEND, &mBQEffectSendI);
		(*mBQObjectI)->GetInterface(mBQObjectI, SL_IID_VOLUME,  &mVolumeI);

		Track();

		(*mBQPlayI)->SetPositionUpdatePeriod(mBQPlayI, (SLmillisecond)(ado->i_duration * 1000));
		(*mBQPlayI)->SetPlayState(mBQPlayI, SL_PLAYSTATE_PLAYING);

		if (mFlags & AUDIO_FLAG_MULTI_THREAD)
		{
			i_thread = new SLSoundThread(this);
			i_thread->Start();
		}

		mFadeMode = 0;
		mFadeTime = SL_FADETIME;

		if (mFlags & AUDIO_FLAG_FADEIN)
			FadeIn();

		mPlayTime = 0;
		mPlayOffset = 0;
		mPlayEnd = FALSE;

		return true;
	}

	void SLSound::Track()
	{
		if (IsStoped() || IsPaused() || mAudio == NULL)
			return ;

#ifdef M_PLATFORM_ANDROID
		SLAndroidSimpleBufferQueueState state;
#else
		SLBufferQueueState state;
#endif
		(*mBQBufferQueueI)->GetState(mBQBufferQueueI, &state);

		int processed = mBufferQueue.Size() - state.count;
		if (processed > 0)
		{
			mPlayOffset += mBufferQueue[0].size;
			mBufferFlag[mBufferQueue[0].id] = 0;
			mBufferQueue.Erase(0);

			if (mPlayOffset >= mAudio->i_data_size && (mFlags & AUDIO_FLAG_LOOPED))
				mPlayOffset -= mAudio->i_data_size;
		}

		SLmillisecond millsecond;
		(*mBQPlayI)->GetPosition(mBQPlayI, &millsecond);
		mPlayTime = millsecond / 1000.0f;

		int queued = mBufferQueue.Size();
		while (queued++ < NUM_BUFFER)
		{
			PlayBuffer pb = {0, 0};
			for (int i = 0; i < NUM_BUFFER; ++i)
			{
				if (mBufferFlag[i] == 0)
				{
					pb.id = i;
					break;
				}
			}

			int offset = pb.id * mBufferSize;
			int nreads = mAudio->Read(&mBuffer[offset], mBufferSize);
			if (nreads > 0)
			{
#ifdef M_PLATFORM_ANDROID
				(*mBQBufferQueueI)->Enqueue(mBQBufferQueueI, &mBuffer[offset], nreads);
#else
				(*mBQBufferQueueI)->Enqueue(mBQBufferQueueI, &mBuffer[offset], nreads, false);
#endif
				pb.size = nreads;
				mBufferFlag[pb.id] = 1;
				mBufferQueue.PushBack(pb);
			}
			else if (mFlags & AUDIO_FLAG_LOOPED)
			{
				mAudio->Seek(0);
			}
			else
			{
				mPlayEnd = TRUE;
				break;
			}
		}
	}

	void SLSound::Pause()
	{
		if (IsStoped() || IsPaused())
			return ;

		(*mBQPlayI)->SetPlayState(mBQPlayI, SL_PLAYSTATE_PAUSED);
	}

	void SLSound::Resume()
	{
		if (IsStoped() || !IsPaused())
			return ;

		(*mBQPlayI)->SetPlayState(mBQPlayI, SL_PLAYSTATE_PLAYING);
	}

	void SLSound::Stop()
	{
		if (IsStoped())
			return ;

		safe_delete(i_thread);

		(*mBQPlayI)->SetPlayState(mBQPlayI, SL_PLAYSTATE_STOPPED);

		(*mBQObjectI)->Destroy(mBQObjectI);
		mBQObjectI = NULL;
		mBQPlayI = NULL;
		mBQBufferQueueI = NULL;
		mBQEffectSendI = NULL;
		mVolumeI = NULL;

		mBuffer.Clear();
		mBufferQueue.Clear();

		mPosition = Float3(0, 0, 0);
		mVolume = 1;
		mAttenStart = mAttenEnd = 0;
		mInvAttenDist = 1;

		mAudio = NULL;
		mCategory = 0;
		mFlags = 0;

		mFadeMode = 0;
		mFadeTime = SL_FADETIME;
	}

	bool SLSound::IsTimeOut()
	{
		d_assert (!IsStoped());

		return mPlayEnd == TRUE && mBufferQueue.Size() == 0;
	}

	bool SLSound::IsPaused()
	{
		d_assert (!IsStoped());

		SLuint32 state = 0;
		(*mBQPlayI)->GetPlayState(mBQPlayI, &state);

		return state == SL_PLAYSTATE_PAUSED;
	}

	void SLSound::SetPosition(const Float3 & position)
	{
		mPosition = position;
	}

	void SLSound::SetVolume(float volume)
	{
		mVolume = volume;
	}

	void SLSound::SetAttenParam(float start, float end)
	{
		mAttenStart = start;
		mAttenEnd = end;

		if (mAttenEnd < mAttenStart)
			mAttenEnd = mAttenStart;

		if (mAttenStart == mAttenEnd)
			mInvAttenDist = 1;
		else
			mInvAttenDist = 1 / (mAttenEnd - mAttenStart);
	}

	void SLSound::Update(const Float3 & listener)
	{
		float vol = mVolume * AudioSystem::Instance()->GetGlobalVolume(mCategory);

		if (mFlags & AUDIO_FLAG_3DSOUND)
		{
			Float3 d = mPosition - listener;
			float len = d.Length();

			if (len > mAttenStart)
			{
				float k = (len - mAttenStart) * mInvAttenDist;
				k = 1.0f - Math::Clamp(k, 0.0f, 1.0f);
				vol *= k;
			}
		}

		float fade = 1;

		if (mFadeMode == FADE_IN)
			mFadeTime += Root::Instance()->GetFrameTime();
		else if (mFadeMode == FADE_OUT)
			mFadeTime -= Root::Instance()->GetFrameTime();

		mFadeTime = Math::Clamp(mFadeTime, 0.0f, SL_FADETIME);
		fade = mFadeTime / SL_FADETIME;
		vol *= fade;

		if (!Math::Equal(mVolumeAbs, vol))
		{
			SLmillibel volume = _getVolumeLevel(vol);

			(*mVolumeI)->SetVolumeLevel(mVolumeI, volume);
			(*mVolumeI)->SetMute(mVolumeI, vol > 0 ? FALSE : TRUE);

			mVolumeAbs = vol;
		}

		if (fade == 0 && mFadeMode == FADE_OUT)
			Stop();
		else if (IsTimeOut())
			Stop();
		else if (i_thread == NULL)
			Track();
	}

	void SLSound::FadeIn()
	{
		if (mFadeMode != FADE_IN)
		{
			mFadeMode = FADE_IN;
			mFadeTime = 0;
		}
	}

	void SLSound::FadeOut()
	{
		if (mFadeMode != FADE_OUT)
		{
			mFadeMode = FADE_OUT;
			mFadeTime = SL_FADETIME;
		}
	}
}



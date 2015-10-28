#include "ALSound.h"
#include "MRoot.h"
#include "MAudioSystem.h"

#define AL_FADETIME 1.5f

namespace Rad {

	inline ALint _getALFormat(int num_chanel, int sample_size)
	{
		if (num_chanel == 1)
		{
			if (sample_size == 1)
				return AL_FORMAT_MONO8;
			else if (sample_size == 2)
				return AL_FORMAT_MONO16;
		}
		else if (num_chanel == 2)
		{
			if (sample_size == 1)
				return AL_FORMAT_STEREO8;
			else if (sample_size == 2)
				return AL_FORMAT_STEREO16;
		}

		return 0;
	}

	struct ALSoundThread : public Thread
	{
		ALSound * i_sound;

		ALSoundThread(ALSound * sound) : Thread(false) { i_sound = sound; }
		virtual  ~ALSoundThread() {}

		virtual void Run()
		{
			while (mStatus != Thread::STOP)
				i_sound->Track();
		}
	};

	//
	ALSound::ALSound()
		: mCategory(0)
		, mFlags(0)
		, mALSourceId(0)
		, mALFormat(0)
	{
		mPosition = Float3(0, 0, 0);
		mVolume = 1;
		mAttenStart = mAttenEnd = 0;
		mInvAttenDist = 1;

		mFadeMode = 0;
		mFadeTime = AL_FADETIME;

		mPlayTime = 0;
		mPlayOffset = 0;
		mPlayEnd = FALSE;

		i_thread = NULL;
	}

	ALSound::~ALSound()
	{
		Stop();
	}

	bool ALSound::Play(IAudioPtr ado, int category, int flags)
	{
		d_assert (IsStoped() && ado != NULL);

		mALFormat = _getALFormat(ado->i_channels, ado->i_sample_size);
		if (mALFormat == 0)
			return false;

		mAudio = ado;
		mCategory = category;
		mFlags = flags;
		mVolumeAbs = 1;

		int size = 0;
		if (ado->i_channels == 1)
		{
			size = ado->i_sample_rate / 2;
			size -= size % 2;
		}
		else
		{
			size = ado->i_sample_rate;
			size -= size % 4;
		}
		mBuffer.Resize(size);

		alGenSources(1, &mALSourceId);
		alSourcei(mALSourceId, AL_SOURCE_RELATIVE, AL_TRUE);
		alSource3f(mALSourceId, AL_POSITION, 0, 0, 0);
		alSource3f(mALSourceId, AL_VELOCITY, 0, 0, 0);
		alSourcef(mALSourceId, AL_REFERENCE_DISTANCE, 1.0);
		alSourcef(mALSourceId, AL_MAX_DISTANCE, 1.0);
		alSourcef(mALSourceId, AL_ROLLOFF_FACTOR, 0.0);
		alSourcei(mALSourceId, AL_LOOPING, AL_FALSE);

		Track();

		if (mFlags & AUDIO_FLAG_MULTI_THREAD)
		{
			i_thread = new ALSoundThread(this);
			i_thread->Start();
		}

		alSourcePlay(mALSourceId);

		mFadeMode = 0;
		mFadeTime = AL_FADETIME;

		if (mFlags & AUDIO_FLAG_FADEIN)
			FadeIn();

		mTimePerBytes = mAudio->i_duration / mAudio->i_data_size;
		mPlayTime = 0;
		mPlayOffset = 0;
		mPlayEnd = FALSE;

		return true;
	}

	void ALSound::Track()
	{
		if (IsStoped() || IsPaused() || mAudio == NULL)
			return ;

		// remove processed
		int processed;
		alGetSourcei(mALSourceId, AL_BUFFERS_PROCESSED, &processed);

		if (processed > 0)
		{
			ALuint bufferId;
			alSourceUnqueueBuffers(mALSourceId, 1, &bufferId);
			alDeleteBuffers(1, &bufferId);

			mPlayOffset += mBufferQueue[0].size;
			mBufferQueue.Erase(0);

			if (mPlayOffset >= mAudio->i_data_size && (mFlags & AUDIO_FLAG_LOOPED))
				mPlayOffset -= mAudio->i_data_size;
		}

		// update play time
		int processed_bytes;
		alGetSourcei(mALSourceId, AL_BYTE_OFFSET, &processed_bytes);

		mPlayTime = (mPlayOffset + processed_bytes) * mTimePerBytes;

		// update queue
		int queued = 0;
		alGetSourcei (mALSourceId, AL_BUFFERS_QUEUED, &queued);

		while (queued++ < NUM_BUFFER)
		{
			int nreads = mAudio->Read(&mBuffer[0], mBuffer.Size());
			if (nreads > 0)
			{
				PlayBuffer pb; 
				pb.id = 0;
				pb.size = nreads;

				alGenBuffers(1, &pb.id);
				alBufferData(pb.id, mALFormat, &mBuffer[0], nreads, mAudio->i_sample_rate);
				alSourceQueueBuffers(mALSourceId, 1, &pb.id);

				// ! it reset AL_BYTE_OFFSET
				int state = 0;
				alGetSourcei(mALSourceId, AL_SOURCE_STATE, &state);
				if (state == AL_STOPPED)
					alSourcePlay(mALSourceId);

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

	void ALSound::Pause()
	{
		if (IsStoped() || IsPaused())
			return ;

		alSourcePause(mALSourceId);
	}

	void ALSound::Resume()
	{
		if (IsStoped() || !IsPaused())
			return ;

		alSourcePlay(mALSourceId);
	}

	void ALSound::Stop()
	{
		if (IsStoped())
			return ;

		safe_delete(i_thread);

		alSourceStop(mALSourceId);

		int processed;
		alGetSourcei(mALSourceId, AL_BUFFERS_PROCESSED, &processed);

		while (processed > 0)
		{
			unsigned int buffer;
			alSourceUnqueueBuffers(mALSourceId, 1, &buffer);
			alDeleteBuffers(1, &buffer);

			processed -= 1;
		}

		alDeleteSources(1, &mALSourceId);

		mALSourceId = 0;
		mALFormat = 0;

		mPosition = Float3(0, 0, 0);
		mVolume = 1;
		mAttenStart = mAttenEnd = 0;
		mInvAttenDist = 1;

		mAudio = NULL;
		mCategory = 0;
		mFlags = 0;

		mPlayOffset = 0;
		mPlayTime = 0;

		mBuffer.Clear();
		mBufferQueue.Clear();

		mFadeMode = 0;
		mFadeTime = AL_FADETIME;
	}

	bool ALSound::IsTimeOut()
	{
		d_assert (!IsStoped());

		return mPlayEnd == TRUE && mBufferQueue.Size() == 0;
	}

	bool ALSound::IsPaused()
	{
		d_assert (!IsStoped());

		int state = 0;
		alGetSourcei(mALSourceId, AL_SOURCE_STATE, &state);

		return state == AL_PAUSED;
	}

	void ALSound::SetPosition(const Float3 & position)
	{
		mPosition = position;
	}

	void ALSound::SetVolume(float volume)
	{
		mVolume = volume;
	}

	void ALSound::SetAttenParam(float start, float end)
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

	void ALSound::Update(const Float3 & listener)
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

		mFadeTime = Math::Clamp(mFadeTime, 0.0f, AL_FADETIME);
		fade = mFadeTime / AL_FADETIME;

		vol *= fade;
		if (!Math::Equal(mVolumeAbs, vol))
		{
			mVolumeAbs = vol;
			alSourcef(mALSourceId, AL_GAIN, vol);
		}

		if (fade == 0 && mFadeMode == FADE_OUT)
			Stop();
		else if (IsTimeOut())
			Stop();
		else if (i_thread == NULL)
			Track();
	}

	void ALSound::FadeIn()
	{
		if (mFadeMode != FADE_IN)
		{
			mFadeMode = FADE_IN;
			mFadeTime = 0;
		}
	}

	void ALSound::FadeOut()
	{
		if (mFadeMode != FADE_OUT)
		{
			mFadeMode = FADE_OUT;
			mFadeTime = AL_FADETIME;
		}
	}
}



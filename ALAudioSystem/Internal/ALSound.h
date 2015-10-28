/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include <al.h>
#include <alc.h>

#include "MMath2.h"
#include "MAudio.h"
#include "MArray.h"
#include "MThread.h"

namespace Rad {

#define NUM_BUFFER 2

	class ALSound
	{
	public:
		enum {
			FADE_IN = 1,
			FADE_OUT,
		};


		struct PlayBuffer
		{
			ALuint id;
			int size;
		};

	public:
		ALSound();
		~ALSound();

		bool 
			Play(IAudioPtr ado, int category, int flags);
		void 
			Pause();
		void
			Resume();
		void 
			Stop();
		void 
			Track();
		bool
			IsTimeOut();
		bool
			IsPaused();
		bool 
			IsStoped() { return mALSourceId == 0; }
		int
			GetFlags() { return mFlags; }
		float
			GetTime() { return mPlayTime; }

		void 
			SetPosition(const Float3 & position);
		void 
			SetVolume(float volume);
		void 
			SetAttenParam(float start, float end);

		void 
			Update(const Float3 & listener);

		void 
			FadeIn();
		void 
			FadeOut();

	protected:
		IAudioPtr mAudio;
		int mCategory;
		int mFlags;
		float mVolumeAbs;

		ALuint mALSourceId;
		ALuint mALFormat;

		Float3 mPosition;
		float mVolume;
		float mAttenStart;
		float mAttenEnd;
		float mInvAttenDist;

		float mTimePerBytes;
		float mPlayTime;
		int mPlayOffset;
		int mPlayEnd;

		float mFadeTime;
		int mFadeMode;

		Array<byte> mBuffer;
		FixedArray<PlayBuffer, NUM_BUFFER> mBufferQueue;

		Thread * i_thread;
	};
}


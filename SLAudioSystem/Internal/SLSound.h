/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information LTD
*/
#pragma once

#include "MPlatform.h"

#ifdef M_PLATFORM_WIN32
#include <OpenSLES.h>
#include <OpenSLES_Platform.h>
#endif

#ifdef M_PLATFORM_ANDROID
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#endif

#include "MMath2.h"
#include "MAudio.h"
#include "MField.h"
#include "MArray.h"
#include "MThread.h"

#define NUM_BUFFER 4

namespace Rad {

	class SLSound
	{
	public:
		enum {
			FADE_IN = 1,
			FADE_OUT,
		};

		struct PlayBuffer
		{
			int id;
			int size;
		};

	public:
		SLSound();
		~SLSound();

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
			IsStoped() { return mBQObjectI == 0; }
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

		SLObjectItf mBQObjectI;
		SLPlayItf mBQPlayI;
		SLVolumeItf mVolumeI;
#ifdef M_PLATFORM_ANDROID
		SLAndroidSimpleBufferQueueItf mBQBufferQueueI;
#else
		SLBufferQueueItf mBQBufferQueueI;
#endif
		SLEffectSendItf mBQEffectSendI;

		Float3 mPosition;
		float mVolume;
		float mAttenStart;
		float mAttenEnd;
		float mInvAttenDist;

		float mPlayTime;
		int mPlayOffset;
		int mPlayEnd;

		float mFadeTime;
		int mFadeMode;

		int mBufferSize;
		Array<byte> mBuffer;
		Field<int, NUM_BUFFER> mBufferFlag;
		FixedArray<PlayBuffer, NUM_BUFFER> mBufferQueue;

		Thread * i_thread;
	};
}
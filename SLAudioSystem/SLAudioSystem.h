/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information LTD
*/
#pragma once

#include "MAudioSystem.h"
#include "MThread.h"

#ifdef M_PLATFORM_WIN32

#ifdef SL_EXPORT
#define SL_ENTRY __declspec(dllexport)
#else
#define SL_ENTRY __declspec(dllimport)
#endif

#else

#define SL_ENTRY

#endif

namespace Rad {

	class SL_ENTRY SLAudioSystem : public AudioSystem
	{
		struct queued_info
		{
			DECLARE_POOL_ALLOC();

			IAudioPtr audio;
			int channel;
			int category;
			int flags;
		};

	public:
		SLAudioSystem();
		virtual ~SLAudioSystem();

		virtual int 
			PlaySound(IAudioPtr ado, int category, int flags, int channel);
		virtual void
			PauseSound(int channel);
		virtual void
			ResumeSound(int channel);
		virtual void 
			StopSound(int channel);
		virtual void 
			SetSoundPosition(int channel, const Float3 & position);
		virtual void 
			SetSoundVolume(int channel, float volume);
		virtual void 
			SetSoundAttenParam(int channel, float start, float end);

		virtual float
			GetSoundPlayTime(int channel);

		virtual void
			Update(const Float3 & listener);

	protected:
		Array<queued_info *> mQueue;
	};
}

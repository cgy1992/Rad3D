/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAudioSystem.h"

#ifdef M_PLATFORM_WIN32

	#ifdef AL_EXPORT
		#define AL_ENTRY __declspec(dllexport)
	#else
		#define AL_ENTRY __declspec(dllimport)
	#endif

#else

	#define AL_ENTRY

#endif

namespace Rad {

	class AL_ENTRY ALAudioSystem : public AudioSystem
	{
	public:
		struct queued_info
		{
			DECLARE_POOL_ALLOC();

			IAudioPtr audio;
			int channel;
			int category;
			int flags;
		};

	public:
		ALAudioSystem();
		virtual ~ALAudioSystem();

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

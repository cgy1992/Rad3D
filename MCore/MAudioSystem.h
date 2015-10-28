/*
*	AudioSystem
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSingleton.h"
#include "MField.h"
#include "MMap.h"
#include "MMath2.h"
#include "MResource.h"
#include "MAudio.h"

namespace Rad {

	class M_ENTRY SoundSource : public ResourceDataStream
	{
	public:
		SoundSource(const String & name, const String & source);
		virtual ~SoundSource();

		virtual void 
			Release();
	};

	typedef SmartPtr<SoundSource> SoundSourcePtr;

	//
#define AUDIO_FLAG_LOOPED	0x01
#define AUDIO_FLAG_3DSOUND	0x02
#define AUDIO_FLAG_QUEUED	0x04
#define AUDIO_FLAG_FADEIN	0x08
#define AUDIO_FLAG_MULTI_THREAD	0x20

#define AUDIO_VOLUME_LEVEL	16

	class M_ENTRY AudioSystem : public Singleton<AudioSystem>
	{
	public:
		AudioSystem();
		virtual ~AudioSystem();

		SoundSourcePtr 
			LoadSource(const String & filename, float priority = 0);
		SoundSourcePtr 
			GetSource(const String & filename);
		void 
			_delSource(SoundSource * p);

		void 
			SetGlobalVolume(int category, float volume) { mGlobalVolumes[category] = volume; }
		float 
			GetGlobalVolume(int category) { return mGlobalVolumes[category]; }

		// stream can't shared any other sound, if you need, call stream->Clone()
		//
		int 
			PlaySound(const String & filename, int category = 0, int flag = 0, int channel = -1);
		int 
			PlaySound(DataStreamPtr stream, int category = 0, int flag = 0, int channel = -1);
		virtual int 
			PlaySound(IAudioPtr ado, int category = 0, int flag = 0, int channel = -1) = 0;
		virtual void
			PauseSound(int channel) = 0;
		virtual void
			ResumeSound(int channel) = 0;
		virtual void 
			StopSound(int channel) = 0;

		virtual void 
			SetSoundPosition(int channel, const Float3 & position) = 0;
		virtual void 
			SetSoundVolume(int channel, float volume) = 0;
		virtual void 
			SetSoundAttenParam(int channel, float start, float end) = 0;

		virtual float
			GetSoundPlayTime(int channel) = 0;

		virtual void
			Update(const Float3 & listener) = 0;

	protected:
		SoundSourcePtr
			_find(Hash2 hash, const String & filename);

	protected:
		Map<Hash2, SoundSource *> mSourceMap;
		Field<float, 8> mGlobalVolumes;
	};

}

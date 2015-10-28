/*
*	PSManager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMap.h"
#include "MSingleton.h"
#include "MParticleSystem.h"

namespace Rad {

	class M_ENTRY PS_Manager : public Singleton<PS_Manager>
	{
		struct PlayInfo {
			float duration;
			ParticleSystem * p;
		};
		
	public:
		typedef float (*WorldHeightFunction)(float x, float y, float z);
		typedef bool (*WorldCollideFunction)(Float3 * normal, Particle * p, PS_Set * render);

		static WorldHeightFunction d_get_height;
		static WorldCollideFunction d_collide;

	public:
		PS_Manager();
		~PS_Manager();

		void
			Play(const char * filename, const Float3 & position, float duration);

		PS_SourcePtr 
			GetSource(const String & filename);
		PS_SourcePtr 
			NewSource(const String & name);
		PS_SourcePtr 
			LoadSource(const String & filename, float priority = 0);
		void 
			_delSource(PS_Source * p);

	protected:
		PS_SourcePtr
			_find(Hash2 hash, const String & name);

	protected:
		void 
			OnInit();
		void 
			OnShutdown();
		void 
			OnUpdate(float frameTime);

	protected:
		Map<Hash2, PS_Source *> mSourceMap;

		Array<PlayInfo> mPlayInfos;
	};
}
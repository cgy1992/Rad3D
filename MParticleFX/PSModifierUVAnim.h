/*
*	Modifier UVAnim
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_ModifierUVAnim : public PS_Modifier
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Modifier);

	public:
		PS_ModifierUVAnim();
		virtual ~PS_ModifierUVAnim();

		void
			SetInterval(float r) { mInterval = r; }
		float
			GetInterval() { return mInterval; }

		void
			SetParamIndex(int index) { mParamIndex = index; }
		int
			GetParamIndex() { return Min(mParamIndex, PS_PARTICLE_PARAM_SIZE - 1); }

		void
			SetLooped(bool loop) { mLooped = loop; }
		bool
			IsLooped() { return mLooped; }

		void
			SetRandom(bool b) { mRandom = b; }
		bool
			IsRandom() { return mRandom; }

		void
			SetRandomParamIndex(int index) { mRandomParamIndex = index;}
		int
			GetRandomParamIndex() { return Min(mRandomParamIndex, PS_PARTICLE_PARAM_SIZE - 1); }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		int mParamIndex;
		float mInterval;
		bool mLooped;

		bool mRandom;
		int mRandomParamIndex;
	};

}
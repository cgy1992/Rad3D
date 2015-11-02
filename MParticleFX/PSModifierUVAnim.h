/*
*	Modifier UVAnim
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSModifierKeyController.h"

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


	class FX_ENTRY PS_ModifierUVAnimKF : public PS_ModifierKeyController
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_ModifierKeyController);

	public:
		PS_ModifierUVAnimKF();
		virtual ~PS_ModifierUVAnimKF();

		IKeyController *
			GetKeyController() { return &mKeyController; }

		void
			SetInterpolation(bool b) { mInterpolation = b; }
		bool
			GetInterpolation() const { return mInterpolation; }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		bool mInterpolation;
		KeyControllerFloat4 mKeyController;
	};
}
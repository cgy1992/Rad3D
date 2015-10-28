/*
*	Modifier UVScroll
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_ModifierUVScroll : public PS_Modifier
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Modifier);

	public:
		PS_ModifierUVScroll();
		virtual ~PS_ModifierUVScroll();

		virtual void 
			Modify(Particle * p, float elapsedTime);

		void
			SetURate(float u) { mURate = u; }
		float
			GetURate() { return mURate; }

		void
			SetVRate(float v) { mVRate = v; }
		float
			GetVRate() { return mVRate; }

	public:
		float mURate;
		float mVRate;
	};

	class FX_ENTRY PS_ModifierUVScroll2 : public PS_Modifier
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Modifier);

	public:
		PS_ModifierUVScroll2();
		virtual ~PS_ModifierUVScroll2();

		virtual void 
			Modify(Particle * p, float elapsedTime);

		void
			SetURate(float u) { mURate = u; }
		float
			GetURate() { return mURate; }

		void
			SetVRate(float v) { mVRate = v; }
		float
			GetVRate() { return mVRate; }

		void
			SetTime2(float t) { mTime2 = t; }
		float
			GetTime2() { return mTime2; }

		void
			SetURate2(float u) { mURate2 = u; }
		float
			GetURate2() { return mURate2; }

		void
			SetVRate2(float v) { mVRate2 = v; }
		float
			GetVRate2() { return mVRate2; }

	public:
		float mURate;
		float mVRate;

		float mTime2;
		float mURate2;
		float mVRate2;
	};

}
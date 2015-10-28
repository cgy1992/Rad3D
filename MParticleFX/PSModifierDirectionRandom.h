/*
*	Modifier Direction Random
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_ModifierDirectionRandom : public PS_Modifier
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Modifier);

	public:
		PS_ModifierDirectionRandom();
		virtual ~PS_ModifierDirectionRandom();

		void 
			SetRandomness(float v);
		float 
			GetRandomness() const { return mRandomness; }

		void 
			SetScope(float v);
		float 
			GetScope() const { return mScope; }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		float mRandomness;
		float mScope;
	};

}
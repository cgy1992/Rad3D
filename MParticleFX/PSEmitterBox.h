/*
*	PS_EmitterBox
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "PSEmitterPoint.h"

namespace Rad {

	class FX_ENTRY PS_EmitterBox : public PS_EmitterPoint
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_EmitterPoint);

	public:
		PS_EmitterBox();
		virtual ~PS_EmitterBox();

		void 
			SetExtend(const Float3 & ext);
		const Float3 & 
			GetExtend() const { return mExtend; }

		virtual void 
			InitParticle(Particle * p);

	protected:
		Float3 mExtend;
	};
}
/*
*	PS_Point
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "PSEmitter.h"

namespace Rad {

	class FX_ENTRY PS_EmitterPoint : public PS_Emitter
	{
		DECLARE_RTTI();

	public:
		PS_EmitterPoint();
		virtual ~PS_EmitterPoint();

		virtual void 
			InitParticle(Particle * p);
	};

}
/*
*	PS_PartcileSystem
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "MParticleSystem.h"

namespace Rad {

	class PS_ParticleSystemSet;

	class FX_ENTRY PS_ParticleSystem
	{
		DECLARE_POOL_ALLOC();

	public:
		PS_ParticleSystem(PS_ParticleSystemSet * parent);
		virtual ~PS_ParticleSystem();

		void 
			_update(Particle * p);

		const Aabb &
			_getBound() { return mParticleSystem->GetWorldAabb(); }

	protected:
		PS_ParticleSystemSet * mParent;
		ParticleSystem * mParticleSystem;
	};

}
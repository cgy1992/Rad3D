/*
*	PS_Group
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"

namespace Rad {

	class PS_GroupSet;

	class FX_ENTRY PS_Group
	{
		DECLARE_POOL_ALLOC();

	public:
		PS_Group(PS_GroupSet * parent);
		virtual ~PS_Group();

		void 
			_update(Particle * p);

	protected:
		PS_GroupSet * mParent;
		ParticleSystem * mParticleSystem;
	};

}
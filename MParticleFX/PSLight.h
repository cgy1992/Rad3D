/*
*	PS_Light
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "MLight.h"

namespace Rad {

	class PS_LightSet;

	class FX_ENTRY PS_Light
	{
		DECLARE_POOL_ALLOC();

	public:
		PS_Light(PS_LightSet * parent);
		virtual ~PS_Light();

		void 
			_update(Particle * p);

		const Aabb &
			_getBound() { return mLight->GetWorldAabb(); }

	protected:
		PS_LightSet * mParent;
		Light * mLight;
	};

}
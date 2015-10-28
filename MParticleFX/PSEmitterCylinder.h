/*
*	PS_Cylinder
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "PSEmitterBox.h"

namespace Rad {

	class FX_ENTRY PS_EmitterCylinder : public PS_EmitterBox
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_EmitterBox);

	public:
		PS_EmitterCylinder();
		virtual ~PS_EmitterCylinder();

		virtual void 
			InitParticle(Particle * p);

		void 
			SetInnerScale(float inner);
		float 
			GetInnerScale() const { return mInnerScale; }

	protected:
		float mInnerScale;
	};

}
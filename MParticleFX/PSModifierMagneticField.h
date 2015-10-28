/*
*	Modifier Magnetic Field
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_ModifierMagneticField : public PS_Modifier
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Modifier);

	public:
		PS_ModifierMagneticField();
		virtual ~PS_ModifierMagneticField();

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		Float3 mCenter;
		float mRadius;
		float mAttraction;
		int mOperation;
		float mInnerLife;
	};

}
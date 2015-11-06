/*
*	Modifier Magnetic Field
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSModifierKeyController.h"

namespace Rad {

	class FX_ENTRY PS_ModifierMagneticField : public PS_ModifierKeyController
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Modifier);

	public:
		PS_ModifierMagneticField();
		virtual ~PS_ModifierMagneticField();

		virtual IKeyController * 
			GetKeyController() { return &mKeyController; }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		Float3 mCenter;
		float mRadius;
		float mInnerLife;
		int mOperation;
		KeyControllerFloat mKeyController;
	};

}
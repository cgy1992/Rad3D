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

		void
			SetCenter(const Float3 & center) { mCenter = center; }
		const Float3 &
			GetCenter() const { return mCenter; }

		void
			SetRadius(float radius) { mRadius = radius; }
		float
			GetRadius() const { return mRadius; }

		void
			SetInnerLife(float life) { mInnerLife = life; }
		float
			GetInnerLife() const { return mInnerLife; }

		void
			SetOperation(int op) { mOperation = op; }
		int
			GetOperation() const { return mOperation; }

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
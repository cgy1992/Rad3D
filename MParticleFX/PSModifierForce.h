/*
*	Modifier Force
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSModifierKeyController.h"

namespace Rad {

	class FX_ENTRY PS_ModifierForce : public PS_ModifierKeyController
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_ModifierKeyController);

	public:
		PS_ModifierForce();
		virtual ~PS_ModifierForce();

		IKeyController * 
			GetKeyController() { return &mKeyController; }

		void 
			SetOperation(int type) { mOperation = type; }
		int 
			GetOperation() const { return mOperation; }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		int mOperation;
		KeyControllerFloat3 mKeyController;
	};

}
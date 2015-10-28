/*
*	Modifier Rotation
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSModifierKeyController.h"

namespace Rad {

	class FX_ENTRY PS_ModifierRotation : public PS_ModifierKeyController
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_ModifierKeyController);

	public:
		PS_ModifierRotation();
		virtual ~PS_ModifierRotation();

		IKeyController *
			GetKeyController() { return &mKeyController; }

		void 
			SetOperation(int op) { mOperation = op; }
		int 
			GetOperation(int op) { return mOperation; }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		int mOperation;
		KeyControllerFloat3 mKeyController;
	};

}
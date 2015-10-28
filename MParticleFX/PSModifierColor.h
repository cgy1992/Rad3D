/*
*	Modifier Color
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSModifierKeyController.h"

namespace Rad {

	class FX_ENTRY PS_ModifierColor : public PS_ModifierKeyController
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_ModifierKeyController);

	public:
		PS_ModifierColor();
		virtual ~PS_ModifierColor();

		void 
			SetOperation(int op) { mOperation = op; }
		int 
			GetOperation(int op) { return mOperation; }

		IKeyController * 
			GetKeyController() { return &mKeyController; }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		int mOperation;
		KeyControllerColor4 mKeyController;
	};

}
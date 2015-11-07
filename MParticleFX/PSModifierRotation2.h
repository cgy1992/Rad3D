/*
*	Modifier Rotation2
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSModifierKeyController.h"

namespace Rad {

	class FX_ENTRY PS_ModifierRotation2 : public PS_ModifierKeyController
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Modifier);

	public:
		PS_ModifierRotation2();
		virtual ~PS_ModifierRotation2();

		virtual IKeyController * 
			GetKeyController() { return &mKeyController; }

		void
			SetRotationAble(bool b);
		bool
			IsRotationAble() { return mRotationAble; }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		KeyControllerFloat3 mKeyController;
		bool mRotationAble;
	};
}
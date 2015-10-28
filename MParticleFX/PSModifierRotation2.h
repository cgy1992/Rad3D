/*
*	Modifier Rotation2
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_ModifierRotation2 : public PS_Modifier
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Modifier);

	public:
		PS_ModifierRotation2();
		virtual ~PS_ModifierRotation2();

		void 
			SetRotationSpeed(const Float3 & speed);
		const Float3 &
			GetRotationSpeed() { return mRotationSpeed; }

		void
			SetRotationAble(bool b);
		bool
			IsRotationAble() { return mRotationAble; }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		Float3 mRotationSpeed;
		bool mRotationAble;
	};

}
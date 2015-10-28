/*
*	PSModifier
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MObject.h"
#include "PSParticle.h"

namespace Rad {

	class PS_Set;
	class ParticleSystem;

	class M_ENTRY PS_Modifier : public IObject
	{
		DECLARE_RTTI();
		DECLARE_POOL_ALLOC();
		DECLARE_PROPERTY(IObject)

	public:
		PS_Modifier();
		virtual ~PS_Modifier();

		virtual void
			Init(PS_Set * parent);

		PS_Set * 
			GetParent() { return mParent; }

		void
			SetEnable(bool enable);
		bool
			IsEnable();

		virtual void 
			Modify(Particle * p, float elapsedTime) = 0;

	protected:
		PS_Set * mParent;
		bool mEnable;
	};

}

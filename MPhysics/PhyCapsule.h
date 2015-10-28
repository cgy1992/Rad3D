/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntity.h"

namespace Rad {

	class PHY_ENTRY PhyCapsule : public PhyEntity
	{
		DECLARE_RTTI();

	public:
		PhyCapsule(eType type);
		virtual ~PhyCapsule();

		void 
			SetCapsuleParam(float radius, float length);
		void 
			GetCapsuleParam(float & radius, float & length);

		void 
			SetMass(float mass, int direction, float radius, float length);

	protected:
	};

}
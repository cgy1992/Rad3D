/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntity.h"

namespace Rad {

	class PHY_ENTRY PhySphere : public PhyEntity
	{
		DECLARE_RTTI();

	public:
		PhySphere(eType type);
		virtual ~PhySphere();

		void 
			SetRadius(float radius);
		float 
			GetRadius();

		void 
			SetMass(float mass, float radius);

	protected:
	};

}
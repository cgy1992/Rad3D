/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntity.h"

namespace Rad {

	class PHY_ENTRY PhyCylinder : public PhyEntity
	{
		DECLARE_RTTI();

	public:
		PhyCylinder(eType type);
		virtual ~PhyCylinder();

		void 
			SetCylinderParam(float radius, float length);
		void 
			GetCylinderParam(float & radius, float & length);

		void 
			SetMass(float mass, int direction, float radius, float length);

	protected:
	};

}
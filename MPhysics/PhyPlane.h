/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntity.h"

namespace Rad {

	class PHY_ENTRY PhyPlane : public PhyEntity
	{
		DECLARE_RTTI();

	public:
		PhyPlane();
		virtual ~PhyPlane();

		void 
			SetPlane(float a, float b, float c, float d);
		void 
			GetPlane(float & a, float & b, float & c, float & d);
	};
}
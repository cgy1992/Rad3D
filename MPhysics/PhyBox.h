/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntity.h"

namespace Rad {

	class PHY_ENTRY PhyBox : public PhyEntity
	{
		DECLARE_RTTI();

	public:
		PhyBox(eType type);
		virtual ~PhyBox();

		void 
			SetLength(float lx, float ly, float lz);
		void 
			GetLength(float & lx, float & ly, float & lz);

		void 
			SetMass(float mass, float lx, float ly, float lz);
	};

}
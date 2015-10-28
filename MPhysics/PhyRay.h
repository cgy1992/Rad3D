/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntity.h"
#include "MFloat3.h"
#include "MRay.h"

namespace Rad {

	class PHY_ENTRY PhyRay : public PhyEntity
	{
		DECLARE_RTTI();

	public:
		PhyRay();
		virtual ~PhyRay();

		void 
			SetRayParam(const Float3 & o, const Float3 & dir, float length);
		void 
			GetRayParam(Float3 & o, Float3 & dir, float & length);

		void 
			SetContactParam(int firstContact, int backfaceCull);
		void 
			GetContactParam(int & firstContact, int & backfaceCull);

		void 
			SetClosestHit(int closestHit);
		int 
			GetClosestHit();


	protected:
	};

}
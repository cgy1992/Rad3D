#include "PhyRay.h"
#include "PhyWorld.h"
#include "PhyInternal.h"

namespace Rad {

	ImplementRTTI(PhyRay, PhyEntity);

	PhyRay::PhyRay()
		: PhyEntity(PhyEntity::STATIC_BODY)
	{
		mGeomId = dCreateRay(g_OdeSpace, 1);

		dGeomSetData(mOdeGeom, this);
	}

	PhyRay::~PhyRay()
	{
	}

	void PhyRay::SetRayParam(const Float3 & o, const Float3 & dir, float length)
	{
		dGeomRaySet(mOdeGeom, o.x, o.y, o.z, dir.x, dir.y, dir.z);
		dGeomRaySetLength(mOdeGeom, length);
	}

	void PhyRay::GetRayParam(Float3 & o, Float3 & dir, float & length)
	{
		dVector3 v1, v2;
		dGeomRayGet(mOdeGeom, v1, v2);

		o = Float3(v1[0], v1[1], v1[2]);
		dir = Float3(v2[0], v2[1], v2[2]);
		length = dGeomRayGetLength(mOdeGeom);
	}

	void PhyRay::SetContactParam(int firstContact, int backfaceCull)
	{
		dGeomRaySetParams(mOdeGeom, firstContact, backfaceCull);
	}

	void PhyRay::GetContactParam(int & firstContact, int & backfaceCull)
	{
		dGeomRayGetParams(mOdeGeom, &firstContact, &backfaceCull);
	}

	void PhyRay::SetClosestHit(int closestHit)
	{
		dGeomRaySetClosestHit(mOdeGeom, closestHit);
	}

	int PhyRay::GetClosestHit()
	{
		return dGeomRayGetClosestHit(mOdeGeom);
	}

}
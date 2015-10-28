#include "PhyCapsule.h"
#include "PhyWorld.h"
#include "PhyInternal.h"

namespace Rad {

	ImplementRTTI(PhyCapsule, PhyEntity);

	PhyCapsule::PhyCapsule(eType type)
		: PhyEntity(type)
	{
		mGeomId = dCreateCapsule(g_OdeSpace, 1, 1);

		if (mBodyId != NULL)
			dGeomSetBody(mOdeGeom, mOdeBody);

		dGeomSetData(mOdeGeom, this);
	}

	PhyCapsule::~PhyCapsule()
	{
	}

	void PhyCapsule::SetCapsuleParam(float radius, float length)
	{
		dGeomCapsuleSetParams(mOdeGeom, radius, length);
	}

	void PhyCapsule::GetCapsuleParam(float & radius, float & length)
	{
		dGeomCapsuleGetParams(mOdeGeom, &radius, &length);
	}

	void PhyCapsule::SetMass(float mass, int direction, float radius, float length)
	{
		if (mBodyId != NULL)
		{
			dMass m;
			dMassSetZero(&m);
			dMassSetCapsuleTotal(&m, mass, direction, radius, length);
			dBodySetMass(mOdeBody, &m);
		}
	}

}

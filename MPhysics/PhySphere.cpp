#include "PhySphere.h"
#include "PhyWorld.h"
#include "PhyInternal.h"

namespace Rad {

	ImplementRTTI(PhySphere, PhyEntity);

	PhySphere::PhySphere(eType type)
		: PhyEntity(type)
	{
		mGeomId = dCreateSphere(g_OdeSpace, 1);

		if (mBodyId != NULL)
			dGeomSetBody(mOdeGeom, mOdeBody);

		dGeomSetData(mOdeGeom, this);
	}

	PhySphere::~PhySphere()
	{
	}

	void PhySphere::SetRadius(float radius)
	{
		dGeomSphereSetRadius(mOdeGeom, radius);
	}

	float PhySphere::GetRadius()
	{
		return dGeomSphereGetRadius(mOdeGeom);
	}

	void PhySphere::SetMass(float mass, float radius)
	{
		if (mBodyId != NULL)
		{
			dMass m;
			dMassSetZero(&m);
			dMassSetSphereTotal(&m, mass, radius);
			dBodySetMass(mOdeBody, &m);
		}
	}

}
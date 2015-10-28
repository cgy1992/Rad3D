#include "PhyCylinder.h"
#include "PhyWorld.h"
#include "PhyInternal.h"

namespace Rad {

	ImplementRTTI(PhyCylinder, PhyEntity);

	PhyCylinder::PhyCylinder(eType type)
		: PhyEntity(type)
	{
		mGeomId = dCreateCylinder(g_OdeSpace, 1, 1);

		if (mBodyId != NULL)
			dGeomSetBody(mOdeGeom, mOdeBody);

		dGeomSetData(mOdeGeom, this);
	}

	PhyCylinder::~PhyCylinder()
	{
	}

	void PhyCylinder::SetCylinderParam(float radius, float length)
	{
		dGeomCylinderSetParams(mOdeGeom, radius, length);
	}

	void PhyCylinder::GetCylinderParam(float & radius, float & length)
	{
		dGeomCylinderGetParams(mOdeGeom, &radius, &length);
	}

	void PhyCylinder::SetMass(float mass, int direction, float radius, float length)
	{
		if (mBodyId != NULL)
		{
			dMass m;

			dMassSetZero(&m);
			dMassSetCylinderTotal(&m, mass, direction, radius, length);

			dBodySetMass(mOdeBody, &m);
		}
	}

}

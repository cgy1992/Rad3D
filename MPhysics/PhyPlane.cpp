#include "PhyPlane.h"
#include "PhyWorld.h"
#include "PhyInternal.h"

namespace Rad {

	ImplementRTTI(PhyPlane, PhyEntity);

	PhyPlane::PhyPlane()
		: PhyEntity(PhyEntity::STATIC_BODY)
	{
		mGeomId = dCreatePlane(g_OdeSpace, 0, 1, 0, 0);

		dGeomSetData(mOdeGeom, this);
	}

	PhyPlane::~PhyPlane()
	{
	}

	void PhyPlane::SetPlane(float a, float b, float c, float d)
	{
		dGeomPlaneSetParams(mOdeGeom, a, b, c, d);
	}

	void PhyPlane::GetPlane(float & a, float & b, float & c, float & d)
	{
		dVector4 v;
		dGeomPlaneGetParams(mOdeGeom, v);

		a = v[0]; b = v[1]; c = v[2]; d = v[3];
	}
}
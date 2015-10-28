#include "PhyBox.h"
#include "PhyWorld.h"
#include "PhyInternal.h"

namespace Rad {

	ImplementRTTI(PhyBox, PhyEntity);

	PhyBox::PhyBox(eType type)
		: PhyEntity(type)
	{
		mGeomId = dCreateBox(g_OdeSpace, 1, 1, 1);
		
		if (mBodyId != NULL)
			dGeomSetBody(mOdeGeom, mOdeBody);

		dGeomSetData(mOdeGeom, this);
	}

	PhyBox::~PhyBox()
	{
	}

	void PhyBox::SetLength(float lx, float ly, float lz)
	{
		dGeomBoxSetLengths(mOdeGeom, lx, ly, lz);
	}

	void PhyBox::GetLength(float & lx, float & ly, float & lz)
	{
		dVector3 v;
		dGeomBoxGetLengths(mOdeGeom, v);
		lx = v[0]; ly = v[1]; lz = v[2];
	}

	void PhyBox::SetMass(float mass, float lx, float ly, float lz)
	{
		if (mBodyId != NULL)
		{
			dMass m;
			dMassSetZero(&m);
			dMassSetBoxTotal(&m, mass, lx, ly, lz);
			dBodySetMass(mOdeBody, &m);
		}
	}

}
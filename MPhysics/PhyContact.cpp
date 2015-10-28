#include "PhyContact.h"
#include "PhyInternal.h"

namespace Rad {

#define mOdeContact ((dContact *)mContactId)

	PhyContact::PhyContact()
		: mContactId(NULL)
	{
	}

	PhyContact::~PhyContact()
	{
	}

	Float3 PhyContact::GetContactPosition()
	{
		Float3 p;

		p.x = mOdeContact->geom.pos[0];
		p.y = mOdeContact->geom.pos[1];
		p.z = mOdeContact->geom.pos[2];

		return p;
	}

	Float3 PhyContact::GetContactNormal()
	{
		Float3 n;

		n.x = mOdeContact->geom.normal[0];
		n.y = mOdeContact->geom.normal[1];
		n.z = mOdeContact->geom.normal[2];
		
		return n;
	}

	float PhyContact::GetPenetrationDepth()
	{
		return mOdeContact->geom.depth;
	}

	PhyEntity * PhyContact::GetEntity1()
	{
		if (mOdeContact->geom.g1 != NULL)
			return (PhyEntity *)dGeomGetData(mOdeContact->geom.g1);
		
		return NULL;
	}

	PhyEntity * PhyContact::GetEntity2()
	{
		if (mOdeContact->geom.g2 != NULL)
			return (PhyEntity *)dGeomGetData(mOdeContact->geom.g2);

		return NULL;
	}

}
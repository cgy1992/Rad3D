/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#define dSINGLE
#include <ode/ode.h>

#include "PhyEntry.h"

namespace Rad {

#define g_OdeWorld ((dWorldID)PhyWorld::Instance()->_getInternalWorld())
#define g_OdeSpace ((dSpaceID)PhyWorld::Instance()->_getInternalSpace())
#define g_OdeJointGroup ((dJointGroupID)PhyWorld::Instance()->_getInternalJointGroup())

#define mOdeMass ((dMass *)mMassId)
#define mOdeBody ((dBodyID)mBodyId)
#define mOdeGeom ((dGeomID)mGeomId)

}


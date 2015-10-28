/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyInternal.h"
#include "PhyContact.h"

namespace Rad {

	void ode_updateCallback(void *data, dGeomID o1, dGeomID o2);

	struct ode_callback_data
	{
		PhyCollideCallback * callback;
		void * data;
	};

	void ode_collideCallback(void *data, dGeomID o1, dGeomID o2);
	
}
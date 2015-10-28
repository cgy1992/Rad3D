/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntry.h"
#include "PhyEntity.h"

namespace Rad {

	class PHY_ENTRY PhyContact
	{
		DECLARE_ALLOC();

	public:
		void 
			_setContactId(PhyHandle _contactId) { mContactId = _contactId; }
		PhyHandle 
			_getContactId() { return mContactId; }

	public:
		PhyContact();
		~PhyContact();

		Float3 
			GetContactPosition();
		Float3 
			GetContactNormal();
		float
			GetPenetrationDepth();

		PhyEntity * 
			GetEntity1();
		PhyEntity * 
			GetEntity2();

	protected:
		PhyHandle mContactId;
		
	};

	typedef void PhyCollideCallback(void * data, PhyContact * contacts, int count);
}
#include "PhyCallback.h"
#include "PhyWorld.h"

namespace Rad{

	#define MAX_CONTACTS 8

	inline void PhyCalcuBounce(dContact * contact)
	{
		PhyEntity * entity1 = (PhyEntity *)dGeomGetData(contact->geom.g1);
		PhyEntity * entity2 = (PhyEntity *)dGeomGetData(contact->geom.g2);

		if (entity1 != NULL && entity2 != NULL)
		{
			float bounce1, bounce2;
			float bounce_vel1, bounce_vel2;

			entity1->GetBounceParam(bounce1, bounce_vel1);
			entity2->GetBounceParam(bounce2, bounce_vel2);

			contact->surface.bounce = (bounce1 + bounce2) * 0.5f;
			contact->surface.bounce_vel = (bounce_vel1 + bounce_vel2) * 0.5f;
		}
	}

	inline bool PhyTestNoCollide(PhyEntity * e1, PhyEntity * e2)
	{
		if (e1 == NULL || e2 == NULL || e1 == e2)
			return true;

		int collFlag1 = e1->GetCollisionFlag();
		int collFlag2 = e2->GetCollisionFlag();

		return (collFlag1 & collFlag2) == 0;
	}

	void ode_updateCallback(void *data, dGeomID o1, dGeomID o2)
	{
		dBodyID b1 = dGeomGetBody(o1);
		dBodyID b2 = dGeomGetBody(o2);

		if (b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact))
			return;

		PhyEntity * entity1 = (PhyEntity *)dGeomGetData(o1);
		PhyEntity * entity2 = (PhyEntity *)dGeomGetData(o2);

		if (PhyTestNoCollide(entity1, entity2))
			return ;

		dContact contact[MAX_CONTACTS];
		PhyContact phy_contact[MAX_CONTACTS];
		for (int i = 0; i < MAX_CONTACTS; ++i)
		{
			contact[i].surface.mode = dContactBounce;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.mu2 = 0.0f;
			contact[i].surface.bounce = 0.1f;
			contact[i].surface.bounce_vel = 0.1f;
			contact[i].surface.soft_cfm = 0.01f;

			phy_contact[i]._setContactId(&contact[i]);
		}

		int count = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
		if (count > 0)
		{
			PhyWorld::Instance()->E_OnCollide(phy_contact, count);
		}

		for (int i = 0; i < count; i++)
		{
			PhyCalcuBounce(contact + i);

			dJointID jointId = dJointCreateContact(g_OdeWorld, g_OdeJointGroup, contact + i);
			dJointAttach(jointId, b1, b2);
		}
	}

	void ode_collideCallback(void *data, dGeomID o1, dGeomID o2)
	{
		PhyEntity * entity1 = (PhyEntity *)dGeomGetData(o1);
		PhyEntity * entity2 = (PhyEntity *)dGeomGetData(o2);

		if (PhyTestNoCollide(entity1, entity2))
			return ;

		ode_callback_data * callbackData = (ode_callback_data *)data;

		dContact contact[MAX_CONTACTS];
		PhyContact phy_contact[MAX_CONTACTS];
		for (int i = 0; i < MAX_CONTACTS; ++i)
		{
			contact[i].surface.mode = dContactBounce;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.mu2 = 0.0f;
			contact[i].surface.bounce = 0.1f;
			contact[i].surface.bounce_vel = 0.1f;
			contact[i].surface.soft_cfm = 0.01f;

			phy_contact[i]._setContactId(&contact[i]);
		}

		int count = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));

		if (count > 0 && callbackData->callback != NULL)
		{
			PhyCollideCallback * callback = (PhyCollideCallback *)callbackData->callback;

			callback(callbackData->data, phy_contact, count);
		}
	}
}


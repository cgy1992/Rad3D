#include "PhyWorld.h"
#include "PhyInternal.h"
#include "PhyCallback.h"

namespace Rad {

	ImplementSingleton(PhyWorld);
	
	PhyWorld::PhyWorld()
	{
		// ODE Init 
		dInitODE();

		mWorldId = dWorldCreate();
		dWorldSetCFM (g_OdeWorld, EPSILON_E5);
		dWorldSetAutoDisableFlag (g_OdeWorld, 1);
		dWorldSetLinearDamping(g_OdeWorld, 0.00001f);
		dWorldSetAngularDamping(g_OdeWorld, 0.005f);
		dWorldSetMaxAngularSpeed(g_OdeWorld, 200.0f);
		dWorldSetContactMaxCorrectingVel(g_OdeWorld, 0.1f);

		mSpaceId = dHashSpaceCreate(0);
		mJointGroupId = dJointGroupCreate(0);

		mEntityLinker = NULL;

		mPhyRay = new PhyRay;
	}

	PhyWorld::~PhyWorld()
	{
		delete mPhyRay;

		d_assert (mEntityLinker == NULL);

		// ODE Shutdown
		dJointGroupDestroy(g_OdeJointGroup);
		dSpaceDestroy(g_OdeSpace);
		dWorldDestroy(g_OdeWorld);
		dCloseODE();
	}

	void PhyWorld::SetWorldGravity(float x, float y, float z)
	{
		dWorldSetGravity(g_OdeWorld, x, y, z);
	}

	void PhyWorld::SetWorldContactSurfaceLayer(float depth)
	{
		dWorldSetContactSurfaceLayer(g_OdeWorld, depth);
	}

	void PhyWorld::Collide(PhyEntity * e1, PhyCollideCallback * callback, void * data)
	{
		d_assert (e1 != NULL);

		ode_callback_data callbackData;
		callbackData.callback = callback;
		callbackData.data = data;

		dSpaceCollide2((dGeomID)e1->mGeomId, (dGeomID)mSpaceId, &callbackData, &ode_collideCallback);
	}

	void PhyWorld::Collide(PhyEntity * e1, PhyEntity * e2, PhyCollideCallback * callback, void * data)
	{
		d_assert (e1 != NULL && e2 != NULL);

		ode_callback_data callbackData;
		callbackData.callback = callback;
		callbackData.data = data;

		ode_collideCallback(&callbackData, (dGeomID)e1->mGeomId, (dGeomID)e2->mGeomId);
	}

	void PhyWorld::RayCheck(const Ray & ray, float len, int flag, PhyCollideCallback * callback, void * data)
	{
		mPhyRay->SetRayParam(ray.orig, ray.dir, len);
		mPhyRay->SetCollisionFlag(flag);
		mPhyRay->SetContactParam(TRUE, TRUE);
		mPhyRay->SetClosestHit(TRUE);

		Collide(mPhyRay, callback, data);
	}

	void PhyWorld::RayCheck(const Ray & ray, float len, PhyEntity * e, PhyCollideCallback * callback, void * data)
	{
		mPhyRay->SetRayParam(ray.orig, ray.dir, len);
		mPhyRay->SetCollisionFlag(e->GetCollisionFlag());
		mPhyRay->SetContactParam(TRUE, TRUE);
		mPhyRay->SetClosestHit(TRUE);

		Collide(mPhyRay, e, callback, data);
	}

	void PhyWorld::Update(float elapsedTime)
	{
		profile_code();

		if (elapsedTime > 0)
		{
			// dSpaceCollide(g_OdeSpace, 0, &ode_updateCallback);

			PhyEntity * e = mEntityLinker;
			while (e != NULL)
			{
				if (e->GetType() == PhyEntity::RIGID_BODY)
				{
					dSpaceCollide2((dGeomID)e->mGeomId, (dGeomID)mSpaceId, NULL, &ode_updateCallback);
				}

				e = LINKER_NEXT(e);
			}

			dWorldQuickStep(g_OdeWorld, elapsedTime);

			dJointGroupEmpty(g_OdeJointGroup);
		}
		
	}
}




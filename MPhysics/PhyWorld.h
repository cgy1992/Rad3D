/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSingleton.h"
#include "PhyEntry.h"

#include "PhyBox.h"
#include "PhyCapsule.h"
#include "PhyCylinder.h"
#include "PhyMesh.h"
#include "PhyPlane.h"
#include "PhyRay.h"
#include "PhySphere.h"
#include "PhyContact.h"

namespace Rad {

	class PHY_ENTRY PhyWorld : public Singleton<PhyWorld>
	{
		friend class PhyEntity;

	public:
		tEvent2<PhyContact *, int> E_OnCollide;

	public:
		PhyHandle 
			_getInternalWorld() { return mWorldId; }
		PhyHandle 
			_getInternalSpace() { return mSpaceId; }
		PhyHandle 
			_getInternalJointGroup() { return mJointGroupId; }

	public:
		PhyWorld();
		~PhyWorld();

		void 
			SetWorldGravity(float x, float y, float z);
		void 
			SetWorldContactSurfaceLayer(float depth);

		void 
			Collide(PhyEntity * e1, PhyCollideCallback * callback, void * data = NULL);
		void 
			Collide(PhyEntity * e1, PhyEntity * e2, PhyCollideCallback * callback, void * data = NULL);
		void 
			RayCheck(const Ray & ray, float len, int flag, PhyCollideCallback * callback, void * data = NULL);
		void 
			RayCheck(const Ray & ray, float len, PhyEntity * e, PhyCollideCallback * callback, void * data = NULL);

		void 
			Update(float elapsedTime);

	protected:
		PhyHandle mWorldId;
		PhyHandle mSpaceId;
		PhyHandle mJointGroupId;

		PhyMeshDataPool mMeshDataPool;

		PhyEntity * mEntityLinker;

		PhyRay * mPhyRay;
	};

}
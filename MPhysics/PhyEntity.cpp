#include "PhyEntity.h"
#include "PhyWorld.h"
#include "PhyInternal.h"

namespace Rad {

	ImplementRTTI(PhyEntity, Shape);

	PhyEntity::PhyEntity(eType type)
		: mType(type)
		, mCollisionFlag(0x7FFFFFFF)
		, mBodyId(NULL)
		, mGeomId(NULL)
		, mBounce(0.1f)
		, mBounceVel(0.1f)
	{
		if (mType == RIGID_BODY)
			mBodyId = dBodyCreate(g_OdeWorld);

		LINKER_APPEND(PhyWorld::Instance()->mEntityLinker, this);
	}

	PhyEntity::~PhyEntity()
	{
		LINKER_REMOVE(PhyWorld::Instance()->mEntityLinker, this);

		if (mBodyId != NULL)
			dBodyDestroy(mOdeBody);

		if (mGeomId != NULL)
			dGeomDestroy(mOdeGeom);
	}

	PhyEntity::eType PhyEntity::GetType()
	{
		return mType;
	}

	void PhyEntity::SetCollisionFlag(int flag)
	{
		mCollisionFlag = flag;
	}

	int PhyEntity::GetCollisionFlag()
	{
		if (mOwner)
		{
			return mOwner->GetFlags();
		}
		else
		{
			return mCollisionFlag;
		}
	}

	void PhyEntity::SetBodyEnable(bool enable)
	{
		if (mBodyId == NULL)
			return ;

		if (enable)
			dBodyEnable(mOdeBody);
		else
			dBodyDisable(mOdeBody);
	}

	bool PhyEntity::IsBodyEnable()
	{
		return mBodyId == NULL && dBodyIsEnabled(mOdeBody) != 0;
	}

	void PhyEntity::SetBodyAutoDisable(bool _auto)
	{
		if (mBodyId != NULL)
			dBodySetAutoDisableFlag(mOdeBody, _auto ? 1 : 0);
	}

	void PhyEntity::SetGeomEnable(bool _enabled)
	{
		if (_enabled)
			dGeomEnable(mOdeGeom);
		else
			dGeomDisable(mOdeGeom);
	}

	bool PhyEntity::IsGeomEnable()
	{
		return dGeomIsEnabled(mOdeGeom) != 0;
	}

	void PhyEntity::SetGravityInfl(bool _infl)
	{
		if (mBodyId != NULL)
		{
			dBodySetGravityMode(mOdeBody, _infl ? 1 : 0);
		}
	}

	bool PhyEntity::IsGravityInfl()
	{
		if (mBodyId != NULL)
		{
			return dBodyGetGravityMode(mOdeBody) != 0;
		}

		return false;
	}

	void PhyEntity::SetBounceParam(float bounce, float bounce_vel)
	{
		mBounce = bounce;
		mBounceVel = bounce_vel;
	}

	void PhyEntity::GetBounceParam(float & bounce, float & bounce_vel)
	{
		bounce = mBounce;
		bounce_vel = mBounceVel;
	}

	void PhyEntity::SetPosition(const Float3 & p)
	{
		if (mBodyId != NULL)
		{
			dBodySetPosition(mOdeBody, p.x, p.y, p.z);
		}
		else
		{
			dGeomSetPosition(mOdeGeom, p.x, p.y, p.z);
		}
	}

	Float3 PhyEntity::GetPosition()
	{
		Float3 position;

		if (mBodyId != NULL)
		{
			const dReal * v = dBodyGetPosition(mOdeBody);
			position.x = v[0];
			position.y = v[1];
			position.z = v[2];
		}
		else
		{
			const dReal * v = dGeomGetPosition(mOdeGeom);
			position.x = v[0];
			position.y = v[1];
			position.z = v[2];
		}

		return position;
	}

	void PhyEntity::SetRotation(const Quat & q)
	{
		dQuaternion dq;
		dq[0] = q.w;
		dq[1] = q.x;
		dq[2] = q.y;
		dq[3] = q.z;

		if (mBodyId != NULL)
		{
			dBodySetQuaternion(mOdeBody, dq);
		}
		else
		{
			dGeomSetQuaternion(mOdeGeom, dq);
		}
	}

	Quat PhyEntity::GetRotation()
	{
		Quat q;

		if (mBodyId != NULL)
		{
			const dReal * dq = dBodyGetQuaternion(mOdeBody);
			q.w = dq[0];
			q.x = dq[1];
			q.y = dq[2];
			q.z = dq[3];
		}
		else
		{
			dQuaternion dq;
			dGeomGetQuaternion(mOdeGeom, dq);
			q.w = dq[0];
			q.x = dq[1];
			q.y = dq[2];
			q.z = dq[3];
		}

		return q;
	}

	void PhyEntity::SetLinearVel(const Float3 & vel)
	{
		if (mBodyId != NULL)
			dBodySetLinearVel(mOdeBody, vel.x, vel.y, vel.z);
	}

	Float3 PhyEntity::GetLinearVel()
	{
		Float3 vel = Float3::Zero;

		if (mBodyId != NULL)
		{
			const dReal * v = dBodyGetLinearVel(mOdeBody);
			vel.x = v[0];
			vel.y = v[1];
			vel.z = v[2];
		}

		return vel;
	}

	void PhyEntity::SetAngularVel(const Float3 & vel)
	{
		if (mBodyId != NULL)
			dBodySetAngularVel(mOdeBody, vel.x, vel.y, vel.z);
	}

	Float3 PhyEntity::GetAngularVel()
	{
		Float3 vel = Float3::Zero;

		if (mBodyId != NULL)
		{
			const dReal * v = dBodyGetAngularVel(mOdeBody);
			vel.x = v[0];
			vel.y = v[1];
			vel.z = v[2];
		}

		return vel;
	}

	void PhyEntity::SetForce(const Float3 & force)
	{
		if (mBodyId != NULL)
			dBodySetForce(mOdeBody, force.x, force.y, force.z);
	}

	Float3 PhyEntity::GetForce()
	{
		Float3 force = Float3::Zero;

		if (mBodyId != NULL)
		{
			const dReal * v = dBodyGetForce(mOdeBody);
			force.x = v[0];
			force.y = v[1];
			force.z = v[2];
		}

		return force;
	}

	void PhyEntity::SetTorque(const Float3 & torque)
	{
		if (mBodyId != NULL)
			dBodySetTorque(mOdeBody, torque.x, torque.y, torque.z);
	}

	Float3 PhyEntity::GetTorque()
	{
		Float3 torque = Float3::Zero;

		if (mBodyId != NULL)
		{
			const dReal * v = dBodyGetTorque(mOdeBody);
			torque.x = v[0];
			torque.y = v[1];
			torque.z = v[2];
		}

		return torque;
	}

	void PhyEntity::AddForce(const Float3 & force)
	{
		if (mBodyId != NULL)
			dBodyAddForce(mOdeBody, force.x, force.y, force.z);
	}

	void PhyEntity::AddForceAt(const Float3 & force, const Float3 & position)
	{
		if (mBodyId != NULL)
			dBodyAddForceAtPos(mOdeBody, force.x, force.y, force.z, position.x, position.y, position.z);
	}
	
	void PhyEntity::AddRelativeForce(const Float3 & force)
	{
		if (mBodyId != NULL)
			dBodyAddRelForce(mOdeBody, force.x, force.y, force.z);
	}

	void PhyEntity::AddRelativeForceAt(const Float3 & force, const Float3 & position)
	{
		if (mBodyId != NULL)
			dBodyAddRelForceAtPos(mOdeBody, force.x, force.y, force.z, position.x, position.y, position.z);
	}

	void PhyEntity::AddRelativeForceAtRelative(const Float3 & force, const Float3 & position)
	{
		if (mBodyId != NULL)
			dBodyAddRelForceAtRelPos(mOdeBody, force.x, force.y, force.z, position.x, position.y, position.z);
	}

	void PhyEntity::AddTorque(const Float3 & torque)
	{
		if (mBodyId != NULL)
			dBodyAddTorque(mOdeBody, torque.x, torque.y, torque.z);
	}

	void PhyEntity::AddRelativeTorque(const Float3 & torque)
	{
		if (mBodyId != NULL)
			dBodyAddRelTorque(mOdeBody, torque.x, torque.y, torque.z);
	}

	void PhyEntity::OnChangeTm(int flags)
	{
		if (mType == STATIC_BODY)
		{
			if (flags & eTmFlags::TRANSLATE)
			{
				SetPosition(mOwner->GetWorldPosition());
			}

			if (flags & eTmFlags::ROTATE)
			{
				SetRotation(mOwner->GetWorldRotation());
			}
		}
	}

	Node * __RayCheckNode = NULL;
	Float3 __RayCheckPosition = Float3(0, 0, 0);

	void __RayCheckCallBack(void * data, PhyContact * contacts, int count)
	{
		PhyEntity * pEntity = contacts->GetEntity2();

		if (data != NULL)
		{
			__RayCheckNode = (Node *)data;
			__RayCheckPosition = contacts->GetContactPosition();
		}
	}

	bool PhyEntity::OnRayCheck(float & dist, const Ray & ray, float length)
	{
		__RayCheckNode = NULL;

		PhyWorld::Instance()->RayCheck(ray, length, this, __RayCheckCallBack, mOwner);

		if (__RayCheckNode != NULL)
		{
			dist = __RayCheckPosition.Distance(ray.orig);
		}

		return __RayCheckNode != NULL;
	}
}

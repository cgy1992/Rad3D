/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntry.h"
#include "MShape.h"
#include "MNode.h"

namespace Rad {

	class World;

	class PHY_ENTRY PhyEntity : public Shape
	{
		DECLARE_RTTI();
		DECLARE_LINKER(PhyEntity);

		friend class PhyWorld;

	public:
		enum eType {
			STATIC_BODY,
			RIGID_BODY,
		};

	public:
		PhyHandle _getGeomId() { return mGeomId; }
		PhyHandle _getBodyId() { return mBodyId; }

	public:
		PhyEntity(eType type);
		virtual ~PhyEntity();

		eType 
			GetType();

		void 
			SetCollisionFlag(int filed);
		int 
			GetCollisionFlag();

		void 
			SetBodyEnable(bool _enabled);
		bool 
			IsBodyEnable();

		void 
			SetBodyAutoDisable(bool _auto);

		void 
			SetGeomEnable(bool _enabled);
		bool 
			IsGeomEnable();

		void 
			SetGravityInfl(bool _infl);
		bool 
			IsGravityInfl();

		void 
			SetBounceParam(float bounce, float bounce_vel);
		void 
			GetBounceParam(float & bounce, float & bounce_vel);

		void 
			SetPosition(const Float3 & p);
		Float3 
			GetPosition();

		void 
			SetRotation(const Quat & q);
		Quat 
			GetRotation();

		void 
			SetLinearVel(const Float3 & vel);
		Float3 
			GetLinearVel();

		void 
			SetAngularVel(const Float3 & vel);
		Float3 
			GetAngularVel();

		void 
			SetForce(const Float3 & force);
		Float3 
			GetForce();

		void 
			SetTorque(const Float3 & force);
		Float3 
			GetTorque();

		void 
			AddForce(const Float3 & force);
		void 
			AddForceAt(const Float3 & force, const Float3 & position);

		void 
			AddRelativeForce(const Float3 & force);
		void 
			AddRelativeForceAt(const Float3 & force, const Float3 & position);
		void 
			AddRelativeForceAtRelative(const Float3 & torque, const Float3 & position);

		void 
			AddTorque(const Float3 & torque);
		void 
			AddRelativeTorque(const Float3 & torque);

		//
		virtual void
			OnAttach(Node * owner) { Shape::OnAttach(owner); }
		virtual void 
			OnLoaded() {}
		virtual void 
			OnUnload() {}
		virtual void 
			OnUpdate(float elapsedTime) {}
		virtual void
			OnChangeTm(int flags);

		virtual bool
			OnRayCheck(float & dist, const Ray & ray, float length);

	protected:
		eType mType;

		int mCollisionFlag;

		PhyHandle mBodyId;
		PhyHandle mGeomId;

		float mBounce;
		float mBounceVel;
	};
}
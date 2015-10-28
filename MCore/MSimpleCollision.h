/*
*	Simple Collision
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRay.h"
#include "MEvent.h"
#include "MComponent.h"

namespace Rad {

	struct eSimpleCollisionType : public IEnum
	{
		enum enum_t {
			BOX,
			SPHERE,
		};

		DECLARE_ENUM(eSimpleCollisionType);
	};

	//
	class M_ENTRY SimpleCollision : public IComponent
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(IComponent);

	public:
		int mType;
		float mRadiusRatio;
		int mUserFlags;

	public:
		SimpleCollision();
		virtual ~SimpleCollision();

		virtual void
			Attach(ComponentOwner * owner);
		virtual void
			Detach();

		virtual int
			Update(float elapsedTime) { return 0; }

		bool
			RayCheck(float * dist, const Ray & ray, float len);

		void 
			OnDebugRender();

	protected:
		cListener0<SimpleCollision> L_DebugRender;
	};

}
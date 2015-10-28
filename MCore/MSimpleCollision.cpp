#include "MSimpleCollision.h"
#include "MRenderHelper.h"
#include "MNode.h"
#include "MWorld.h"

namespace Rad {

	DF_ENUM_BEGIN(eSimpleCollisionType)
		DF_ENUM(BOX)
		DF_ENUM(SPHERE)
	DF_ENUM_END()


	DF_PROPERTY_BEGIN(SimpleCollision)
		DF_PROPERTY_ENUM(SimpleCollision, mType, "", "Type", eSimpleCollisionType)
		DF_PROPERTY(SimpleCollision, mRadiusRatio, "", "RadiusRatio", PT_Float)
		DF_PROPERTY(SimpleCollision, mUserFlags, "", "UserFlags", PT_Int)
	DF_PROPERTY_END()

	ImplementRTTI(SimpleCollision, IComponent);

	SimpleCollision::SimpleCollision()
	{
		mType = eSimpleCollisionType::BOX;
		mRadiusRatio = 0.8f;
		mUserFlags = 0;
	}

	SimpleCollision::~SimpleCollision()
	{
	}

	void SimpleCollision::Attach(ComponentOwner * owner)
	{
		d_assert (KIND_OF(Node, owner));

		IComponent::Attach(owner);

		if (Root::InEditor())
		{
			World::Instance()->E_RenderSolidEnd += L_DebugRender(this, &SimpleCollision::OnDebugRender);
		}
	}

	void SimpleCollision::Detach()
	{
		if (Root::InEditor())
		{
			World::Instance()->E_RenderSolidEnd -= L_DebugRender(this, &SimpleCollision::OnDebugRender);
		}

		IComponent::Detach();
	}

	bool SimpleCollision::RayCheck(float * _dist, const Ray & ray, float len)
	{
		Node * node = (Node *)mOwner;

		float dist;
		Aabb aabb = node->GetLocalAabb();
		Mat4 form = node->GetWorldTM();

		if (mType == eSimpleCollisionType::BOX)
		{
			Obb obb(aabb.minimum, aabb.maximum);
			obb.extent *= mRadiusRatio;
			obb.Transform(form);

			if (ray.Intersect(&dist, obb))
			{
				if (*_dist)
					*_dist = dist;

				return dist < len;
			}
		}
		else
		{
			Float3 halfSize = aabb.GetHalfSize();

			Sphere sph;
			sph.center = aabb.GetCenter();
			sph.radius = Max3(halfSize.x, halfSize.y, halfSize.z) * mRadiusRatio;
			sph.center.TransformA(form);

			if (ray.Intersect(&dist, sph))
			{
				if (*_dist)
					*_dist = dist;

				return dist < len;
			}
		}

		return false;
	}

	void SimpleCollision::OnDebugRender()
	{
		Camera * cam = World::Instance()->GetCurrentRenderContext()->GetCamera();
		Node * node = (Node *)mOwner;

		if (cam->InVisible(node->GetWorldAabb()))
		{
			Aabb aabb = node->GetLocalAabb();
			Mat4 form = node->GetWorldTM();
			Float4 color = Float4(0, 1, 0);

			if (mType == eSimpleCollisionType::BOX)
			{
				Obb obb(aabb.minimum, aabb.maximum);
				obb.extent *= mRadiusRatio;

				RenderSystem::Instance()->SetRenderState(eFillMode::FRAME, eCullMode::NONE);
				RenderHelper::Instance()->DebugDrawBox(obb, color, form);
			}
			else
			{
				Float3 halfSize = aabb.GetHalfSize();

				Sphere sph;
				sph.center = aabb.GetCenter();
				sph.radius = Max3(halfSize.x, halfSize.y, halfSize.z) * mRadiusRatio;

				RenderSystem::Instance()->SetRenderState(eFillMode::FRAME, eCullMode::NONE);
				RenderHelper::Instance()->DebugDrawSphere(sph, color, form);
			}
		}
	}

}
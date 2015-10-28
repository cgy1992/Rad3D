#include "PSModifierDeflectorPlane.h"

namespace Rad {

	ImplementRTTI(PS_ModifierDeflectorPlane, PS_Modifier);

	DF_PROPERTY_BEGIN(PS_ModifierDeflectorPlane)
		DF_PROPERTY_EX(PS_ModifierDeflectorPlane, mPlane, "", "Plane", "DeflectorPlane", PT_Float4)
		DF_PROPERTY(PS_ModifierDeflectorPlane, mBounce, "", "Bounce", PT_Float)
	DF_PROPERTY_END()

	PS_ModifierDeflectorPlane::PS_ModifierDeflectorPlane()
		: mPlane(Float3(0, 1, 0), 0)
		, mBounce(0)
	{
	}

	PS_ModifierDeflectorPlane::~PS_ModifierDeflectorPlane()
	{
	}

	void PS_ModifierDeflectorPlane::SetPlane(const Plane & plane)
	{
		mPlane = plane;
	}

	void PS_ModifierDeflectorPlane::SetBounce(float bounce)
	{
		mBounce = bounce;
	}

	void PS_ModifierDeflectorPlane::Modify(Particle * p, float elapsedTime)
	{
		Float3 Velocity = p->Direction * p->Speed;
		Float3 dir = Velocity * elapsedTime;

		if (mPlane.Distance(p->Position + dir) <= 0.0)
		{
			float d = mPlane.Distance(p->Position);

			if (d > 0.0)
			{
				// for intersection point
				Float3 dist = dir * (- d / dir.Dot(mPlane.normal));
				// set new position
				p->Position = (p->Position + dist) + (dist - dir) * mBounce;

				Velocity.Reflect(mPlane.normal);
				Velocity *= mBounce;

				p->Direction = Velocity;
				p->Speed = p->Direction.Normalize();
			}
		}
	}
}

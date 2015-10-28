#include "PSModifierRotation2.h"

namespace Rad {

	ImplementRTTI(PS_ModifierRotation2, PS_Modifier);

	DF_PROPERTY_BEGIN(PS_ModifierRotation2)
		DF_PROPERTY(PS_ModifierRotation2, mRotationSpeed, "", "RotationSpeed", PT_Float3)
		DF_PROPERTY(PS_ModifierRotation2, mRotationAble, "", "RotationAble", PT_Bool)
	DF_PROPERTY_END()

	PS_ModifierRotation2::PS_ModifierRotation2()
		: mRotationSpeed(0, 90, 0)
		, mRotationAble(false)
	{
	}

	PS_ModifierRotation2::~PS_ModifierRotation2()
	{
	}

	void PS_ModifierRotation2::SetRotationSpeed(const Float3 & speed)
	{
		mRotationSpeed = speed;
	}

	void PS_ModifierRotation2::SetRotationAble(bool b)
	{
		mRotationAble = b;
	}

	void PS_ModifierRotation2::Modify(Particle * p, float elapsedTime)
	{
		Float3 step = mRotationSpeed * elapsedTime;

		Quat q;
		q.FromEulerAngle(step);

		if (mParent->IsLocalSpace())
		{
			p->Position.TransformQ(q);
		}
		else
		{
			p->Position -= mParent->GetParent()->GetWorldPosition();
			p->Position.TransformQ(q);
			p->Position += mParent->GetParent()->GetWorldPosition();
		}

		if (mRotationAble)
		{
			p->Rotation += step;
		}
	}

}

#include "PSModifierRotation2.h"

namespace Rad {

	ImplementRTTI(PS_ModifierRotation2, PS_Modifier);

	DF_PROPERTY_BEGIN(PS_ModifierRotation2)
		DF_PROPERTY_EX(PS_ModifierRotation2, mKeyController, "", "KeyController", "PT_KeyController", PT_UserData)
		DF_PROPERTY(PS_ModifierRotation2, mRotationAble, "", "RotationAble", PT_Bool)
	DF_PROPERTY_END()

	PS_ModifierRotation2::PS_ModifierRotation2()
		: mRotationAble(false)
	{
	}

	PS_ModifierRotation2::~PS_ModifierRotation2()
	{
	}

	void PS_ModifierRotation2::SetRotationAble(bool b)
	{
		mRotationAble = b;
	}

	void PS_ModifierRotation2::Modify(Particle * p, float elapsedTime)
	{
		float time = 1 - p->Life * p->InitLife.y;

		KF_Float3 v;
		if (mKeyController.GetValue(v, time, true))
		{
			Float3 step = v.data * elapsedTime;

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

}

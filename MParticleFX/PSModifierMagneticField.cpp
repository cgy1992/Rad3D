#include "PSModifierMagneticField.h"

namespace Rad {

	ImplementRTTI(PS_ModifierMagneticField, PS_Modifier);

	DF_PROPERTY_BEGIN(PS_ModifierMagneticField)
		DF_PROPERTY(PS_ModifierMagneticField, mCenter, "", "Center", PT_Float3)
		DF_PROPERTY(PS_ModifierMagneticField, mRadius, "", "Radius", PT_Float)
		DF_PROPERTY(PS_ModifierMagneticField, mInnerLife, "", "InnerLife", PT_Float)
		DF_PROPERTY_ENUM(PS_ModifierMagneticField, mOperation, "", "Operation", PS_Operation)
		DF_PROPERTY_EX(PS_ModifierMagneticField, mKeyController, "", "KeyController", "PT_KeyController", PT_UserData)
	DF_PROPERTY_END()

	PS_ModifierMagneticField::PS_ModifierMagneticField()
		: mCenter(0, 0, 0)
		, mRadius(0)
		, mOperation(PS_Operation::SET)
		, mInnerLife(1)
	{
	}

	PS_ModifierMagneticField::~PS_ModifierMagneticField()
	{
	}

	void PS_ModifierMagneticField::Modify(Particle * p, float elapsedTime)
	{
		Float3 center = mCenter;
		if (!mParent->IsLocalSpace())
		{
			center.TransformA(mParent->GetParent()->GetWorldTM());
		}

		Float3 dir = mCenter - p->Position;
		float len = dir.Normalize();
		if (len - mRadius > p->Speed * elapsedTime)
		{
			KF_Float v;
			if (mKeyController.GetValue(v, time, true))
			{
				Float3 Velocity = p->Direction * p->Speed;

				switch (mOperation)
				{
				case PS_Operation::SET:
					Velocity = dir * v.data;
					break;

				case PS_Operation::ADD:
					Velocity += dir * v.data * elapsedTime;
					break;

				case PS_Operation::MUL:
					Velocity *= dir * v.data;
					break;

				case PS_Operation::AVG:
					Velocity = (dir * v.data + Velocity) * 0.5f;
					break;
				}

				p->Direction = Velocity;
				p->Speed = p->Direction.Normalize();
			}
		}
		else
		{
			p->Position = center - dir * mRadius;
			p->Speed = 0;

			if (p->Life > mInnerLife * p->InitLife.x)
				p->Life = mInnerLife * p->InitLife.x;
		}
	}

}
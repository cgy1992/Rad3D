#include "PSModifierMagneticField.h"

namespace Rad {

	ImplementRTTI(PS_ModifierMagneticField, PS_Modifier);

	DF_PROPERTY_BEGIN(PS_ModifierMagneticField)
		DF_PROPERTY(PS_ModifierMagneticField, mCenter, "", "Center", PT_Float3)
		DF_PROPERTY(PS_ModifierMagneticField, mRadius, "", "Radius", PT_Float)
		DF_PROPERTY(PS_ModifierMagneticField, mAttraction, "", "Attraction", PT_Float)
		DF_PROPERTY_ENUM(PS_ModifierMagneticField, mOperation, "", "Operation", PS_Operation)
		DF_PROPERTY(PS_ModifierMagneticField, mInnerLife, "", "InnerLife", PT_Float)
	DF_PROPERTY_END()

	PS_ModifierMagneticField::PS_ModifierMagneticField()
		: mCenter(0, 0, 0)
		, mRadius(0)
		, mAttraction(UNIT_METRES)
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

		switch (mOperation)
		{
		case PS_Operation::SET:
			p->Speed = mAttraction;
			break;

		case PS_Operation::ADD:
			p->Speed += mAttraction * elapsedTime;
			break;

		case PS_Operation::MUL:
			p->Speed *= mAttraction;
			break;

		case PS_Operation::AVG:
			p->Speed = (p->Speed + mAttraction) * 0.5f;
			break;
		}

		Float3 dir = mCenter - p->Position;
		float len = dir.Normalize();
		if (len - mRadius > p->Speed * elapsedTime)
		{
			p->Direction = dir;
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
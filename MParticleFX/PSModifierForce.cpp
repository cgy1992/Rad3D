#include "PSModifierForce.h"

namespace Rad {

	ImplementRTTI(PS_ModifierForce, PS_ModifierKeyController);

	DF_PROPERTY_BEGIN(PS_ModifierForce)
		DF_PROPERTY_ENUM(PS_ModifierForce, mOperation, "", "Type", PS_Operation)
		DF_PROPERTY_EX(PS_ModifierForce, mKeyController, "", "KeyController", "PT_KeyController", PT_UserData)
	DF_PROPERTY_END()

	PS_ModifierForce::PS_ModifierForce()
		: mOperation(PS_Operation::ADD)
	{
	}

	PS_ModifierForce::~PS_ModifierForce()
	{
	}

	void PS_ModifierForce::Modify(Particle * p, float elapsedTime)
	{
		float time = 1 - p->Life / p->MaxLife;

		KF_Float3 v;
		if (mKeyController.GetValue(v, time, true))
		{
			Float3 Velocity = p->Direction * p->Speed;

			switch (mOperation)
			{
			case PS_Operation::SET:
				Velocity = v.data;
				break;

			case PS_Operation::ADD:
				Velocity += v.data * elapsedTime;
				break;

			case PS_Operation::MUL:
				Velocity *= v.data;
				break;

			case PS_Operation::AVG:
				Velocity = (Velocity + v.data) / 2;
				break;
			}

			p->Direction = Velocity;
			p->Speed = p->Direction.Normalize();
		}
	}

}
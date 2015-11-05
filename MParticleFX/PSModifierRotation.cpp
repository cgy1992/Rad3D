#include "PSModifierRotation.h"

namespace Rad {

	ImplementRTTI(PS_ModifierRotation, PS_ModifierKeyController);

	DF_PROPERTY_BEGIN(PS_ModifierRotation)
		DF_PROPERTY_ENUM(PS_ModifierRotation, mOperation, "", "Operation", PS_Operation)
		DF_PROPERTY_EX(PS_ModifierRotation, mKeyController, "", "KeyController", "PT_KeyController", PT_UserData)
	DF_PROPERTY_END()

	PS_ModifierRotation::PS_ModifierRotation()
		: mOperation(PS_Operation::SET)
	{
	}

	PS_ModifierRotation::~PS_ModifierRotation()
	{
	}

	void PS_ModifierRotation::Modify(Particle * p, float elapsedTime)
	{
		float time = 1 - p->Life * p->InitLife.y;

		KF_Float3 v;
		if (mKeyController.GetValue(v, time, true))
		{
			switch (mOperation)
			{
			case PS_Operation::SET:
				p->Rotation = v.data;
				break;

			case PS_Operation::ADD:
				p->Rotation += v.data * elapsedTime;
				break;

			case PS_Operation::SUB:
				p->Rotation -= v.data * elapsedTime;
				break;

			case PS_Operation::MUL:
				p->Rotation *= v.data;
				break;

			case PS_Operation::AVG:
				p->Rotation = (p->Rotation + v.data) / 2;
				break;
			}
		}
	}

}
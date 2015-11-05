#include "PSModifierScale.h"

namespace Rad {

	ImplementRTTI(PS_ModifierScale, PS_ModifierKeyController);

	DF_PROPERTY_BEGIN(PS_ModifierScale)
		DF_PROPERTY_ENUM(PS_ModifierScale, mOperation, "", "Operation", PS_Operation)
		DF_PROPERTY_EX(PS_ModifierScale, mKeyController, "", "KeyController", "PT_KeyController", PT_UserData)
	DF_PROPERTY_END()

	PS_ModifierScale::PS_ModifierScale()
		: mOperation(PS_Operation::SET)
	{
	}

	PS_ModifierScale::~PS_ModifierScale()
	{
	}

	void PS_ModifierScale::Modify(Particle * p, float elapsedTime)
	{
		float time = 1 - p->Life * p->InitLife.y;

		KF_Float3 v;
		if (mKeyController.GetValue(v, time, true))
		{
			switch (mOperation)
			{
			case PS_Operation::SET:
				p->Size = v.data;
				break;

			case PS_Operation::ADD:
				p->Size += v.data * elapsedTime;
				break;

			case PS_Operation::SUB:
				p->Size -= v.data * elapsedTime;
				break;

			case PS_Operation::MUL:
				p->Size = p->InitSize * v.data;
				break;

			case PS_Operation::AVG:
				p->Size = (p->Size + v.data) / 2;
				break;
			}
		}
	}

}

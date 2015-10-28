#include "PSModifierColor.h"

namespace Rad {

	ImplementRTTI(PS_ModifierColor, PS_ModifierKeyController);

	DF_PROPERTY_BEGIN(PS_ModifierColor)
		DF_PROPERTY_ENUM(PS_ModifierColor, mOperation, "", "Operation", PS_Operation)
		DF_PROPERTY_EX(PS_ModifierColor, mKeyController, "", "KeyController", "PT_KeyController", PT_UserData)
	DF_PROPERTY_END()

	PS_ModifierColor::PS_ModifierColor()
		: mOperation(PS_Operation::SET)
	{
	}

	PS_ModifierColor::~PS_ModifierColor()
	{
	}

	void PS_ModifierColor::Modify(Particle * p, float elapsedTime)
	{
		float time = 1 - p->Life / p->MaxLife;

		KF_Color4 v;
		if (mKeyController.GetValue(v, time, true))
		{
			switch (mOperation)
			{
			case PS_Operation::SET:
				p->Color = v.data;
				break;

			case PS_Operation::ADD:
				p->Color += v.data * elapsedTime;
				break;

			case PS_Operation::SUB:
				p->Color -= v.data * elapsedTime;
				break;

			case PS_Operation::MUL:
				p->Color *= v.data;
				break;

			case PS_Operation::AVG:
				p->Color = (p->Color + v.data) / 2;
				break;
			}
		}
	}

}

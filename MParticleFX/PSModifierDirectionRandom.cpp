#include "PSModifierDirectionRandom.h"

namespace Rad {

	ImplementRTTI(PS_ModifierDirectionRandom, PS_Modifier);

	DF_PROPERTY_BEGIN(PS_ModifierDirectionRandom)
		DF_PROPERTY(PS_ModifierDirectionRandom, mRandomness, "", "Randomness", PT_Float)
		DF_PROPERTY(PS_ModifierDirectionRandom, mScope, "", "Scope", PT_Float)
	DF_PROPERTY_END()

	PS_ModifierDirectionRandom::PS_ModifierDirectionRandom()
		: mRandomness(0)
		, mScope(0)
	{
	}

	PS_ModifierDirectionRandom::~PS_ModifierDirectionRandom()
	{
	}

	void PS_ModifierDirectionRandom::SetRandomness(float v)
	{
		mRandomness = v;
	}

	void PS_ModifierDirectionRandom::SetScope(float v)
	{
		mScope = v;
	}

	void PS_ModifierDirectionRandom::Modify(Particle * p, float elapsedTime)
	{
		if (mScope > Math::RandomUnit())
		{
			Float3 Velocity = p->Direction * p->Speed;

			Velocity.x += Math::RandRange(-mRandomness, mRandomness) * elapsedTime;
			Velocity.y += Math::RandRange(-mRandomness, mRandomness) * elapsedTime;
			Velocity.z += Math::RandRange(-mRandomness, mRandomness) * elapsedTime;

			p->Direction = Velocity;
			p->Speed = p->Direction.Normalize();
		}
	}

}


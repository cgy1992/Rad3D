#include "PSEmitterBox.h"

namespace Rad {

	ImplementRTTI(PS_EmitterBox, PS_EmitterPoint);

	DF_PROPERTY_BEGIN(PS_EmitterBox)
		DF_PROPERTY(PS_EmitterBox, mExtend, "", "Extend", PT_Float3)
	DF_PROPERTY_END()

	PS_EmitterBox::PS_EmitterBox()
		: mExtend(0, 0, 0)
	{
	}

	PS_EmitterBox::~PS_EmitterBox()
	{
	}

	void PS_EmitterBox::SetExtend(const Float3 & ext)
	{
		mExtend = ext;
	}

	void PS_EmitterBox::InitParticle(Particle * p)
	{
		PS_Emitter::InitParticle(p);

		Float3 xAxis(1, 0, 0), yAxis(0, 1, 0), zAxis(0, 0, 1);

		float x = Math::RandRange(-1.0f, 1.0f);
		float y = Math::RandRange(-1.0f, 1.0f);
		float z = Math::RandRange(-1.0f, 1.0f);

		p->Position = mPosition + (x * xAxis + y * yAxis + z * zAxis) * mExtend;
	}

}
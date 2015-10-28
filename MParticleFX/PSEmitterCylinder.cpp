#include "PSEmitterCylinder.h"

namespace Rad {

	ImplementRTTI(PS_EmitterCylinder, PS_EmitterBox);

	DF_PROPERTY_BEGIN(PS_EmitterCylinder)
		DF_PROPERTY(PS_EmitterCylinder, mInnerScale, "", "InnerScale", PT_Float)
	DF_PROPERTY_END()

	PS_EmitterCylinder::PS_EmitterCylinder()
		: mInnerScale(0)
	{
	}

	PS_EmitterCylinder::~PS_EmitterCylinder()
	{
	}

	void PS_EmitterCylinder::InitParticle(Particle * p)
	{
		PS_Emitter::InitParticle(p);

		Float3 xAxis(1, 0, 0), yAxis(0, 1, 0), zAxis(0, 0, 1);

		float alpha = Math::RandRange(0.0f, 2 * PI);

		float s = Math::RandRange(mInnerScale, 1.0f);
		float x = s * Math::FastCos(alpha);
		float z = s * Math::FastSin(alpha);
		float y = Math::RandRange(-1.0f, 1.0f);
		
		p->Position = mPosition + (x * xAxis + y * yAxis + z * zAxis) * mExtend;
	}

	void PS_EmitterCylinder::SetInnerScale(float inner)
	{
		mInnerScale = inner;
	}

}
#include "PSEmitterEllipsoid.h"

namespace Rad {

	ImplementRTTI(PS_EmitterEllipsoid, PS_EmitterBox);

	DF_PROPERTY_BEGIN(PS_EmitterEllipsoid)
		DF_PROPERTY(PS_EmitterEllipsoid, mInnerScale, "", "InnerScale", PT_Float)
	DF_PROPERTY_END()

	PS_EmitterEllipsoid::PS_EmitterEllipsoid()
		: mInnerScale(0)
	{
	}

	PS_EmitterEllipsoid::~PS_EmitterEllipsoid()
	{
	}

	void PS_EmitterEllipsoid::InitParticle(Particle * p)
	{
		PS_Emitter::InitParticle(p);

		Float3 xAxis(1, 0, 0), yAxis(0, 1, 0), zAxis(0, 0, 1);

		float alpha = Math::RandRange(0.0f, 2 * PI);
		float beta = Math::RandRange(0.0f, PI);

		float s = Math::RandRange(mInnerScale, 1.0f);

		float sinbeta = Math::FastSin(beta);
		float x = s * Math::FastCos(alpha) * sinbeta;
		float y = s * Math::FastSin(alpha) * sinbeta;
		float z = s * Math::FastCos(beta);

		p->Position = mPosition + (x * xAxis + y * yAxis + z * zAxis) * mExtend;
	}

	void PS_EmitterEllipsoid::SetInnerScale(float inner)
	{
		mInnerScale = inner;
	}
}

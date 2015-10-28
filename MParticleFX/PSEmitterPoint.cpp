#include "PSEmitterPoint.h"

namespace Rad {
	
	ImplementRTTI(PS_EmitterPoint, PS_Emitter);

	PS_EmitterPoint::PS_EmitterPoint()
	{
	}

	PS_EmitterPoint::~PS_EmitterPoint()
	{
	}

	void PS_EmitterPoint::InitParticle(Particle * p)
	{
		PS_Emitter::InitParticle(p);

		p->Position = mPosition;
	}

}

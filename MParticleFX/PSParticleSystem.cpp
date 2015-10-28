#include "PSParticleSystem.h"
#include "PSParticleSystemSet.h"

namespace Rad {

	PS_ParticleSystem::PS_ParticleSystem(PS_ParticleSystemSet * parent)
		: mParent(parent)
	{
		mParticleSystem = new ParticleSystem;
		mParticleSystem->SetSource(mParent->_getSource());
	}

	PS_ParticleSystem::~PS_ParticleSystem()
	{
		delete mParticleSystem;
	}

	void PS_ParticleSystem::_update(Particle * p)
	{
		Float3 Position = p->Position;
		if (mParent->IsLocalSpace())
			Position.TransformA(mParent->GetParent()->GetWorldTM());

		mParticleSystem->SetPosition(Position);
		mParticleSystem->SetOpacity(p->Color.a);
		mParticleSystem->SetDirection(p->Direction);
	}

}
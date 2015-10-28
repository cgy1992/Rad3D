#include "PSGroup.h"
#include "PSGroupSet.h"

namespace Rad {

	PS_Group::PS_Group(PS_GroupSet * parent)
		: mParent(parent)
	{
		mParticleSystem = new ParticleSystem;
		mParticleSystem->SetUserData(0, (void *)1);
		mParticleSystem->SetSource(mParent->_getSource());

		for (int i = 0; i < mParticleSystem->GetSetCount(); ++i)
		{
			mParticleSystem->GetSet(i)->SetEnable(false);
		}

		int i0 = mParent->GetSetIndex().x;
		if (i0 >= 0 && i0 < mParticleSystem->GetSetCount())
			mParticleSystem->GetSet(i0)->SetEnable(true);

		int i1 = mParent->GetSetIndex().y;
		if (i1 >= 0 && i1 < mParticleSystem->GetSetCount())
			mParticleSystem->GetSet(i1)->SetEnable(true);

		int i2 = mParent->GetSetIndex().z;
		if (i2 >= 0 && i2 < mParticleSystem->GetSetCount())
			mParticleSystem->GetSet(i2)->SetEnable(true);

		int i3 = mParent->GetSetIndex().w;
		if (i3 >= 0 && i3 < mParticleSystem->GetSetCount())
			mParticleSystem->GetSet(i3)->SetEnable(true);
	}

	PS_Group::~PS_Group()
	{
		delete mParticleSystem;
	}

	void PS_Group::_update(Particle * p)
	{
		Float3 Position = p->Position;
		if (mParent->IsLocalSpace())
			Position.TransformA(mParent->GetParent()->GetWorldTM());

		mParticleSystem->SetPosition(Position);
		mParticleSystem->SetDirection(p->Direction);
		mParticleSystem->SetOpacity(p->Color.a);
	}

}
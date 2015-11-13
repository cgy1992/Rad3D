#include "PSGroup.h"
#include "PSGroupSet.h"

namespace Rad {

	PS_Group::PS_Group(PS_GroupSet * parent)
		: mParent(parent)
	{
		mParticleSystem = new ParticleSystem;

		int i0 = mParent->GetSetIndex().x;
		if (i0 >= 0 && i0 < mParent->GetParent()->GetSetCount() && mParent->GetParent()->GetSet(i0) != parent)
			mParticleSystem->CloneSet(mParent->GetParent()->GetSet(i0));

		int i1 = mParent->GetSetIndex().y;
		if (i1 >= 0 && i1 < mParent->GetParent()->GetSetCount() && mParent->GetParent()->GetSet(i1) != parent)
			mParticleSystem->CloneSet(mParent->GetParent()->GetSet(i1));

		int i2 = mParent->GetSetIndex().z;
		if (i2 >= 0 && i2 < mParent->GetParent()->GetSetCount() && mParent->GetParent()->GetSet(i2) != parent)
			mParticleSystem->CloneSet(mParent->GetParent()->GetSet(i2));

		int i3 = mParent->GetSetIndex().w;
		if (i3 >= 0 && i3 < mParent->GetParent()->GetSetCount() && mParent->GetParent()->GetSet(i3) != parent)
			mParticleSystem->CloneSet(mParent->GetParent()->GetSet(i3));
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
#include "PSLight.h"
#include "PSLightSet.h"

namespace Rad {

	PS_Light::PS_Light(PS_LightSet * parent)
		: mParent(parent)
	{
		mLight = new Light;
		mLight->SetLocalAabb(Aabb::Default);

		mLight->SetAttenParam(parent->GetAttenStart(), parent->GetAttenEnd(), parent->GetAttenFallOff());
		mLight->SetSpotParam(parent->GetSpotInner(), parent->GetSpotOuter(), parent->GetSpotFallOff());
	}

	PS_Light::~PS_Light()
	{
		delete mLight;
	}

	void PS_Light::_update(Particle * p)
	{
		Float3 Position = p->Position;
		if (mParent->IsLocalSpace())
			Position.TransformA(mParent->GetParent()->GetWorldTM());

		mLight->SetPosition(Position);
		mLight->SetDirection(p->Direction);
		mLight->SetDiffuse(Float3(p->Color.r, p->Color.g, p->Color.b));
	}

}
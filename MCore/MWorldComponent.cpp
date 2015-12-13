#include "MWorldComponent.h"
#include "MWorld.h"

namespace Rad {

	ImplementRTTI(WorldComponent, IObject);

	DF_PROPERTY_BEGIN(WorldComponent)
		DF_PROPERTY(WorldComponent, mName, "", "Name", PT_FixedString)
		DF_PROPERTY(WorldComponent, mEnable, "", "Enable", PT_Bool)
		DF_PROPERTY(WorldComponent, mOrder, "", "Order", PT_Int)
	DF_PROPERTY_END()

	WorldComponent::WorldComponent()
		: mEnable(true)
		, mOrder(0)
	{
		_inworld = false;
	}

	WorldComponent::~WorldComponent()
	{
	}

	void WorldComponent::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mEnable")
		{
			SetEnable(mEnable);
		}
		else if (p->name == "mOrder")
		{
			SetOrder(mOrder);
		}
		else
		{
			IObject::OnPropertyChanged(p);
		}
	}

	void WorldComponent::SetName(const FixedString32 & name)
	{
		mName = name;
	}

	void WorldComponent::SetEnable(bool b)
	{
		mEnable = b;
	}

	void WorldComponent::SetOrder(int order)
	{
		mOrder = order;

		if (_inworld)
		{
			World::Instance()->RemoveComponent(this, false);
			World::Instance()->AddComponent(this);
		}
	}

}
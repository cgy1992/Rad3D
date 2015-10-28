#include "MGUI_Component.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(Component, IComponent);

	Component::Component()
		: mUserData(NULL)
	{
	}

	Component::~Component()
	{
	}

	void Component::Attach(ComponentOwner * owner)
	{
		d_assert (KIND_OF(Widget, owner));

		IComponent::Attach(owner);
	}

	void Component::Detach()
	{
		IComponent::Detach();
	}

}}


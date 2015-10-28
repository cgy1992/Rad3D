#include "MComponent.h"
#include "MRoot.h"

namespace Rad {

	ImplementRTTI(ComponentOwner, IObject);

	ComponentOwner::ComponentOwner()
		: mComponentLinker(NULL)
	{
	}
	
	ComponentOwner::~ComponentOwner()
	{
		RemoveAllComponent();
	}

	void ComponentOwner::AddComponent(IComponent * cp)
	{
		d_assert (cp->GetOwner() == NULL);

		LINKER_APPEND(mComponentLinker, cp);

		cp->Attach(this);
	}

	void ComponentOwner::RemoveComponent(IComponent * cp)
	{
		d_assert (cp->GetOwner() == this);

		cp->Detach();

		LINKER_REMOVE(mComponentLinker, cp);

		delete cp;
	}

	void ComponentOwner::RemoveAllComponent()
	{
		while (mComponentLinker != NULL)
		{
			RemoveComponent(mComponentLinker);
		}
	}

	IComponent * ComponentOwner::FirstComponent()
	{
		IComponent * cp = mComponentLinker;

		while (cp != NULL)
		{
			if (LINKER_NEXT(cp) == NULL)
				return cp;

			cp = LINKER_NEXT(cp);
		}

		return NULL;
	}

	IComponent * ComponentOwner::NextComponent(IComponent * cp)
	{
		return LINKER_PREV(cp);
	}

	void ComponentOwner::UpdateComponent(float elapsedTime)
	{
		IComponent * cp = FirstComponent();
		while (cp != NULL)
		{
			IComponent * next = NextComponent(cp);

			if (cp->IsEnable())
			{
				if (cp->Update(elapsedTime) < 0)
					RemoveComponent(cp);
			}

			cp = next;
		}
	}

	void ComponentOwner::Serialize(Serializer & sl)
	{
		IObject::Serialize(sl);

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			IComponent * pComponent = FirstComponent();
			while (pComponent != NULL)
			{
				if (pComponent->Serializable())
				{
					OS << pComponent->GetRTTI()->TypeId();
					pComponent->Serialize(OS);
				}

				pComponent = NextComponent(pComponent);
			}

			OS << 0;
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int typeId = 0;
			while (IS.Read(&typeId, sizeof(int)) && typeId != 0)
			{
				IComponent * pComponent = Root::NEW_OBJECT_T<IComponent>(typeId);
				d_assert (pComponent != NULL);

				pComponent->Serialize(IS);

				AddComponent(pComponent);
			}
		}
	}

	//
	ImplementRTTI(IComponent, IObject);

	IComponent::IComponent()
		: mOwner(NULL)
		, mEnable(true)
	{
	}

	IComponent::~IComponent()
	{
	}

	void IComponent::Attach(ComponentOwner * owner)
	{
		d_assert (mOwner == NULL);

		mOwner = owner;
	}

	void IComponent::Detach()
	{
		d_assert (mOwner != NULL);

		mOwner = NULL;
	}

}
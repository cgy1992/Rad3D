#include "MComponent.h"
#include "MRoot.h"

namespace Rad {

	ImplementRTTI(ComponentOwner, IObject);

	ComponentOwner::ComponentOwner()
	{
	}
	
	ComponentOwner::~ComponentOwner()
	{
		RemoveAllComponent();
	}

	void ComponentOwner::AddComponent(IComponent * p)
	{
		d_assert (p->GetOwner() == NULL);

		int i;
		for (i = mComponents.Size(); i > 0 ; ++i)
		{
			if (mComponents[i - 1]->GetOrder() <= p->GetOrder())
				break;
		}

		mComponents.Insert(i, p);

		p->Attach(this);
	}

	void ComponentOwner::RemoveComponent(IComponent * p, bool _delete)
	{
		int i;
		for (i = 0; i < mComponents.Size(); ++i)
		{
			if (mComponents[i] == p)
				break;
		}

		d_assert (i < mComponents.Size());

		RemoveComponent(i, _delete);
	}

	void ComponentOwner::RemoveComponent(int i, bool _delete)
	{
		mComponents[i]->Detach();
		
		if (_delete)
			delete mComponents[i];

		mComponents.Erase(i);
	}

	void ComponentOwner::RemoveAllComponent()
	{
		for (int i = 0; i < mComponents.Size(); ++i)
		{
			mComponents[i]->Detach();
			delete mComponents[i];
		}

		mComponents.Clear();
	}

	void ComponentOwner::ResortComponent(IComponent * p)
	{
		int i;
		for (i = 0; i < mComponents.Size(); ++i)
		{
			if (mComponents[i] == p)
				break;
		}

		d_assert (i < mComponents.Size());

		mComponents.Erase(i);

		for (i = mComponents.Size(); i > 0 ; ++i)
		{
			if (mComponents[i - 1]->GetOrder() < p->GetOrder())
				break;
		}

		mComponents.Insert(i, p);
	}

	void ComponentOwner::UpdateComponent(float elapsedTime)
	{
		for (int i = 0; i < mComponents.Size(); ++i)
		{
			IComponent * p = mComponents[i];
			if (p->IsEnable())
			{
				if (p->Update(elapsedTime) < 0)
					RemoveComponent(i--);
			}
		}
	}

	void ComponentOwner::Serialize(Serializer & sl)
	{
		IObject::Serialize(sl);

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			for (int i = 0; i < mComponents.Size(); ++i)
			{
				IComponent * pComponent = mComponents[i];
				if (pComponent->Serializable())
				{
					OS << pComponent->GetRTTI()->TypeId();
					pComponent->Serialize(OS);
				}
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
		, mOrder(0)
	{
	}

	IComponent::~IComponent()
	{
	}

	void IComponent::SetEnable(bool b)
	{
		mEnable = b;
	}

	void IComponent::SetOrder(int order)
	{
		mOrder = order;

		if (mOwner != NULL)
			mOwner->ResortComponent(this);
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
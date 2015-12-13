/*
*	Component
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MObject.h"
#include "MLinker.h"

namespace Rad {

	//
	class IComponent;

	class M_ENTRY ComponentOwner : public IObject
	{
		DECLARE_RTTI();

	public:
		ComponentOwner();
		virtual ~ComponentOwner();

		void
			AddComponent(IComponent * p);
		void
			RemoveComponent(IComponent * p, bool _delete = true);
		void
			RemoveComponent(int i, bool _delete = true);
		void
			RemoveAllComponent();
		void
			ResortComponent(IComponent * p);
		IComponent *
			GetComponent(int i) { return mComponents[i]; }
		int
			GetComponentCount() { return mComponents.Size(); }

		void
			UpdateComponent(float elapsedTime);

		virtual void
			Serialize(Serializer & sl);

	protected:
		Array<IComponent *, t_alloc_pool<IComponent *> > mComponents;
	};

	//
	class M_ENTRY IComponent : public IObject
	{
		DECLARE_RTTI();

	public:
		IComponent();
		virtual ~IComponent();

		ComponentOwner *
			GetOwner() { return mOwner; }
		template <class T> T *
			GetOwnerT() { d_assert(KIND_OF(T, obj)); return (T *)mOwner; }

		virtual void 
			SetEnable(bool b);
		bool 
			IsEnable() { return mEnable; }

		void
			SetOrder(int order);
		int
			GetOrder() { return mOrder; }

		virtual void
			Attach(ComponentOwner * owner);
		virtual void
			Detach();

		virtual int
			Update(float elapsedTime) = 0;

	protected:
		ComponentOwner * mOwner;
		bool mEnable;
		int mOrder;
	};
}
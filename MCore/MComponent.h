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
			AddComponent(IComponent * cp);
		void
			RemoveComponent(IComponent * cp);
		void
			RemoveAllComponent();
		IComponent *
			FirstComponent();
		IComponent *
			NextComponent(IComponent * cp);

		void
			UpdateComponent(float elapsedTime);

		virtual void
			Serialize(Serializer & sl);

	protected:
		IComponent * mComponentLinker;
	};

	//
	class M_ENTRY IComponent : public IObject
	{
		DECLARE_RTTI();
		DECLARE_LINKER(IComponent);

	public:
		IComponent();
		virtual ~IComponent();

		ComponentOwner *
			GetOwner() { return mOwner; }
		template <class T> T *
			GetOwnerT() { d_assert(KIND_OF(T, obj)); return (T *)mOwner; }

		virtual void 
			SetEnable(bool b) { mEnable = b; }
		bool 
			IsEnable() { return mEnable; }

		virtual void
			Attach(ComponentOwner * owner);
		virtual void
			Detach();

		virtual int
			Update(float elapsedTime) = 0;

	protected:
		ComponentOwner * mOwner;
		bool mEnable;
	};
}
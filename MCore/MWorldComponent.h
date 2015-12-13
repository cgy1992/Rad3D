#pragma once

#include "MTypes.h"

namespace Rad {

	class M_ENTRY WorldComponent : public IObject
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(IObject);

		friend class World;

		bool _inworld;

	public:
		WorldComponent();
		virtual ~WorldComponent();

		virtual void
			OnPropertyChanged(const Property * p);

		virtual void 
			SetEnable(bool b);
		bool 
			IsEnable() const { return mEnable; }

		void
			SetName(const FixedString32 & name);
		const FixedString32 &
			GetName() const { return mName; }

		void
			SetOrder(int order);
		int
			GetOrder() const { return mOrder; }

		virtual int
			Update(float elapsedTime) { return 0; };

	protected:
		FixedString32 mName;
		bool mEnable;
		int mOrder;
	};

}
/*
*	NodeTracker
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MAnimation.h"

namespace Rad {

	class M_ENTRY NodeTracker : public IComponent
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(IComponent);

	public:
		KeyControllerTransform mKeyController;
		bool mLooped;
		float mPosition;

	public:
		NodeTracker();
		virtual ~NodeTracker();

		virtual void
			Serialize(Serializer & sl);

		virtual void
			Attach(ComponentOwner * owner);

		virtual int
			Update(float elapsedTime);

		void 
			SetLooped(bool looped);
		bool 
			GetLooped() { return mLooped; }

		void 
			SetPosition(float position);
		float
			GetPosition() { return mPosition; }

		KeyControllerTransform *
			GetKeyController() { return &mKeyController; }
	};
}
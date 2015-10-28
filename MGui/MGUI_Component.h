/*
*	MGUI Component
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"

namespace Rad { namespace MGUI {

	class Widget;

	class MGUI_ENTRY Component : public IComponent
	{
		DECLARE_POOL_ALLOC();
		DECLARE_RTTI();

	public:
		Component();
		virtual ~Component();

		virtual void 
			Attach(ComponentOwner * owner);
		virtual void 
			Detach();

		Widget * 
			GetWidget() { return (Widget *)mOwner; }

		void 
			SetUserData(void * userData) { mUserData = userData; }
		void * 
			GetUserData() { return mUserData; }

	protected:
		void * mUserData;
	};

}}

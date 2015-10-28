/*
*	MGUI Factory
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRoot.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Factory : public IObjectFactory
	{
		DECLARE_RTTI();

	public:
		Factory() {}
		virtual ~Factory(){}

		virtual IObject * 
			New() { d_assert(0); return NULL; }
		virtual IObject * 
			New(int count) { d_assert(0); return NULL; }

		virtual Widget * 
			NewWidget(const LookFeel * _lookfeel, Widget * _parent) = 0;
	};

	template<class T>
	class TFactory : public Factory
	{
	public:
		TFactory() {}
		virtual ~TFactory() {}

		virtual Widget * 
			NewWidget(const LookFeel * _lookfeel, Widget * _parent) { return new T(_lookfeel, _parent); }

		virtual const RTTI_INFO * 
			GetRTTI() { return &T::RTTI; }
	};

	template <class T>
	inline void REG_WIDGET()
	{
		Root::Instance()->RegisterObject(T::RTTI.TypeId(), new TFactory<T>);
	}

	template <class T>
	inline T * NEW_WIDGET(const char * className, const LookFeel * _lookfeel, Widget * _parent)
	{
		IObjectFactory * factory = Root::Instance()->GetObjectFactory(className);
		if (factory)
		{
			d_assert (KIND_OF(Factory, factory));
			Widget * widget = static_cast<Factory *>(factory)->NewWidget(_lookfeel, _parent);

			d_assert(KIND_OF(T, widget));

			return (T *)widget;
		}

		return NULL;
	}

}}
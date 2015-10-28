/*
*	Object
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRml.h"
#include "MRTTI.h"
#include "MEvent.h"
#include "MProperty.h"
#include "MSerializer.h"

namespace Rad {

	struct M_ENTRY IObject
	{
		DECLARE_ALLOC();
		DECLARE_RTTI_ROOT(IObject);

	public:
		tEvent2<IObject *, const Property *> E_PropertyChanged;

	public:
		IObject();
		virtual ~IObject();

		virtual int 
			GetPropertySize() const { return 0; };
		int 
			GetPropertySizeLocal() const { return 0; }
		virtual const Property * 
			GetProperty(int index) const { return NULL; }
		virtual const Property *
			GetProperty(const char * name) const { return NULL; }
		virtual const Property *
			GetPropertyById(int id) const { return NULL; }
		virtual const void * 
			GetPropertyData(const Property * p) const;
		virtual void 
			SetPropertyData(const Property * p, const void * data);
		virtual void
			OnPropertyChanged(const Property * p) {}

		virtual void
			Copy(const IObject * from);
		virtual IObject *
			Clone() { return NULL; }

		virtual bool
			Serializable() { return true; }
		virtual void
			Serialize(Serializer & sl);
		virtual void
			Serialize(rml_node * root, bool isOut);

	protected:
		int mVersion;
	};

	//
	class M_ENTRY IObjectFactory
	{
		DECLARE_RTTI_ROOT(IObjectFactory);

	public:
		IObjectFactory() {}
		virtual ~IObjectFactory() {}

		virtual IObject * 
			New() = 0;
		virtual IObject * 
			New(int count) = 0;

		virtual const RTTI_INFO *
			GetRTTI() = 0;
	};

	template<class T>
	class TObjectFactory : public IObjectFactory
	{
	public:
		TObjectFactory() {}
		virtual ~TObjectFactory() {}

		virtual IObject * 
			New() { return new T; }
		virtual IObject * 
			New(int count) { return new T[count]; }

		virtual const RTTI_INFO * 
			GetRTTI() { return &T::RTTI; }
	};

}


/*
*	Root
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSet.h"
#include "MMap.h"
#include "MTimer.h"
#include "MObject.h"
#include "MLog.h"

namespace Rad {

	class M_ENTRY Root : public Singleton<Root>
	{
	public:
		template <class T>
		static void 
			REG_OBJECT_T();

		template <class T>
		static void 
			UNREG_OBJECT_T();

		template <class T>
		static T * 
			NEW_OBJECT_T(const char * className);

		template <class T>
		static T * 
			NEW_OBJECT_T(int typeId);

		static IObject * 
			NEW_OBJECT(const char * className);
		static IObject * 
			NEW_OBJECT(int typeId);

		static void 
			ENUM_OBJECT_FACTORY(Array<FixedString32> & classNames, const char * baseClass, bool includeBase);

	public:
		static bool msInEditor;

		static void
			SetInEditor(bool b) { msInEditor = b;}
		static bool 
			InEditor() { return msInEditor; }

		struct Config
		{
			int nPool16;
			int nPool32;
			int nPool64;
			int nPool128;
			int nPool256;

			bool bRadC;

			Config()
			{
				nPool16 = 1024;
				nPool32 = 1024;
				nPool64 = 1024;
				nPool128 = 1024;
				nPool256 = 1024;

				bRadC = true;
			}
		};

	public:
		Root(const Config * config = NULL, const char * log = "log.txt");
		~Root();

		void
			RegisterObject(int typeId, IObjectFactory * factory);
		void
			UnregisterObject(int typeId);
		IObjectFactory *
			GetObjectFactory(const char * className);
		IObjectFactory *
			GetObjectFactory(int typeId);
		void
			EnumObjectFactory(Array<FixedString32> & classNames, const char * baseClass, bool includeBase);

		void 
			Update();

		float 
			GetTime() { return mTimer.ElapsedTime(); }
		float 
			GetFrameTime() { return mFrameTime; }
		float 
			GetLastTime() { return mLastFrameTime; }
		float 
			GetFPS() { return mFPS; }

	protected:
		Map<int, IObjectFactory *> mObjectFactoryMap2;

		Config mConfig;
		Log * mLog;

		Timer mTimer;
		float mLastFrameTime;
		float mFrameTime;
		float mLastFPSCalcTime;
		float mFPS;
	};

	//
	template <class T>
	inline void Root::REG_OBJECT_T()
	{
		Root::Instance()->RegisterObject(T::RTTI.TypeId(), new TObjectFactory<T>);
	}

	template <class T>
	inline void Root::UNREG_OBJECT_T()
	{
		Root::Instance()->RegisterObject(T::RTTI.TypeId());
	}

	template<class T>
	inline T * Root::NEW_OBJECT_T(const char * className)
	{
		IObjectFactory * factory = Root::Instance()->GetObjectFactory(className);
		if (factory)
		{
			IObject * obj = factory->New();

			d_assert (KIND_OF(T, obj));

			return (T *)obj;
		}

		return NULL;
	}

	template<class T>
	inline T * Root::NEW_OBJECT_T(int typeId)
	{
		IObjectFactory * factory = Root::Instance()->GetObjectFactory(typeId);
		if (factory)
		{
			IObject * obj = factory->New();

			d_assert (KIND_OF(T, obj));

			return (T *)obj;
		}

		return NULL;
	}

	inline IObject * Root::NEW_OBJECT(const char * className)
	{
		IObjectFactory * factory = Root::Instance()->GetObjectFactory(className);
		if (factory)
		{
			return factory->New();
		}

		return NULL;
	}

	inline IObject * Root::NEW_OBJECT(int typeId)
	{
		IObjectFactory * factory = Root::Instance()->GetObjectFactory(typeId);
		if (factory)
		{
			return factory->New();
		}

		return NULL;
	}

	inline void Root::ENUM_OBJECT_FACTORY(Array<FixedString32> & classNames, const char * baseClass, bool includeBase)
	{
		Root::Instance()->EnumObjectFactory(classNames, baseClass, includeBase);
	}

}

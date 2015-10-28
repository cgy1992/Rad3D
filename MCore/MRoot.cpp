#include "MRoot.h"
#include "MCodeTimer.h"
#include "RadC.h"

namespace Rad {

	ImplementSingleton(Root);

	bool Root::msInEditor = false;

	Root::Root(const Config * config, const char * log)
		: mLastFrameTime(0)
		, mFrameTime(0)
		, mLastFPSCalcTime(0)
		, mFPS(0)
	{
		if (config != NULL)
			mConfig = *config;

		Memory::Init(
			mConfig.nPool16,
			mConfig.nPool32,
			mConfig.nPool64,
			mConfig.nPool128,
			mConfig.nPool256);

		Math::Init();

		mLog = new Log(log);

		mTimer.Start();

		if (mConfig.bRadC)
			radc_init();
	}

	Root::~Root()
	{
		if (mConfig.bRadC)
			radc_shutdown();

		for (int i = 0; i < mObjectFactoryMap2.Size(); ++i)
		{
			delete mObjectFactoryMap2[i].value;;
		}
		mObjectFactoryMap2.Clear();

		CodeTimer::OuputAll();
		Memory::OutputMemoryPoolStatus();

		delete mLog;

		Math::Shutdown();
		Memory::Shutdown();
	}

	void Root::Update()
	{
		float time = GetTime();

		mFrameTime = Max(0.0f, time - mLastFrameTime);
		mLastFrameTime = time;

		if (time - mLastFPSCalcTime > 1.0f)
		{
			mFPS = mFrameTime > 0 ? 1.0f / mFrameTime : 0;
			mLastFPSCalcTime = time;
		}
	}

	void Root::RegisterObject(int typeId, IObjectFactory * factory)
	{
		d_assert (mObjectFactoryMap2.Find(typeId) == -1);

		mObjectFactoryMap2.Insert(typeId, factory);
	}

	void Root::UnregisterObject(int typeId)
	{
		int index = mObjectFactoryMap2.Find(typeId);

		d_assert (index != -1);

		mObjectFactoryMap2.Erase(index);
	}

	IObjectFactory * Root::GetObjectFactory(const char * className)
	{
		return GetObjectFactory(Math::Crc32(className));
	}

	IObjectFactory * Root::GetObjectFactory(int typeId)
	{
		int index = mObjectFactoryMap2.Find(typeId);

		return index != -1 ? mObjectFactoryMap2[index].value : NULL;
	}

	bool _InheritOf(const RTTI_INFO * _info, const char * supperClass, bool _includeBase)
	{
		const RTTI_INFO * p = _includeBase ? _info : _info->Base();
		while (p != NULL)
		{
			if (strcmp(p->Name(), supperClass) == 0)
				break;

			p = p->Base();
		}

		return p != NULL;
	}

	void Root::EnumObjectFactory(Array<FixedString32> & classNames, const char * baseClass, bool includeBase)
	{
		d_assert (baseClass);

		FixedSet<FixedString32, 128> names; 
		for (int i = 0; i < mObjectFactoryMap2.Size(); ++i)
		{
			const RTTI_INFO * info = mObjectFactoryMap2[i].value->GetRTTI();
			if (_InheritOf(info, baseClass, includeBase))
			{
				names.Insert(info->Name());
			}
		}

		for (int i = 0; i < names.Size(); ++i)
		{
			classNames.PushBack(names[i]);
		}
	}

}

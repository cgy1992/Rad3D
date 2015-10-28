/*
*	Prefab Manager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMap.h"
#include "MMath2.h"
#include "MSingleton.h"
#include "MResource.h"

namespace Rad {

	class M_ENTRY PrefabSource : public ResourceDataStream
	{
	public:
		PrefabSource(const String & name, const String & source);
		virtual ~PrefabSource();

		virtual void 
			Release();
	};

	typedef SmartPtr<PrefabSource> PrefabSourcePtr;

	//
	class M_ENTRY PrefabManager : public Singleton<PrefabManager>
	{
	public:
		PrefabManager();
		~PrefabManager();

		PrefabSourcePtr 
			LoadPrefab(const String & filename, float priority = 0);
		PrefabSourcePtr 
			GetPrefab(const String & filename);

		void 
			_delPrefab(PrefabSource * p);

	protected:
		PrefabSourcePtr
			_find(Hash2 hash, const String & filename);

	protected:
		Map<Hash2, PrefabSource *> mSourceMap;
	};

}
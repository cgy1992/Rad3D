#include "MPrefabManager.h"

namespace Rad {

	PrefabSource::PrefabSource(const String & name, const String & source)
		: ResourceDataStream(name, source)
	{
	}

	PrefabSource::~PrefabSource()
	{
	}

	void PrefabSource::Release()
	{
		PrefabManager::Instance()->_delPrefab(this);
	}

	//
	ImplementSingleton(PrefabManager);

	PrefabManager::PrefabManager()
	{
	}

	PrefabManager::~PrefabManager()
	{
		d_assert (mSourceMap.Empty() && "Prefab has already clear?");
	}

	PrefabSourcePtr PrefabManager::LoadPrefab(const String & filename, float priority)
	{
		Hash2 hash(filename.c_str());

		PrefabSourcePtr p = _find(hash, filename);
		if (p == NULL)
		{
			p = new PrefabSource(filename, filename);
			p->SetPriority(priority);

			mSourceMap.Insert(hash, p.c_ptr());
		}

		if (priority < 0)
		{
			p->EnsureLoad();
		}
		else
		{
			p->Load();
		}

		return p;
	}

	PrefabSourcePtr PrefabManager::_find(Hash2 hash, const String & filename)
	{
		int i = mSourceMap.Find(hash);

		d_assert (i == - 1 || mSourceMap[i].value->GetName() == filename);

		return i != -1 ? mSourceMap[i].value : NULL;
	}

	PrefabSourcePtr PrefabManager::GetPrefab(const String & filename)
	{
		return _find(Hash2(filename.c_str()), filename);
	}

	void PrefabManager::_delPrefab(PrefabSource * p)
	{
		Hash2 hash(p->GetName().c_str());
		int i = mSourceMap.Find(hash);

		d_assert (i != -1);

		delete mSourceMap[i].value;
		mSourceMap.Erase(i);
	}

}
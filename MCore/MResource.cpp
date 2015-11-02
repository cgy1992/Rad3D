#include "MResource.h"
#include "MResourceManager.h"

namespace Rad {

    Resource::Resource(const String & name, const String & sourceName)
        : mName(name)
		, mSourceName(sourceName)
		, mPriority(0)
		, mLoadState(UNLOADED)
		, mDependenceLinker(NULL)
    {
    }

    Resource::~Resource()
    {
		d_assert (mDependenceLinker == NULL && "Do you forget unload???");
    }

	void Resource::Load()
	{
		if (mLoadState != Resource::UNLOADED)
			return ;

		mLoadState = LOADING;

		if (CanLoad())
			ResourceManager::Instance()->GetResourceLoader()->Load(this);
		else
			OnLoad();
	}

	void Resource::EnsureLoad()
	{
		if (mLoadState == Resource::LOADED)
			return ;

		if (CanLoad())
		{
			if (mLoadState == LOADING)
				Unload();

			mLoadState = LOADING;
			ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
		}
		else
		{
			mLoadState = Resource::LOADED;
		}
	}

	void Resource::Unload()
	{
		if (mLoadState == LOADED ||
			mLoadState == LOADING)
			OnUnload();
	}

	void Resource::Reload()
	{
		if (CanLoad())
		{
			Unload();
			Load();
		}
	}

	void Resource::OnLoad()
	{
		d_assert (mDependenceLinker == NULL);

		if (mLoadState == LOADING)
		{
			mLoadState = LOADED;

			E_Loaded();
		}
	}
	
	void Resource::OnUnload()
	{
		if (mLoadState == LOADED)
			E_Unload();

		mLoadState = UNLOADED;

		_removeAllDependence();
	}

	void Resource::_addDependence(Resource * pSubResource)
	{
		LINKER_APPEND(mDependenceLinker, new ResourceDependence(this, pSubResource));
	}

	void Resource::_removeDependence(ResourceDependence * pDependence)
	{
		LINKER_REMOVE(mDependenceLinker, pDependence);
		delete pDependence;

		if (mLoadState == LOADING && _getDependences() == NULL)
			OnLoad();
	}

	void Resource::_removeAllDependence()
	{
		ResourceDependence * depend = mDependenceLinker;
		while (depend)
		{
			ResourceDependence * next = LINKER_NEXT(depend);

			delete depend;

			depend = next;
		}

		mDependenceLinker = NULL;
	}

	ResourceDependence * Resource::_getDependences()
	{
		return mDependenceLinker;
	}





	ResourceDataStream::ResourceDataStream(const String & name, const String & sourceName)
		: Resource(name, sourceName)
	{
	}

	ResourceDataStream::~ResourceDataStream()
	{
	}

	void ResourceDataStream::_loadImp(DataStreamPtr stream)
	{
		mDataStream = stream;

		E_LoadImp(stream);
	}
}
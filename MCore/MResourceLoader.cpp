#include "MResourceLoader.h"
#include "MResourceManager.h"

namespace Rad {

	ResourceLoader::ResourceLoader()
	{
	}

	ResourceLoader::~ResourceLoader()
	{
	}

	void ResourceLoader::Load(ResourcePtr pResource)
	{
		ForceLoad(pResource);
	}

	void ResourceLoader::ForceLoad(ResourcePtr pResource)
	{
		if (pResource->GetLoadState() == Resource::LOADED)
			return ;

		const char * source = pResource->GetSourceName().c_str();

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

		pResource->_load(stream);

		d_assert (pResource->_getDependences() == NULL);

		pResource->OnLoad();
	}

}
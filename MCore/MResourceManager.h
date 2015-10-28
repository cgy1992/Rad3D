/*
*	ResourceManager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MArray.h"
#include "MArchive.h"
#include "MPKArchive.h"
#include "MSingleton.h"
#include "MResource.h"
#include "MResourceLoader.h"
#include "MResourceLoaderMT.h"

namespace Rad {

	class M_ENTRY ResourceManager : public Singleton<ResourceManager>
	{
		typedef Array<Archive *> ArchiveArray;

	public:
		ResourceManager();
		~ResourceManager();

		void
			InsertArchive(int i, Archive * archive);
		void 
			AddArchive(Archive * archive);
		void 
			RemoveArchive(const String & name);
		Archive * 
			GetArchive(const String & name);

		DataStreamPtr 
			OpenResource(const String & source);
		bool 
			HasResource(const String & source);
		DataStreamPtr 
			OpenFile(const String & filename);

		String 
			GetRelativeSourceName(const String & absFilename);
		String 
			GetAbsoluteSourceName(const String & filename);

		void 
			SetResourceLoader(ResourceLoader * loader);
		ResourceLoader * 
			GetResourceLoader();

	protected:
		ArchiveArray mArchives;
		ResourceLoader * mResourceLoader;
		Mutex mMutex;
	};

}

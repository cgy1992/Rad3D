#include "MResourceManager.h"
#include "MLog.h"

namespace Rad {

	ImplementSingleton(ResourceManager);

	ResourceManager::ResourceManager()
	{
		mResourceLoader = new ResourceLoader;
	}

	ResourceManager::~ResourceManager()
	{
		for (int i = 0; i < mArchives.Size(); ++i)
			delete mArchives[i];

		mArchives.Clear();

		safe_delete(mResourceLoader);
	}

	void ResourceManager::InsertArchive(int i, Archive * archive)
	{
		mArchives.Insert(i, archive);
	}

	void ResourceManager::AddArchive(Archive * archive)
	{
		mArchives.PushBack(archive);
	}

	void ResourceManager::RemoveArchive(const String & name)
	{
		for (int i = 0; i < mArchives.Size(); ++i)
		{
			if (mArchives[i]->GetName() == name)
			{
				delete mArchives[i];
				mArchives.Erase(i);
				return ;
			}
		}
	}

	Archive * ResourceManager::GetArchive(const String & name)
	{
		for (int i = 0; i < mArchives.Size(); ++i)
		{
			if (mArchives[i]->GetName() == name)
				return mArchives[i];
		}

		return NULL;
	}

	DataStreamPtr ResourceManager::OpenResource(const String & source)
	{
		MUTEX_ENTER(mMutex);

		DataStreamPtr stream = NULL;

		for (int i = 0; i < mArchives.Size(); ++i)
		{
			stream = mArchives[i]->Open(source);
			if (stream != NULL)
				return stream;
		}

		d_log("!: file '%s' open failed.", source.c_str());

		return stream;
	}

	DataStreamPtr ResourceManager::OpenFile(const String & filename)
	{
		MUTEX_ENTER(mMutex);

		if (FileHelper::Exist(filename))
			return DataStreamPtr(new FileStream(filename));
		else
			return NULL;
	}

	bool ResourceManager::HasResource(const String & source)
	{
		for (int i = 0; i < mArchives.Size(); ++i)
		{
			if (mArchives[i]->Exist(source))
				return true;
		}

		return false;
	}

	String ResourceManager::GetRelativeSourceName(const String & absFilename)
	{
		if (absFilename == "")
			return "";

		String file = absFilename;

		file.Replace('/', '\\');

		const char * testFile = file.c_str();
		int length = file.Length();

		bool exist = false;

		while (length)
		{
			if (testFile[length - 1] == '\\')
			{
				const char * tfile = testFile + length;

				exist = ResourceManager::Instance()->HasResource(tfile);

				if (exist)
					break;
			}

			--length;
		}

		if (!exist)
			exist = ResourceManager::Instance()->HasResource(testFile);

		if (!exist)
			return "";

		testFile += length;

		return testFile;
	}

	String ResourceManager::GetAbsoluteSourceName(const String & filename)
	{
		if (filename == "")
			return "";

		DataStreamPtr stream = NULL;

		for (int i = 0; i < mArchives.Size(); ++i)
		{
			Archive * pArchive = mArchives[i];

			if (TYPE_OF(FilePathArchive, pArchive) && mArchives[i]->Exist(filename))
			{
				return pArchive->GetName() + "/" + filename;
			}
		}

		return "";
	}

	void ResourceManager::SetResourceLoader(ResourceLoader * loader)
	{
		if (mResourceLoader != loader)
		{
			safe_delete (mResourceLoader);

			mResourceLoader = loader;
		}

		if (mResourceLoader == NULL)
		{
			mResourceLoader = new ResourceLoader;
		}
	}

	ResourceLoader * ResourceManager::GetResourceLoader()
	{
		return mResourceLoader;
	}

}

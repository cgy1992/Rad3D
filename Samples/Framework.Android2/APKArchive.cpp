#include "APKArchive.h"
#include "stdafx.h"

namespace Rad {

	int ReadAPKFile(char ** data, const char * filename)
	{
		d_assert (ANDROID_ACTIVITY != NULL);

		AAsset * pAsset = AAssetManager_open(ANDROID_ACTIVITY->assetManager, filename, AASSET_MODE_UNKNOWN);
		if (pAsset == NULL)
			return -1;

		size_t size = AAsset_getLength(pAsset);
		if (size > 0 && data != NULL)
		{
			unsigned char * pd = new unsigned char[size];
			int read_count = AAsset_read(pAsset, pd, size);
			if(read_count <= 0)
			{
				d_log("?: Read APK File '%s'.", filename);

				delete[] pd;
				pd = NULL;
				size = 0;
			}

			*data = (char *)pd;
		}

		AAsset_close(pAsset);

		return size;
	}

	APKArchive::APKArchive(const String & floderName)
		: Archive(floderName)
	{
	}

	APKArchive::~APKArchive()
	{
	}

	DataStreamPtr APKArchive::Open(const String & source)
	{
		String filename = mName + "/" + source;
		filename.Replace('\\', '/');

		char * data = NULL;
		int size = ReadAPKFile(&data, filename.c_str());
		if (size > 0)
		{
			return new MemoryStream((byte *)data, size, true);
		}

		return NULL;
	}

	bool APKArchive::Exist(const String & source)
	{
		String filename = mName + "/" + source;
		filename.Replace('\\', '/');

		return ReadAPKFile(NULL, filename.c_str()) >= 0;
	}
}
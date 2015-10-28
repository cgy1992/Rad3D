#include "APKArchive.h"
#include "App.h"
#include "NDK_Utils.h"

namespace Rad {

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

		void * data = NULL;
		int size = NDK_Uitl::ReadAPKFile(&data, filename.c_str());
		if (size > 0)
			return new MemoryStream((byte *)data, size, true);

		return NULL;
	}

	bool APKArchive::Exist(const String & source)
	{
		String filename = mName + "/" + source;
		filename.Replace('\\', '/');

		return NDK_Uitl::ReadAPKFile(NULL, filename.c_str()) >= 0;
	}
}
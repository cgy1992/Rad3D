#include "MArchive.h"
#include <dirent.h>

using namespace Rad;

ImplementRootRTTI(Archive);

Archive::Archive(const String & name)
: mName(name)
{
}

Archive::~Archive()
{
}

const String & Archive::GetName() const
{
    return mName;
}




ImplementRTTI(FilePathArchive, Archive);

FilePathArchive::FilePathArchive(const String & name)
	: Archive(name)
{
}

FilePathArchive::~FilePathArchive()
{
}

DataStreamPtr FilePathArchive::Open(const String & filename)
{
	String fullname = mName + "/" + filename;

	if (filename.Length() > 0 && FileHelper::Exist(fullname))
		return new FileStream(fullname);

	return NULL;
}

bool FilePathArchive::Exist(const String & filename)
{
	String fullname = mName + "/" + filename;

	return FileHelper::Exist(fullname);
}
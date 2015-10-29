/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#include <iostream>
#include "MCore.h"
#include "MPKArchive.h"

using namespace Rad;

extern Set<FixedString32> gUncompressed;
extern Array<MPKFileInfo> gFileInfo; 
extern Array<FixedString256> gFilenames;
extern FixedString256 gOutputPath;

dword PK2_SIZE = 0;
dword PK2_MAXSIZE = 0;
int PK2_FILE_COUNT = 0;

void pk2_begin()
{
	gFileInfo.Clear();
	gFilenames.Clear();

	PK2_SIZE = 0;
	PK2_MAXSIZE = 0;
	PK2_FILE_COUNT = 0;
}

void pk2_prepare(const FixedString256 & path, const FixedString256 & floderName, const FixedString256 & prefixName)
{
	FileSystem fs;
	fs.Open(path.c_str());

	while (fs.NextFile())
	{
		FixedString256 filename = fs.FileName();
		filename.ToLower();

		if (filename == "." || filename == "..")
			continue;

		if (fs.FileIsDir())
		{
			FixedString256 fullname = path + "/" + filename;

			pk2_prepare(fullname, floderName, prefixName + filename + "/");
		}
		else
		{
			++PK2_FILE_COUNT;
		}
	}
}

void pk2_create(const FixedString256 & filename)
{
	FixedString256 path = gOutputPath + "/" + filename + ".MPK";
	FileHelper::MakeDir(FileHelper::GetFileDir(path.c_str()));

	FILE * fp = fopen(path.c_str(), "wb");
	if (!fp)
		return ;

	OSerializerF OS(fp, true);

	int FileMagic = MPK2_FILE_MAGIC;
	OS << FileMagic;

	int ckId = MPK_FILE_INFO;
	OS << ckId;

	OSerializerM2 OSM;
	OSM << PK2_FILE_COUNT;
	for (int i = 0; i < PK2_FILE_COUNT; ++i)
	{
		Hash2 hash;
		MPKFileInfo info;

		OSM << hash;
		OSM << info;
	}

	OS << OSM.Size();
	OS.Write(OSM.Data(), OSM.Size());

	OS << 0; // end

	PK2_SIZE = ftell_size(fp);
}

void pk2_convert(const FixedString256 & path, const FixedString256 & floderName, const FixedString256 & prefixName)
{
	FileSystem fs;
	fs.Open(path.c_str());

	while (fs.NextFile())
	{
		FixedString256 filename = fs.FileName();

		if (filename == "." || filename == "..")
			continue;

		if (fs.FileIsDir())
		{
			FixedString256 fullname = path + "/" + filename;

			pk2_convert(fullname, floderName, prefixName + filename + "/");
		}
		else
		{
			FixedString256 orgFileName = prefixName + filename;
			orgFileName.Replace('\\', '/');
			orgFileName.ToLower();

			FixedString256 iFilename = path + "/" + filename;
			FixedString256 oFilename = gOutputPath + "/" + floderName + ".MPK";

			Hash2 hash(orgFileName.c_str());
			MPKFileInfo fi;
			fi.pkid = 0;
			fi.flag = MPK_FLAG_COMPRESSED;
			fi.offset = PK2_SIZE;
			fi.size = 0;
			fi.unc_size = 0;

			if (gUncompressed.Find(FileHelper::GetExternName(orgFileName.c_str()).c_str()) != -1)
			{
				fi.flag &= ~MPK_FLAG_COMPRESSED;
			}

			if (MPK_Append(fi, iFilename.c_str(), oFilename.c_str(), (const byte *)&hash) > 0)
			{
				std::cout << "Convert " << iFilename.c_str() << " OK..."<< std::endl;

				gFileInfo.PushBack(fi);
				gFilenames.PushBack(orgFileName);

				PK2_SIZE += fi.size;
				if (PK2_SIZE > PK2_MAXSIZE)
				{
					std::cout << "Error: MPK2 " << oFilename.c_str() << " size to small..."<< std::endl;
					return ;
				}
			}
			else
			{
				d_assert (0);
				std::cout << "Convert " << iFilename.c_str() << " Failed..."<< std::endl;
			}
		}
	}
}

void pk2_save(const FixedString256 & filename)
{
	if (gFileInfo.Size() == 0)
		return ;

	FixedString256 path = gOutputPath + "/" + filename + ".MPK";

	FILE * fp = fopen(path.c_str(), "rb+");
	if (!fp)
		return ;

	d_assert (gFileInfo.Size() == PK2_FILE_COUNT);

	OSerializerF OS(fp, true);

	int FileMagic = MPK2_FILE_MAGIC;
	OS << FileMagic;

	int ckId = MPK_FILE_INFO;
	OS << ckId;

	OSerializerM2 OSM;
	OSM << PK2_FILE_COUNT;
	for (int i = 0; i < PK2_FILE_COUNT; ++i)
	{
		Hash2 hash(gFilenames[i].c_str());
		MPKFileInfo info = gFileInfo[i];

		OSM << hash;
		OSM << info;
	}

	OS << OSM.Size();
	OS.Write(OSM.Data(), OSM.Size());

	OS << 0; // end
}
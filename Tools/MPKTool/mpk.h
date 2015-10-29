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

dword PK_UID = 0;
dword PK_SIZE = 0;
dword PK_MAXSIZE = 0;

void pk_begin()
{
	gFileInfo.Clear();
	gFilenames.Clear();

	PK_UID = 0;
	PK_SIZE = 0;
	PK_MAXSIZE = 0;
}

void pk_convert(const FixedString256 & path, const FixedString256 & floderName, const FixedString256 & prefixName)
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

			pk_convert(fullname, floderName, prefixName + filename + "/");
		}
		else
		{
			FixedString256 orgFileName = prefixName + filename;
			FixedString8 mapedFilename = MPK_Filename(PK_UID);

			FixedString256 iFilename = path + "/" + filename;
			FixedString256 oFileName = gOutputPath + "/" + floderName + "/" + mapedFilename.c_str();

			orgFileName.Replace('\\', '/');
			orgFileName.ToLower();

			Hash2 hash(orgFileName.c_str());
			MPKFileInfo fi;
			fi.pkid = PK_UID;
			fi.flag = MPK_FLAG_COMPRESSED;
			fi.offset = PK_SIZE;
			fi.size = 0;
			fi.unc_size = 0;

			if (gUncompressed.Find(FileHelper::GetExternName(orgFileName.c_str()).c_str()) != -1)
			{
				fi.flag &= ~MPK_FLAG_COMPRESSED;
			}

			if (MPK_Append(fi, iFilename.c_str(), oFileName.c_str(), (const byte *)&hash) > 0)
			{
				std::cout << "Convert " << iFilename.c_str() << " OK..."<< std::endl;

				gFileInfo.PushBack(fi);
				gFilenames.PushBack(orgFileName);

				PK_SIZE += fi.size;
				if (PK_SIZE >= PK_MAXSIZE)
				{
					PK_UID += 1;
					PK_SIZE = 0;
				}

				if (PK_UID == 0xFFFF)
				{
					std::cout << "Error: File count must be <= 65536 Failed..."<< std::endl;
					return ;
				}
			}
			else
			{
				std::cout << "Convert " << iFilename.c_str() << " Failed..."<< std::endl;
			}
		}
	}
}

void pk_save(const FixedString256 & filename)
{
	if (gFileInfo.Size() == 0)
		return ;

	FixedString256 path = gOutputPath + "/" + filename + "/" + filename + ".MPK";
	FileHelper::MakeDir(FileHelper::GetFileDir(path.c_str()));

	FILE * fp = fopen(path.c_str(), "wb");
	if (!fp)
		return ;

	OSerializerF OS(fp, true);

	int FileMagic = MPK_FILE_MAGIC;
	OS << FileMagic;

	int ckId = MPK_FILE_INFO;
	OS << ckId;

	OSerializerM2 OSM;
	OSM << gFileInfo.Size();
	for (int i = 0; i < gFileInfo.Size(); ++i)
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
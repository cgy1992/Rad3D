#include "MPKArchive.h"

namespace Rad {

	ImplementRTTI(MPKArchive, Archive);

	MPKArchive::MPKArchive(const String & filename, MPK_READ_FILE fnReadFile)
		: Archive(filename)
	{
		mVersion = 1;

		mFnReadFile = fnReadFile;
		if (mFnReadFile == NULL)
			mFnReadFile = MPK_ReadFile;

		mFileDir = FileHelper::GetFileDir(filename);
		if (mFileDir != "")
			mFileDir += "/";

		DataStreamPtr stream = mFnReadFile(filename);
		if (stream != NULL)
			_load(stream);
	}

	MPKArchive::~MPKArchive()
	{
	}

	DataStreamPtr MPKArchive::Open(const String & source)
	{
		String filename_ = source;
		filename_.Replace('\\', '/');
		filename_.ToLower();

		int i = mFileMap.Find(Hash2(filename_.c_str()));
		if (i != -1)
		{
			const MPKFileInfo * fi = &mFileMap[i].value;
			const byte * pwd = (const byte *)&mFileMap[i].key;

			DataStreamPtr stream = NULL;
			if (mVersion < 2)
				stream = mFnReadFile(mFileDir + MPK_Filename(fi->pkid).c_str());
			else
				stream = mFnReadFile(mName);

			if (stream != NULL)
			{
				int size = fi->size;
				byte * data = new byte[size + 1];

				stream->Seek(fi->offset);
				stream->Read(data, size);

				MPK_Encrypt(data, size, pwd);

				if (fi->flag & MPK_FLAG_COMPRESSED)
				{
					byte * uncomp = new byte[fi->unc_size + 1]; 

					int ret = MPK_Decompress(uncomp, fi->unc_size, data, fi->size);
					if (ret == -1)
					{
						d_assert(0);

						delete[] uncomp;
						delete[] data;

						return NULL;
					}

					d_assert (ret == fi->unc_size);

					delete[] data;
					data = uncomp;
					size = fi->unc_size;
				}

				data[size] = 0;

				return new MemoryStream(data, size, true);
			}
		}

		return NULL;
	}

	bool MPKArchive::Exist(const String & source)
	{
		String filename_ = source;
		filename_.Replace('\\', '/');
		filename_.ToLower();

		int i = mFileMap.Find(Hash2(source.c_str()));

		return i != -1;
	}

	void MPKArchive::_load(DataStreamPtr stream)
	{
		int Magic;
		stream->Read(&Magic, sizeof(int));
		if (Magic != MPK_FILE_MAGIC &&
			Magic != MPK2_FILE_MAGIC)
			return ;

		mVersion = Magic - MPK_FILE_MAGIC;

		int ckId = 0, ckLen = 0;
		while (stream->Read(&ckId, sizeof(int)) && ckId != 0)
		{
			stream->Read(&ckLen, sizeof(int));

			if (ckId == MPK_FILE_INFO)
			{
				int count = 0;

				stream->Read(&count, sizeof(int));
				if (count > 0)
					mFileMap.Alloc(count);

				for (int i = 0; i < count; ++i)
				{
					Hash2 hash;
					MPKFileInfo info;

					stream->Read(&hash, sizeof(hash));
					stream->Read(&info, sizeof(info));

					mFileMap.Insert(hash, info);
				}
			}
			else
			{
				stream->Skip(ckLen);
			}
		}
	}

}
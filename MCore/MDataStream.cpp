#include "MDataStream.h"
#include "MFile.h"

namespace Rad {

	int DataStream::ReadString(char * str, int buffSize)
	{
		const int TEMP_SIZE = 128;
		char tmpBuf[TEMP_SIZE] = { 0 };
		int count = 0;
		bool flag = false;

		buffSize -= 1;
		while (!flag && count < buffSize && !eof())
		{
			int read_count = Read(tmpBuf, TEMP_SIZE);

			int i = 0;
			while (i < read_count && count < buffSize)
			{
				if (tmpBuf[i] == 0)
				{
					flag = true;
					i += 1;
					break;
				}

				str[count++] = tmpBuf[i++];
			}

			if (i < read_count)
			{
				Skip(-(read_count - i));
			}
		}

		str[count] = 0;

		return count;
	}

	//
	MemoryStream::MemoryStream(byte * data, int size, bool managed)
		: mSize(size)
		, mCursor(0)
		, mManaged(managed)
	{
		d_assert (data != NULL && size > 0);
		mData = data;
	}

	MemoryStream::MemoryStream(int size)
	: mSize(size)
	, mCursor(0)
	, mManaged(true)
	{
		mData = new byte[size + 1];
		mData[mSize] = 0;
	}

	MemoryStream::~MemoryStream()
	{
		Close();
	}

	DataStream * MemoryStream::Clone()
	{
		MemoryStream * p = new MemoryStream(mSize);

		memcpy(p->mData, mData, mSize);

		return p;
	}

	bool MemoryStream::IsOpen()
	{
		return true;
	}

	void MemoryStream::Close()
	{
		if (mManaged)
			safe_delete_array(mData);

		mData = NULL;
		mSize = 0;
		mCursor = 0;
	}

	void * MemoryStream::GetData()
	{
		return mData;
	}

	bool MemoryStream::eof() const
	{
		return mSize - mCursor <= 0;
	}

	int MemoryStream::Read(void * data, int size)
	{
		d_assert (size > 0);

		if (!eof())
		{
			if (size > mSize - mCursor)
				size = mSize - mCursor; 

			memcpy(data, &mData[mCursor], size);

			mCursor += size;

			return size;
		}

		return 0;
	}

	void MemoryStream::Skip(int sk)
	{
		if (sk < 0 && int(-sk) > mCursor)
		{
			mCursor = 0;
		}
		else
		{
			mCursor += sk;
		}
	}

	void MemoryStream::Seek(int pos)
	{
		mCursor = pos;
	}

	int MemoryStream::Tell()
	{
		return mCursor;
	}

	int MemoryStream::Size() const
	{
		return mSize;
	}

	//
	FileStream::FileStream(const String & file)
		: DataStream(file)
	{
		mData = NULL;
		mFileHandle = fopen(file.c_str(), "rb");
	}

	FileStream::~FileStream()
	{
		Close();
	}

	DataStream * FileStream::Clone()
	{
		return new FileStream(mSource);
	}

	bool FileStream::IsOpen()
	{
		return mFileHandle != NULL;
	}

	void FileStream::Close()
	{
		if (mFileHandle)
		{
			fclose(mFileHandle);
			mFileHandle = NULL;
		}

		safe_delete_array(mData);
	}

	int FileStream::Size() const
	{
		if (mFileHandle)
			return ftell_size(mFileHandle);

		return 0;
	}

	bool FileStream::eof() const
	{
		return mFileHandle == NULL || feof(mFileHandle) != 0;
	}

	int FileStream::Read(void * data, int size)
	{
		if (mFileHandle)
			return fread(data, 1, size, mFileHandle);

		return 0;
	}

	void FileStream::Skip(int sk)
	{
		if (mFileHandle)
			fseek(mFileHandle, sk, SEEK_CUR);
	}

	void FileStream::Seek(int pos)
	{
		if (mFileHandle)
			fseek(mFileHandle, pos, SEEK_SET);
	}

	int FileStream::Tell()
	{
		if (mFileHandle)
			return ftell(mFileHandle);

		return 0;
	}

	void * FileStream::GetData()
	{
		if (!mData)
		{
			int size = Size();
			mData = new char[size + 1];
			Read(mData, size);
			mData[size] = 0;
		}

		return mData;
	}

}

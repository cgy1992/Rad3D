#include "MISerializer.h"

namespace Rad {

	ISerializerD::ISerializerD(DataStreamPtr stream)
	{
		mStream = stream;
	}

	ISerializerD::~ISerializerD()
	{
	}

	void ISerializerD::Skip(int offset)
	{
		mStream->Skip(offset);
	}

	void ISerializerD::Seek(int pos)
	{
		mStream->Seek(pos);
	}

	int ISerializerD::Read(void * data, int size)
	{
		return mStream->Read(data, size);
	}

	int ISerializerD::ReadString(char * data, int maxSize)
	{
		return mStream->ReadString(data, maxSize) > 0;
	}

	//
	ISerializerF::ISerializerF(FILE * stream, bool managed)
		: mFile(stream)
		, mManaged(managed)
	{
	}

	ISerializerF::ISerializerF(const char * filename)
	{
		mFile = fopen(filename, "rb");
		mManaged = true;
	}

	ISerializerF::~ISerializerF()
	{
		if (mManaged && mFile)
			fclose(mFile);
	}

	void ISerializerF::Skip(int offset)
	{
		fseek(mFile, offset, SEEK_CUR);
	}

	void ISerializerF::Seek(int pos)
	{
		fseek(mFile, pos, SEEK_SET);
	}

	int ISerializerF::Read(void * data, int size)
	{
		return fread(data, 1, size, mFile);
	}

	int ISerializerF::ReadString(char * data, int maxSize)
	{
		return fread_string(data, maxSize, mFile);
	}

	//
	ISerializerM::ISerializerM(byte * data, int size, bool managed)
		: mData(data)
		, mSize(size)
		, mCursor(0)
		, mManaged(managed)
	{
	}

	ISerializerM::~ISerializerM()
	{
		if (mManaged)
		{
			delete[] mData;
		}
	}

	void ISerializerM::Skip(int offset)
	{
		mCursor += offset;

		d_assert (mCursor >= 0 && mCursor <= mSize);
	}

	void ISerializerM::Seek(int pos)
	{
		mCursor += pos;

		d_assert (mCursor >= 0 && mCursor <= mSize);
	}

	int ISerializerM::Read(void * data, int size)
	{
		int nreads = 0;

		if (mCursor + size <= mSize)
		{
			memcpy(data, mData + mCursor, size);

			nreads = size;
			mCursor += nreads;
		}

		return nreads;
	}

	int ISerializerM::ReadString(char * data, int maxSize)
	{
		int i = 0;
		char * src = (char *)mData;

		while (mCursor + i < mSize)
		{
			if (src[mCursor + i++] == 0)
			{
				break;
			}
		}

		d_assert (i < maxSize);

		memcpy(data, &src[mCursor], i - mCursor);
		data[i] = 0;

		mCursor += i;

		return i;
	}
}
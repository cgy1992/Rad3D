#include "MOSerializer.h"

namespace Rad {

	OSerializerF::OSerializerF(FILE * stream, bool managed)
		: mFile(stream)
		, mManaged(managed)
	{
	}

	OSerializerF::OSerializerF(const char * filename)
	{
		mFile = fopen(filename, "wb");
		mManaged = true;
	}

	OSerializerF::~OSerializerF()
	{
		if (mFile && mManaged)
			fclose(mFile);
	}

	int OSerializerF::Write(const void * data, int size)
	{
		if (mFile != NULL)
			return fwrite(data, 1, size, mFile);

		return 0;
	}


	//
	OSerializerM::OSerializerM(byte * data, int size, bool managed)
		: mData(data)
		, mSize(size)
		, mLength(0)
		, mManaged(managed)
	{
	}

	OSerializerM::OSerializerM(int size)
		: mSize(size)
		, mLength(0)
		, mManaged(true)
	{
		d_assert (size > 0);

		mData = new byte[size];
	}

	OSerializerM::~OSerializerM()
	{
		if (mManaged)
		{
			delete[] mData;
		}
	}

	int OSerializerM::Write(const void * data, int size)
	{
		d_assert (mLength + size <= mSize);

		memcpy(mData + mLength, data, size);

		mLength += size;

		return size;
	}

	//
	OSerializerM2::OSerializerM2()
	{
	}

	OSerializerM2::~OSerializerM2()
	{
	}

	int OSerializerM2::Write(const void * data, int size)
	{
		int length = mData.Size();
		mData.Resize(length + size);

		memcpy(&mData[length], data, size);

		return size;
	}


	//
	OSerializerTester::OSerializerTester()
		: mLength(0)
	{
	}

	OSerializerTester::~OSerializerTester()
	{
	}

	int OSerializerTester::Write(const void * data, int size)
	{
		mLength += size;

		return size;
	}

}
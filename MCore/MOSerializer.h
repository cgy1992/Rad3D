/*
*	OSerializer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSerializer.h"

namespace Rad {

	class M_ENTRY OSerializer : public Serializer
	{
	public:
		OSerializer() : Serializer(1) {}
		virtual	~OSerializer() {}

		virtual int 
			Write(const void * data, int size) = 0;
		int
			WriteString(const char * str) { return Write(str, strlen(str) + 1); }

		template <class T>
		int
			WriteT(const T & data) { return Write(&data, sizeof(T)); }

		template <class T> OSerializer &
			operator <<(const T & data) { WriteT(data); return *this; }
	};

	template<> 
	inline int OSerializer::WriteT<String>(const String & data)
	{
		return WriteString(data.c_str());
	}

	//
	class M_ENTRY OSerializerF : public OSerializer
	{
	public:
		OSerializerF(FILE * stream, bool managed);
		OSerializerF(const char * filename);
		virtual ~OSerializerF();

		FILE *
			GetFile() { return mFile; }

		virtual int 
			Write(const void * data, int size);

	protected:
		FILE * mFile;
		bool mManaged;
	};

	//
	class M_ENTRY OSerializerM : public OSerializer
	{
	public:
		OSerializerM(byte * data, int size, bool managed);
		OSerializerM(int size);
		virtual ~OSerializerM();

		virtual int 
			Write(const void * data, int size);

		void *
			Data() { return mData; }
		int
			Size() { return mLength; }
		void
			Clear() { mLength = 0; }

	protected:
		byte * mData;
		int mLength;
		int mSize;
		bool mManaged;
	};

	//
	class M_ENTRY OSerializerM2 : public OSerializer
	{
	public:
		OSerializerM2();
		virtual ~OSerializerM2();

		virtual int 
			Write(const void * data, int size);

		void *
			Data() { return &mData[0]; }
		int
			Size() { return mData.Size(); }
		void
			Clear() { mData.Clear(); }

	protected:
		Array<byte> mData;
	};

	//
	class M_ENTRY OSerializerTester : public OSerializer
	{
	public:
		OSerializerTester();
		virtual ~OSerializerTester();

		int
			Size() { return mLength; }
		void
			Clear() { mLength = 0; }

		virtual int 
			Write(const void * data, int size);

	protected:
		int mLength;
	};

	//
	template <int POOL_SIZE>
	class M_ENTRY OSerializerT : public OSerializer
	{
	public:
		OSerializerT()
		{
			mLength = 0;
		}

		virtual ~OSerializerT()
		{
		}

		virtual int Write(const void * data, int size)
		{
			d_assert (mLength + size < POOL_SIZE);

			memcpy(&mData[mLength], data, size);

			mLength += size;

			return size;
		}

		void *
			Data() { return &mData[0]; }
		int
			Size() { return mLength; }
		void
			Clear() { mLength = 0; }

	protected:
		Field<byte, POOL_SIZE> mData;
		int mLength;
	};
}
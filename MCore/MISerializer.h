/*
*	ISerializer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSerializer.h"

namespace Rad {

	class M_ENTRY ISerializer : public Serializer
	{
	public:
		ISerializer() : Serializer(0) {}
		virtual	~ISerializer() {}

		virtual void
			Skip(int offset) = 0;
		virtual void
			Seek(int pos) = 0;
		virtual int 
			Read(void * data, int size) = 0;
		virtual int
			ReadString(char * data, int maxSize) = 0;

		template<class T>
		int
			ReadT(T & data) { return Read(&data, sizeof(T)); }

		template <class T> ISerializer &
			operator >>(T & data) { ReadT(data); return *this; }

		bool
			Check(int magic);
	};

	template<> 
	inline int ISerializer::ReadT<String>(String & data)
	{
		char buff[1024];
		int count = ReadString(buff, 1024);
		data = buff;
		return count;
	}

	inline bool ISerializer::Check(int maigc)
	{
		int val = 0;

		Read(&val, 4);

		d_assert (val == maigc);

		return val == maigc;
	}

	//
	class M_ENTRY ISerializerD : public ISerializer
	{
	public:
		ISerializerD(DataStreamPtr stream);
		virtual	~ISerializerD();

		DataStreamPtr
			GetStream() { return mStream; }

		virtual void
			Skip(int offset);
		virtual void
			Seek(int pos);
		virtual int 
			Read(void * data, int size);
		virtual int
			ReadString(char * data, int maxSize);

	protected:
		DataStreamPtr mStream;
	};

	//
	class M_ENTRY ISerializerF : public ISerializer
	{
	public:
		ISerializerF(FILE * stream, bool managed);
		ISerializerF(const char * filename);
		virtual	~ISerializerF();

		FILE *
			GetFile() { return mFile; }

		virtual void
			Skip(int offset);
		virtual void
			Seek(int pos);
		virtual int 
			Read(void * data, int size);
		virtual int
			ReadString(char * data, int maxSize);

	protected:
		FILE * mFile;
		bool mManaged;
	};

	//
	class M_ENTRY ISerializerM : public ISerializer
	{
	public:
		ISerializerM(byte * data, int size, bool managed);
		virtual ~ISerializerM();

		virtual void
			Skip(int offset);
		virtual void
			Seek(int pos);
		virtual int 
			Read(void * data, int size);
		virtual int
			ReadString(char * data, int maxSize);

	protected:
		byte * mData;
		int mSize;
		int mCursor;
		bool mManaged;
	};
	
}
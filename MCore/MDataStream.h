/*
*	DataStream
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRef.h"
#include "MString.h"
#include "MFixedString.h"
#include "MSmartPtr.h"
#include "MFile.h"

namespace Rad {

	class M_ENTRY DataStream
	{
		DECLARE_REF_MT();

	public:
		DataStream() {}
		DataStream(const String & source) : mSource(source) {}
		virtual ~DataStream() {}

		const String &
			GetSource() { return mSource; }

		virtual void
			Release() { delete this; }
		
		virtual DataStream *
			Clone() = 0;

		virtual void *
			GetData() = 0;
		
		virtual bool
			IsOpen() = 0;
		virtual void
			Close() = 0;
		virtual int 
			Size() const = 0;
		virtual bool
			eof() const = 0;
		
		virtual int
			Read(void * data, int size) = 0;
		int
			ReadString(char * str, int buffSize);

		virtual void 
			Skip(int sk) = 0;
		virtual void 
			Seek(int pos) = 0;
		virtual int
			Tell() = 0;

		template<class T>
		int
			ReadT(T & data) { return Read(&data, sizeof(T)); }

		template <class T> DataStream &
			operator >>(T & data) { ReadT(data); return *this; }

	protected:
		String mSource;
	};

	template<> 
	inline int DataStream::ReadT<String>(String & data)
	{
		char buff[1024];
		int count = ReadString(buff, 1024);
		data = buff;
		return count;
	}
	
	typedef SmartPtr<DataStream> DataStreamPtr;

	//
	class M_ENTRY MemoryStream : public DataStream
	{
	public:
		MemoryStream(byte * data, int size, bool managed);
		MemoryStream(int size);
		virtual ~MemoryStream();

		virtual DataStream *
			Clone();

		virtual void *
			GetData();
		
		virtual bool
			IsOpen();
		virtual void
			Close();
		virtual int
			Size() const;
		virtual bool
			eof() const;
		
		virtual int 
			Read(void * data, int size);

		virtual void
			Skip(int sk);
		virtual void
			Seek(int pos);
		virtual int
			Tell();

	protected:
		int mSize;
		byte * mData;
		int mCursor;
		bool mManaged;
	};

	typedef SmartPtr<MemoryStream> MemoryStreamPtr;

	//
	class M_ENTRY FileStream : public DataStream
	{
	public:
		FileStream(const String & file);
		virtual ~FileStream();

		virtual DataStream *
			Clone();
		virtual void *
			GetData();

		virtual bool
			IsOpen();
		virtual void
			Close();
		virtual int
			Size() const;
		virtual bool
			eof() const;

		virtual int
			Read(void * data, int size);

		virtual void
			Skip(int sk);
		virtual void
			Seek(int pos);
		virtual int
			Tell();

	protected:
		FILE * mFileHandle;
		char * mData;
	};

	typedef SmartPtr<FileStream> FileStreamPtr;

}

/*
*	Archive
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRTTI.h"
#include "MFixedString.h"
#include "MDataStream.h"

namespace Rad {

	class M_ENTRY Archive
	{
		DECLARE_RTTI_ROOT(Archive);

	public:
		Archive(const String & name);
		virtual ~Archive();

		const String & 
			GetName() const;

		virtual DataStreamPtr 
			Open(const String & filename) = 0;
		virtual bool
			Exist(const String & filename) = 0;

	protected:
		String mName;
	};

	class M_ENTRY FilePathArchive : public Archive
	{
		DECLARE_RTTI();

	public:
		FilePathArchive(const String & name);
		virtual ~FilePathArchive();

		virtual DataStreamPtr 
			Open(const String & filename);
		virtual bool
			Exist(const String & filename);
	};
}
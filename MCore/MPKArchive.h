/*
*	MPKArchive
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MPKCommon.h"
#include "MArchive.h"
#include "MMap.h"
#include "MHash.h"

namespace Rad {

	class M_ENTRY MPKArchive : public Archive
	{
		DECLARE_RTTI();

	public:
		MPKArchive(const String & filename, MPK_READ_FILE fnReadFile);
		virtual ~MPKArchive();

		virtual DataStreamPtr
			Open(const String & source);

		virtual bool
			Exist(const String & source);

	protected:
		void 
			_load(DataStreamPtr stream);

	protected:
		int mVersion;
		String mFileDir;
		MPK_READ_FILE mFnReadFile;
		Map<Hash2, MPKFileInfo> mFileMap;
	};

}
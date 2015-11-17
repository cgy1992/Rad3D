/*
*	MPKCommon
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFixedString.h"
#include "MDataStream.h"

namespace Rad {

	struct M_ENTRY MPKFileInfo
	{
		word pkid;
		word flag;
		int offset;
		int size;
		int unc_size;
	};

#define MPK_FILE_MAGIC MAKE_DWORD('M', 'P', 'K', '0')
#define MPK2_FILE_MAGIC MAKE_DWORD('M', 'P', 'K', '2')
#define MPK_FILE_INFO 0x000F

#define MPK_FLAG_COMPRESSED 0x1000


	typedef DataStreamPtr (*MPK_READ_FILE)(const String & filename);

	M_ENTRY DataStreamPtr 
		MPK_ReadFile(const String & filename);
	M_ENTRY FixedString8 
		MPK_Filename(int id);
	M_ENTRY void
		MPK_Encrypt(byte * data, int len, const byte * pwd);
	M_ENTRY int
		MPK_Compress(byte * data, int len);
	M_ENTRY int
		MPK_Decompress(byte * uncomp, int max_len, const byte * comp, int c_len);
	M_ENTRY int
		MPK_Append(MPKFileInfo & fi, const String & existFilename, const String & newFilename, const byte * pwd);
}
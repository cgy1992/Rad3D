/*
*	ETC Image
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MImage.h"
#include "MDataStream.h"

namespace Rad {

	M_ENTRY bool 
		ETC_Test(DataStreamPtr stream);
	M_ENTRY bool
		ETC_Load(Image & image, DataStreamPtr stream);
	M_ENTRY byte *
		ETC_Decode(const byte * blocks, int width, int height, ePixelFormat format);
	M_ENTRY void
		ETC_DecodeBlock(byte pixel[16*4], const byte * iblock, ePixelFormat format);
}
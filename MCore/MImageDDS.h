/*
*	DDS Image
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MImage.h"
#include "MTypes.h"
#include "MDataStream.h"

namespace Rad {

	M_ENTRY bool 
		DDS_Test(DataStreamPtr stream);
	M_ENTRY bool
		DDS_Load(Image & image, DataStreamPtr stream);
	M_ENTRY byte *
		DDS_Decode(const byte * blocks, int width, int height, ePixelFormat format);
	M_ENTRY void
		DDS_DecodeBlock(byte pixel[16*4], const byte * iblock, ePixelFormat format);
}
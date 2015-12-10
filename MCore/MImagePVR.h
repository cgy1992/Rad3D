/*
*	PVR Image
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
		PVR_Test(DataStreamPtr stream);
	M_ENTRY bool
		PVR_Load(Image & image, DataStreamPtr stream);
	M_ENTRY byte *
		PVR_Decode(const byte * blocks, int width, int height, ePixelFormat format);
	M_ENTRY void
		PVR_DecodeBlock(byte pixels[16*4], const byte * iblock, ePixelFormat format);
}
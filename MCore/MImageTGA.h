/*
*	TGA Image
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
		TGA_Test(DataStreamPtr stream);
	M_ENTRY bool
		TGA_Load(Image & image, DataStreamPtr stream);
	M_ENTRY bool
		TGA_Save(OSerializer & OS, const byte * pixels, int width, int height, ePixelFormat format);
}
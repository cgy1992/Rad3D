/*
*	BMP Image
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
		BMP_Test(DataStreamPtr stream);
	M_ENTRY bool
		BMP_Load(Image & image, DataStreamPtr stream);
	M_ENTRY bool
		BMP_Save(OSerializer & OS, const byte * pixels, int width, int height, ePixelFormat format);
}
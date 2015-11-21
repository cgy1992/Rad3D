/*
*	Image
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"
#include "MSingleton.h"

namespace Rad {

	struct M_ENTRY Image
	{
		byte * pixels;
		ePixelFormat format;

		int width;
		int height;
		int depth;
		byte mipmaps;
		byte cubmaps;

		Image()
		{
			pixels = NULL;
			format = ePixelFormat::UNKNOWN;

			width = 0;
			height = 0;
			depth = 1;
			mipmaps = 0;
			cubmaps = 1;
		}
	};

	typedef bool (*FN_ImageTest)(DataStreamPtr stream);
	typedef bool (*FN_ImageLoad)(Image & image, DataStreamPtr stream);

	M_ENTRY void
		IMG_Reg(FN_ImageTest fn_test, FN_ImageLoad fn_load);
	M_ENTRY bool
		IMG_Load(Image & image, DataStreamPtr stream);
	M_ENTRY void
		IMG_Clear();
}

#include "MImageBMP.h"
#include "MImageTGA.h"
#include "MImageDDS.h"
#include "MImagePNG.h"
#include "MImageETC.h"
#include "MImagePVR.h"
#include "MImageJPG.h"

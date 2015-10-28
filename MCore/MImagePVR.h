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
}
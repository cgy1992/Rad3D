/*
*	Video OGG
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/

#pragma once

#include "MVideo.h"

namespace Rad {

	bool 
		OGV_Test(DataStreamPtr stream);
	IVideo *
		OGV_Load(DataStreamPtr stream);

}
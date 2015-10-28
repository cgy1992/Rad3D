/*
*	Audio OGG
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAudio.h"

namespace Rad {

	bool 
		OGG_Test(DataStreamPtr stream);
	IAudio * 
		OGG_Load(DataStreamPtr stream);

}
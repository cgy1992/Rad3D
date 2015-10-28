/*
*	Audio WAV
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAudio.h"

namespace Rad {

	bool 
		WAV_Test(DataStreamPtr stream);
	IAudio * 
		WAV_Load(DataStreamPtr stream);

}
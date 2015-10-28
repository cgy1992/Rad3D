/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MShaderFX.h"

namespace Rad {

	class NullShaderPass : public ShaderPass
	{
	public:
		NullShaderPass() {}
		virtual ~NullShaderPass() {}

		virtual ShaderPass * 
			Clone() { return new NullShaderPass; }
	};

}

/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MShaderFXManager.h"
#include "NullShaderPass.h"

namespace Rad {

	class NullShaderFXManager : public ShaderFXManager
	{
	public:
		NullShaderFXManager();
		virtual ~NullShaderFXManager();

		virtual void
			_loadImp(ShaderFX * fx, DataStreamPtr stream);
	};

}

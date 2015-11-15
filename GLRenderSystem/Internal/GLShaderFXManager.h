/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"
#include "GLShaderPass.h"

namespace Rad {

	class GLShaderFXManager : public ShaderFXManager
	{
	public:
		GLShaderFXManager();
		virtual ~GLShaderFXManager();
		
		void 
			_loadImp(ShaderFX * fx, DataStreamPtr stream);

		void 
			OnLostDevice();
		void 
			OnResetDevice();
	};

}

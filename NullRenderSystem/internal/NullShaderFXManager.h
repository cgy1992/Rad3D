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

		virtual ShaderFX * 
			Load(const String & name, const String & source, const String & macros);
		virtual void 
			Remove(ShaderFX * fx) {}
		virtual void
			Reload(ShaderFX * fx) {}
		virtual void 
			ReloadAll() {}


	protected:
		ShaderFX * mNullShaderFX;
	};

}

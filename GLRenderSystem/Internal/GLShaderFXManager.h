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
		
		virtual ShaderFX * 
			Load(const String & name, const String & source, const String & macros = "");
		virtual ShaderFX * 
			Load(const String & name, DataStreamPtr stream, const String & macros = "");
		virtual void 
			Remove(ShaderFX * fx);
		virtual void
			Reload(ShaderFX * fx);
		virtual void 
			ReloadAll();

		void 
			OnLostDevice();
		void 
			OnResetDevice();

	protected:
		ShaderFX * 
			_find(Hash2 hash, const String & name);

	protected:
		Map<Hash2, ShaderFX *> mFXMap;
	};

}

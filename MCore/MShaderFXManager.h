/*
*	ShaderFX Manager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSingleton.h"
#include "MShaderFX.h"
#include "MMap.h"

namespace Rad {

	class M_ENTRY ShaderFXManager : public Singleton<ShaderFXManager>
	{
	public:
		tEvent0 E_ReloadAll;
	
	public:
		ShaderFXManager();
		virtual ~ShaderFXManager();

		virtual ShaderFX * 
			Load(const String & name, const String & source, const String & macros = "") = 0;
		virtual ShaderFX * 
			Load(const String & name, DataStreamPtr stream, const String & macros = "") = 0;
		virtual void 
			Remove(ShaderFX * fx) = 0;
		virtual void
			Reload(ShaderFX * fx) = 0;
		virtual void 
			ReloadAll() = 0;

		void
			LoadLibrary(const String & libname);

		void 
			AddGlobalMacro(const String & macro);
		void 
			RemoveGlobalMacro(const String & macro);
		const char * 
			GetGlobalMacroString();

	protected:
		String mGlobalMacroString;
	};

}

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

		virtual void
			_loadImp(ShaderFX * fx, DataStreamPtr stream) = 0;

		ShaderFX * 
			Load(const String & name, const String & source, const String & macros = "");
		ShaderFX * 
			Load(const String & name, DataStreamPtr stream, const String & macros = "");
		void 
			Remove(ShaderFX * fx);
		void
			Reload(ShaderFX * fx);
		void 
			ReloadAll();

		void
			LoadLibrary(const String & libname);

		void 
			AddGlobalMacro(const String & macro);
		void 
			RemoveGlobalMacro(const String & macro);
		const char * 
			GetGlobalMacroString();

	protected:
		ShaderFX * 
			_find(Hash2 hash, const String & name);

	protected:
		String mGlobalMacroString;
		Map<Hash2, ShaderFX *> mFXMap;
	};

}

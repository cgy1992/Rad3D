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

namespace Rad {

	class M_ENTRY ShaderFXManager : public Singleton<ShaderFXManager>
	{
	public:
		ShaderFXManager();
		virtual ~ShaderFXManager();

		virtual ShaderFX * 
			Load(const String & name, const String & source, const String & macros = "") = 0;
		virtual void 
			Remove(ShaderFX * fx) = 0;
		virtual void
			Reload(ShaderFX * fx) = 0;
		virtual void 
			ReloadAll() = 0;

		void
			LoadLibrary(const String & libname);

		void 
			AddGlobalMacro(const FixedString32 & macro);
		void 
			RemoveGlobalMacro(const FixedString32 & macro);
		const char * 
			GetGlobalMacroString();

	protected:
		FixedString256 mGlobalMacroString;
	};

}

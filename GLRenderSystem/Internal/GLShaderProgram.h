/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"

namespace Rad {

	class ShaderFX;

	class GLShaderProgram
	{
		DECLARE_REF();
		DECLARE_POOL_ALLOC();

	public:
		GLShaderProgram(const char * vscode, const char * pscode, ShaderFX * fx);
		virtual ~GLShaderProgram();

		void
			Release() { delete this; }

		bool 
			Build();

		void 
			OnLostDevice();
		void 
			OnResetDevice();

		GLuint 
			GetProgram() { return mProgram; }

	protected:
		String mVertexShader;
		String mPixelShader;
		GLuint mProgram;

		ShaderFX * mFX;
	};

	typedef SmartPtr<GLShaderProgram> GLShaderProgramPtr;

}

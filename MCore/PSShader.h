/*
*	PSShader
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"

namespace Rad {

	class M_ENTRY PS_Shader : public IObject, public RenderCallBack
	{
		DECLARE_REF();
		DECLARE_RTTI();

	public:
		PS_Shader();
		virtual ~PS_Shader();

		void
			Release() { delete this; }

		ShaderFX *
			GetShaderFX() { return mFX; }

		virtual void
			Update(float frameTime) {}

	protected:
		ShaderFX * mFX;
	};

	typedef SmartPtr<PS_Shader> PS_ShaderPtr;
}
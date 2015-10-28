/*
*	Sky
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"
#include "MShaderFX.h"

namespace Rad {

	class M_ENTRY Sky
	{
		DECLARE_ALLOC();

	public:
		Sky();
		~Sky();

		void Render();

	protected:
		RenderOp mRenderOp;
		ShaderFX * mTech;
		TexturePtr mTexture;
	};
}

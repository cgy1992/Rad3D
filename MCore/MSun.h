/*
*	Sun
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"
#include "MShaderFX.h"

namespace Rad {

	class M_ENTRY Sun
	{
		DECLARE_ALLOC();

	public:
		Sun();
		~Sun();

		void Render();

	protected:
		RenderOp mRenderOp;
		ShaderFX * mTech;
		TexturePtr mTexture;
	};

}

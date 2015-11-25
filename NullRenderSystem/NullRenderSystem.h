/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderSystem.h"

#ifdef M_PLATFORM_WIN32

#ifdef NL_EXPORT
#define NL_ENTRY __declspec(dllexport)
#else
#define NL_ENTRY __declspec(dllimport)
#endif

#else

#define NL_ENTRY

#endif


namespace Rad {

	class NullHWBufferManager;
	class NullShaderFXManager;

	class NL_ENTRY NullRenderSystem : public RenderSystem
	{
	public:
		NullRenderSystem();
		virtual ~NullRenderSystem();

		virtual void
			OnResize(int w, int h);

		virtual void 
			Begin();
		virtual void 
			End();
		virtual void
			Finish();

		virtual void 
			SetViewport(const Viewport & vp);

		virtual void 
			Clear(eClearMode mode, const Float4 & color, float depth, int stencil);

		virtual void
			PrepareRendering();

		virtual void 
			ReadPixels(void * data, int x, int y, int w, int h);
		virtual void
			StretchRect(RenderTarget * rtDest, RectI * rcDest, RenderTarget * rtSrc, RectI * rcSrc, eTexFilter filter);

		virtual void 
			Render(RenderOp * rop);
		virtual void 
			RenderEx(VertexDeclaration * decl, const void * vertexData, const void * indexData, ePrimType primType, int primCount);
		virtual void 
			RenderScreenQuad(ShaderFX * fx);

	protected:
		NullHWBufferManager * mHWBufferManager;
		NullShaderFXManager * mShaderFXManager;
	};
}

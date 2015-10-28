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
			SetViewport(const Viewport & vp);

		virtual void 
			Clear(eClearMode mode, const Float4 & color, float depth, int stencil);

		virtual void 
			SetProjTM(const Mat4 & projTM);

		virtual void 
			SetRenderTarget(RenderTarget * p);
		virtual void
			SetDepthBuffer(DepthBuffer * p);

		virtual void 
			ReadPixelData(void * data, int x, int y, int w, int h);

		virtual void 
			Render(RenderOp * rop);

		virtual void 
			RenderEx(VertexDeclaration * decl, const void * vertexData, const void * indexData, ePrimType primType, int primCount);

	protected:
		NullHWBufferManager * mHWBufferManager;
		NullShaderFXManager * mShaderFXManager;
	};
}

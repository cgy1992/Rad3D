#include "NullRenderSystem.h"
#include "NullHWBufferManager.h"
#include "NullShaderFXManager.h"

namespace Rad {

	NullRenderSystem::NullRenderSystem()
	{
		mConfig.width = mConfig.height = 1;

		mHWBufferManager = new NullHWBufferManager;
		mShaderFXManager = new NullShaderFXManager;

		OnInit();
	}

	NullRenderSystem::~NullRenderSystem()
	{
		OnShutdown();

		delete mShaderFXManager;
		delete mHWBufferManager;
	}

	
	void NullRenderSystem::OnResize(int w, int h)
	{
		E_Resize(w, h);
	}

	void NullRenderSystem::Begin()
	{
	}

	void NullRenderSystem::End()
	{
	}

	void NullRenderSystem::Finish()
	{
	}

	void NullRenderSystem::SetViewport(const Viewport & viewport)
	{
	}

	void NullRenderSystem::Clear(eClearMode mode, const Float4 & color, float depth, int stencil)
	{
	}

	void NullRenderSystem::PrepareRendering()
	{
	}

	void NullRenderSystem::ReadPixels(void * data, int x, int y, int w, int h)
	{
	}

	void NullRenderSystem::StretchRect(RenderTarget * rtDest, RectI * rcDest, RenderTarget * rtSrc, RectI * rcSrc, eTexFilter filter)
	{
	}

	void NullRenderSystem::Render(RenderOp * rop)
	{
	}

	void NullRenderSystem::RenderEx(VertexDeclaration * vdecl, const void * vertexData, const void * indexData, ePrimType primType, int primCount)
	{
	}

	void NullRenderSystem::RenderScreenQuad(ShaderFX * fx)
	{
	}

}
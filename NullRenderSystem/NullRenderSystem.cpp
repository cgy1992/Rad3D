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
	}

	void NullRenderSystem::Begin()
	{
	}

	void NullRenderSystem::End()
	{
	}

	void NullRenderSystem::SetViewport(const Viewport & viewport)
	{
	}

	void NullRenderSystem::Clear(eClearMode mode, const Float4 & color, float depth, int stencil)
	{
	}

	void NullRenderSystem::SetProjTM(const Mat4 & projTM)
	{
	}

	void NullRenderSystem::SetRenderTarget(RenderTarget * p)
	{
	}

	void NullRenderSystem::SetDepthBuffer(DepthBuffer * p)
	{
	}

	void NullRenderSystem::ReadPixelData(void * data, int x, int y, int w, int h)
	{
	}

	void NullRenderSystem::Render(RenderOp * rop)
	{
	}

	void NullRenderSystem::RenderEx(VertexDeclaration * vdecl, const void * vertexData, const void * indexData, ePrimType primType, int primCount)
	{
	}

}
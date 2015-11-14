#include "MRenderContext.h"
#include "MRenderSystem.h"

namespace Rad {

	RenderContext::RenderContext(int id, int order, const String & name)
		: mId(id)
		, mOrder(order)
		, mName(name)
		, mEnable(true)
		, mMatchId(false)
		, mClearMode(eClearMode::ALL)
		, mClearColor(0, 0, 0, 1)
		, mClearDepth(1)
		, mClearStencil(0)
		, mCamera(NULL)
	{
	}

	RenderContext::~RenderContext()
	{
	}

	void RenderContext::SetEnable(bool enable)
	{
		mEnable = enable;
	}

	void RenderContext::SetColorClear(eClearMode mode, const Float4 & color, float depth, int stencil)
	{
		mClearMode = mode;
		mClearColor = color;
		mClearDepth = depth;
		mClearStencil = stencil;
	}

	void RenderContext::SetRenderTarget(int i, RenderTargetPtr p)
	{
		mRenderTarget[i] = p;
	}

	RenderTargetPtr RenderContext::GetRenderTarget(int i)
	{
		return mRenderTarget[i];
	}

	void RenderContext::SetDepthBuffer(DepthBufferPtr p)
	{
		mDepthBuffer = p;
	}

	DepthBufferPtr RenderContext::GetDepthBuffer()
	{
		return mDepthBuffer;
	}

	void RenderContext::SetCamera(Camera * camera)
	{
		mCamera = camera;
	}

	Camera * RenderContext::GetCamera()
	{
		return mCamera;
	}

	void RenderContext::SetViewport(const Viewport & vp)
	{
		mViewport = vp;
	}

	Viewport RenderContext::GetViewport()
	{
		Viewport vp = mViewport;
		if (vp.w == 0 || vp.h == 0)
		{
			vp.x = vp.y = 0;

			if (mRenderTarget[0] != NULL)
			{
				vp.w = mRenderTarget[0]->GetWidth();
				vp.h = mRenderTarget[0]->GetHeight();
			}
			else
			{
				vp.w = RenderSystem::Instance()->GetConfig().width;
				vp.h = RenderSystem::Instance()->GetConfig().height;
			}
		}

		return vp;
	}

	void RenderContext::SetVisibleCuller(VisibleCullerPtr p)
	{
		mVisibleCuller = p;
	}

	VisibleCullerPtr RenderContext::GetVisibleCuller()
	{
		return mVisibleCuller;
	}

	void RenderContext::SetShaderProvider(ShaderProviderPtr p)
	{
		mShaderProvider = p;
	}

	ShaderProviderPtr RenderContext::GetShaderProvider()
	{
		return mShaderProvider;
	}

	void RenderContext::SetRenderPipeline(RenderPipelinePtr p)
	{
		mRenderPipeline = p;
	}

	RenderPipelinePtr RenderContext::GetRenderPipeline()
	{
		return mRenderPipeline;
	}

	void RenderContext::DoRender(int frameId)
	{
		if (mCamera != NULL)
		{
			Viewport vp = GetViewport();

			mCamera->SetAspect((float)vp.w / vp.h);
		}

		if (mVisibleCuller != NULL && mCamera != NULL)
			mVisibleCuller->DoCull(frameId, mCamera);

		 RenderSystem::Instance()->_clearState();

		// render scene
		RenderSystem::Instance()->SetRenderTarget(0, mRenderTarget[0].c_ptr());
		RenderSystem::Instance()->SetRenderTarget(1, mRenderTarget[1].c_ptr());
		RenderSystem::Instance()->SetRenderTarget(2, mRenderTarget[2].c_ptr());
		RenderSystem::Instance()->SetRenderTarget(3, mRenderTarget[3].c_ptr());
		RenderSystem::Instance()->SetDepthBuffer(mDepthBuffer.c_ptr());
		RenderSystem::Instance()->PrepareRendering();

		RenderSystem::Instance()->SetViewport(mViewport);

		if (mClearMode != eClearMode::NONE)
			RenderSystem::Instance()->Clear(mClearMode, mClearColor, mClearDepth, mClearStencil);

		if (mCamera != NULL)
			RenderSystem::Instance()->SetCamera(mCamera);

		E_RenderBegin(this);

		if (mRenderPipeline != NULL)
			mRenderPipeline->DoRender();

		E_RenderEnd(this);
	}

	void RenderContext::ReadPixels(void * pixels, int x, int y, int w, int h)
	{
		d_assert (x >= mViewport.x && y >= mViewport.y && w <= mViewport.w && h <= mViewport.h);
		d_assert (pixels != NULL);

		RenderSystem::Instance()->SetRenderTarget(NULL, mRenderTarget[0].c_ptr());
		RenderSystem::Instance()->PrepareRendering();
		RenderSystem::Instance()->ReadPixels(pixels, x, y, w, h);
	}

}
#include "MRenderContext.h"
#include "MRenderSystem.h"

namespace Rad {

	RenderContext::RenderContext(int id, const String & name)
		: mId(id)
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
		for (int i = 0; i < mProcesses.Size(); ++i)
		{
			delete mProcesses[i];
		}
	}

	int RenderContext::GetId()
	{
		return mId;
	}

	const String & RenderContext::GetName()
	{
		return mName;
	}

	void RenderContext::SetEnable(bool enable)
	{
		mEnable = enable;
	}

	bool RenderContext::IsEnable()
	{
		return mEnable;
	}

	void RenderContext::SetMatchId(bool matched)
	{
		mMatchId = matched;
	}

	bool RenderContext::IsMatchId()
	{
		return mMatchId;
	}

	void RenderContext::SetColorClear(eClearMode mode, const Float4 & color, float depth, int stencil)
	{
		mClearMode = mode;
		mClearColor = color;
		mClearDepth = depth;
		mClearStencil = stencil;
	}

	void RenderContext::SetRenderTarget(RenderTargetPtr p)
	{
		mRenderTarget = p;
	}

	RenderTargetPtr RenderContext::GetRenderTarget()
	{
		return mRenderTarget;
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

			if (mRenderTarget != NULL)
			{
				vp.w = mRenderTarget->GetWidth();
				vp.h = mRenderTarget->GetHeight();
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
			int w, h;
			if (mRenderTarget != NULL)
			{
				w = mRenderTarget->GetWidth();
				h = mRenderTarget->GetHeight();
			}
			else
			{
				w = RenderSystem::Instance()->GetConfig().width;
				h = RenderSystem::Instance()->GetConfig().height;
			}

			mCamera->SetAspect((float)w / h);
		}

		if (mVisibleCuller != NULL && mCamera != NULL)
			mVisibleCuller->DoCull(frameId, mCamera);

		 E_RenderBegin(this);

		 RenderSystem::Instance()->_clearState();

		// pre-process
		for (int i = 0; i < mProcesses.Size(); ++i)
		{
			if (mProcesses[i]->IsEnable() && !mProcesses[i]->IsPost())
			{
				mProcesses[i]->DoProcess();
			}
		}

		// render scene
		RenderSystem::Instance()->SetRenderTarget(mRenderTarget.c_ptr());
		RenderSystem::Instance()->SetDepthBuffer(mDepthBuffer.c_ptr());

		RenderSystem::Instance()->SetViewport(mViewport);

		RenderSystem::Instance()->Clear(mClearMode, mClearColor, mClearDepth, mClearStencil);

		if (mCamera != NULL)
			RenderSystem::Instance()->SetCamera(mCamera);

		if (mRenderPipeline != NULL)
			mRenderPipeline->DoRender();

		// post-process
		for (int i = 0; i < mProcesses.Size(); ++i)
		{
			if (mProcesses[i]->IsEnable() && mProcesses[i]->IsPost())
			{
				mProcesses[i]->DoProcess();
			}
		}

		E_RenderEnd(this);
	}

	void RenderContext::ReadPixelData(void * data, int x, int y, int w, int h)
	{
		d_assert (x >= mViewport.x && y >= mViewport.y && w <= mViewport.w && h <= mViewport.h);
		d_assert (data != NULL);

		RenderSystem::Instance()->SetRenderTarget(mRenderTarget.c_ptr());
		RenderSystem::Instance()->ReadPixelData(data, x, y, w, h);
	}

	void RenderContext::AddProcess(RenderProcess * p)
	{
		for (int i = 0; i < mProcesses.Size(); ++i)
		{
			if (mProcesses[i]->GetOrder() >= p->GetOrder())
			{
				mProcesses.Insert(i, p);
				return ;
			}
		}

		mProcesses.PushBack(p);
	}

	void RenderContext::RemoveProcess(RenderProcess * p)
	{
		for (int i = 0; i < mProcesses.Size(); ++i)
		{
			if (mProcesses[i] == p)
			{
				delete p;
				mProcesses.Erase(i);
				return ;
			}
		}
	}

	RenderProcess * RenderContext::GetProcess(const RTTI_INFO * rtti)
	{
		for (int i = 0; i < mProcesses.Size(); ++i)
		{
			if (mProcesses[i]->GetRTTI() == rtti)
			{
				return mProcesses[i];
			}
		}

		return NULL;
	}

}
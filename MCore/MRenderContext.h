/*
*	RenderContext
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderTarget.h"
#include "MDepthBuffer.h"
#include "MVisibleCuller.h"
#include "MRenderPipeline.h"
#include "MShaderProvider.h"

namespace Rad {

	class M_ENTRY RenderContext
	{
		DECLARE_ALLOC();

	public:
		RenderContext(int order, const String & name, int id = -1);
		~RenderContext();

		int
			GetId() { return mId; }
		int
			GetOrder() { return mOrder; }
		const String & 
			GetName() { return mName ;}
		
		void 
			SetEnable(bool enable);
		bool 
			IsEnable() { return mEnable; }

		void 
			SetColorClear(eClearMode mode, const Float4 & color, float depth = 1, int stencil = 0);

		void 
			SetRenderTarget(int i, RenderTargetPtr p);
		RenderTargetPtr 
			GetRenderTarget(int i);

		void
			SetDepthBuffer(DepthBufferPtr p);
		DepthBufferPtr
			GetDepthBuffer();

		void 
			SetCamera(Camera * camera);
		Camera * 
			GetCamera();

		void 
			SetViewport(const Viewport & vp);
		Viewport 
			GetViewport();

		void 
			SetVisibleCuller(VisibleCullerPtr p);
		VisibleCullerPtr 
			GetVisibleCuller();

		void 
			SetShaderProvider(ShaderProviderPtr p);
		ShaderProviderPtr
			GetShaderProvider();

		void 
			SetRenderPipeline(RenderPipelinePtr p);
		RenderPipelinePtr 
			GetRenderPipeline();

		void 
			DoRender(int frameId);

		void 
			ReadPixels(void * pixels, int x, int y, int w, int h);

	protected:
		int mId;
		int mOrder;
		String mName;
		bool mEnable;

		eClearMode mClearMode;
		Float4 mClearColor;
		float mClearDepth;
		int mClearStencil;

		Camera * mCamera;
		Viewport mViewport;
		VisibleCullerPtr mVisibleCuller;
		ShaderProviderPtr mShaderProvider;
		RenderPipelinePtr mRenderPipeline;

		RenderTargetPtr mRenderTarget[MAX_HW_RENDERTARGET];
		DepthBufferPtr mDepthBuffer;
	};

}

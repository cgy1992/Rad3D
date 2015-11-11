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
#include "MRenderProcess.h"

namespace Rad {

	class M_ENTRY RenderContext
	{
		DECLARE_ALLOC();

	public:
		tEvent1<RenderContext *> E_RenderBegin;
		tEvent1<RenderContext *> E_RenderEnd;

	public:
		RenderContext(int id, int order, const String & name);
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
			SetMatchId(bool matched);
		bool
			IsMatchId() { return mMatchId; }

		void 
			SetColorClear(eClearMode mode, const Float4 & color, float depth = 1, int stencil = 0);

		void 
			SetRenderTarget(RenderTargetPtr p);
		RenderTargetPtr 
			GetRenderTarget();

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
			ReadPixelData(void * data, int x, int y, int w, int h);

		void
			_addProcess(RenderProcess * p);
		void
			_removeProcess(RenderProcess * p);
		RenderProcess *
			GetProcess(const RTTI_INFO * rtti);
		RenderProcess *
			GetProcess(int i) { return mProcesses[i]; }
		int
			GetProcessCount() { return mProcesses.Size(); }

	protected:
		int mId;
		int mOrder;
		String mName;
		bool mEnable;
		bool mMatchId;

		eClearMode mClearMode;
		Float4 mClearColor;
		float mClearDepth;
		int mClearStencil;

		Camera * mCamera;
		Viewport mViewport;
		VisibleCullerPtr mVisibleCuller;
		ShaderProviderPtr mShaderProvider;
		RenderPipelinePtr mRenderPipeline;

		RenderTargetPtr mRenderTarget;
		DepthBufferPtr mDepthBuffer;

		Array<RenderProcess *> mProcesses;
	};

}

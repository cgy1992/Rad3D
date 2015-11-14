/*
*	RenderSystem
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"
#include "MLight.h"
#include "MCamera.h"
#include "MRenderRegister.h"
#include "MShaderFXManager.h"
#include "MHWBufferManager.h"
#include "MHWBufferManager.h"
#include "MRenderHelper.h"

#define QUERY_OCCLUSION 0x01

namespace Rad {

	class M_ENTRY RenderSystem : public Singleton<RenderSystem>
	{
	public:
		struct Config
		{
			int width, height;
			int colorBits;
			int alphaBits;
			int depthBits;
			int stencilBits;
			bool msaa;

			Config()
			{
				width = height = 1;

#ifdef M_PLATFORM_WIN32
				colorBits = 24;
				alphaBits = 8;
				depthBits = 24;
				stencilBits = 8;
				msaa = false;
#else
				colorBits = 16;
				alphaBits = 0;
				depthBits = 24;
				stencilBits = 8;
				msaa = false;
#endif
			}
		};

		struct HWCaps
		{
			String vender;
			String renderer;
			String version;

			int maxTexSize;
			int maxTexUnit;
			Field<bool, 32> pixelFormats;
		};

	public:
		tEvent2<int, int> E_Resize;

		tEvent1<RenderObject *> E_PreRendering;
		tEvent1<RenderObject *> E_PostRendering;

		tEvent0 E_LostDevice;
		tEvent0 E_ResetDevice;

	public:
		RenderSystem();
		virtual ~RenderSystem();

		const HWCaps &
			GetCaps() { return mCaps; }

		const Config & 
			GetConfig() { return mConfig; }

		void 
			OnInit();
		void 
			OnShutdown();

		virtual void 
			OnResize(int w, int h) = 0;

		virtual void 
			Begin() = 0;
		virtual void 
			End() = 0;
		virtual void
			Finish() = 0;

		virtual void
			BeginEvent(const char * name) {}
		virtual void
			EndEvent() {}
		virtual void
			BeginQuery(int flag = QUERY_OCCLUSION) {}
		virtual int
			EndQuery() { return 0; }

		void
			SetViewport(int x, int y, int w, int h) { SetViewport(Viewport(x, y, w, h)); }
		virtual void 
			SetViewport(const Viewport & vp) = 0;

		virtual void 
			Clear(eClearMode mode, const Float4 & color, float depth, int stencil) = 0;

		void
			_clearState();

		void 
			SetWorldTM(const Mat4 & worldTM);
		void 
			SetViewTM(const Mat4 & viewTM);
		virtual void 
			SetProjTM(const Mat4 & projTM) = 0;

		void 
			SetCamera(Camera * camera);

		void 
			SetMaterial(
				const Float3 & emissive,
				const Float3 & ambient,
				const Float3 & diffuse,
				const Float3 & specular,
				float shininess,
				float opacity);
		void
			SetMaterialRenderState(
				eFillMode fillMode,
				eCullMode cullMode,
				eDepthMode depthCheck,
				eBlendMode blendMode);
		void 
			SetLight(Light * light);
		void 
			SetFog(const Float3 & fogColor, float fogNear, float fogFar);
		void 
			SetClipPlane(float _near, float _far);
		void 
			SetTime(float time);

		void 
			SetTexture(int index, Texture * texture);

		void 
			SetRenderState(
				eFillMode fillMode, 
				eCullMode cullMode, 
				eDepthMode depthMode = eDepthMode::LESS_EQUAL,  
				eBlendMode blendMode = eBlendMode::OPACITY, 
				eColorMode colorMode = eColorMode::ALL);
		void 
			SetShaderState(float alphaTest, float reserved1, float reserved2, float reserved3);
		void 
			SetShaderPass(ShaderPass * pass, bool _bindState);

		void 
			SetRenderTarget(int i, RenderTarget * p);
		void
			SetDepthBuffer(DepthBuffer * p);
		virtual void
			PrepareRendering() = 0;

		virtual void 
			ReadPixels(void * pixels, int x, int y, int w, int h) = 0;
		virtual void
			StretchRect(RenderTarget * rtDest, RectI * rcDest, RenderTarget * rtSrc, RectI * rcSrc, eTexFilter filter = eTexFilter::POINT) = 0;

		void 
			Render(ShaderFX * fx, RenderObject * able);
		void 
			Render(ShaderFX * fx, RenderOp * rop);
		virtual void 
			Render(RenderOp * rop) = 0;
		virtual void 
			RenderEx(VertexDeclaration * decl, const void * vertexData, const void * indexData, ePrimType primType, int primCount) = 0;
		virtual void 
			RenderScreenQuad(ShaderFX * fx) = 0;

		const Viewport &
			_getCurrentViewport() { return mCurrentViewport; }
		RenderTarget * 
			_getCurrentRenderTarget(int i) { return mCurrentRenderTarget[i]; }
		DepthBuffer *
			_getCurrentDepthBuffer() { return mCurrentDepthBuffer; }
		Texture *
			_getCurrentTexture(int i) { d_assert(i < MAX_HW_SAMPLER); return mCurrentTextures[i]; }
		RenderState 
			_getCurrentRenderState() { return mCurrentRenderState; }
		ShaderPass * 
			_getCurrentShaderPass() { return mCurrentShaderPass; }

		int 
			_getBatchCount() { return mBatchCount; }
		int 
			_getPrimitiveCount() { return mPrimitiveCount; }

		void 
			SetColorWriteEnable(bool enable) { mColorWriteEnable = enable; mRenderStateChanged = true; }
		bool 
			GetColorWriteEnable() { return mColorWriteEnable; }

		void
			MapScreenPos(float & x, float & y) { x /= mConfig.width, y /= mConfig.height; }

	protected:
		HWCaps mCaps;
		Config mConfig;

		RenderHelper * mRenderHelper;
		RenderRegister * mRenderRegister;

		Viewport mCurrentViewport;
		RenderTarget * mCurrentRenderTarget[MAX_HW_RENDERTARGET];
		DepthBuffer * mCurrentDepthBuffer;
		bool mRenderTargetChanged;

		bool mRenderStateChanged;
		eFillMode mMaterialFillMode;
		eCullMode mMaterialCullMode;
		eDepthMode mMaterialDepthMode;
		eBlendMode mMaterialBlendMode;
		RenderState mCurrentRenderState;

		ShaderPass * mCurrentShaderPass;
		Texture * mCurrentTextures[MAX_HW_SAMPLER];

		bool mShaderPassChanged;
		bool mTextureChanged[MAX_HW_SAMPLER];

		VertexBuffer * mCurrentVertexBuffer;

		Mat4 mBoneTM[MAX_HW_BONE];

		int mBatchCount;
		int mPrimitiveCount;

		bool mColorWriteEnable;
	};

#if defined(M_DEBUG) || defined(M_PROFILE)
	#define RENDER_EVENT_BEGIN(name) RenderSystem::Instance()->BeginEvent(name)
	#define RENDER_EVENT_END() RenderSystem::Instance()->EndEvent()

	struct __render_event_entry
	{
		__render_event_entry(const char * name)
		{
			RENDER_EVENT_BEGIN(name);
		}

		~__render_event_entry()
		{
			RENDER_EVENT_END();
		}
	};

	#define RENDER_EVENT_ENTRY(name) __render_event_entry(name)

#else
	#define RENDER_EVENT_BEGIN(name)
	#define RENDER_EVENT_END()
	#define RENDER_EVENT_ENTRY(name)
#endif
}

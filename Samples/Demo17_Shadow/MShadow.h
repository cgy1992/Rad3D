/*
*	Shadow
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderContext.h"
#include "MShaderProvider.h"
#include "MVisibleCuller.h"
#include "MRenderPipeline.h"

namespace Rad {

	class Shadow
	{
	public:
		tEvent1<Shadow *> E_RenderDepth;
		tEvent1<Shadow *> E_RenderShadow;

	public:
		Shadow(RenderContext * context, int mapSize);
		virtual ~Shadow();

		void
			SetDistance(float distance);
		float
			GetDistance() { return mDistance; }

		void
			SetFadeRatio(float fadeRatio);
		float
			GetFadeRatio() { return mFadeRatio; }

		void
			SetOffset(float offset);
		float
			GetOffset() { return mOffset; }

		void
			SetColor(const Float4 & color);
		const Float4 &
			GetColor() { return mColor; }
		
		void
			OnResize() {}
		
		Camera *
			GetShadowCamera() { return mShadowCamera; }
		RenderContext *
			GetShadowContext() { return mShadowContext; }

	protected:
		void
			_updateCamera();
		void
			_renderShadow();

	protected:
		RenderContext * mContext;
		int mMapSize;
		float mDistance;
		float mFadeRatio;
		float mOffset;
		Float4 mColor;
	
		Camera * mShadowCamera;
		RenderContext * mShadowContext;

		cListener0<Shadow> L_OnUpdateCamera;
		cListener0<Shadow> L_OnRenderShadow;
	};

	//
	class ShadowVisibleCuller : public VisibleCuller
	{
	public:
		ShadowVisibleCuller();
		virtual ~ShadowVisibleCuller() {}

		virtual void 
			DoCull(int frameId, Camera * camera);
	};

	//
	class ShadowShaderProvider : public ShaderProvider
	{
	public:
		ShadowShaderProvider();
		virtual ~ShadowShaderProvider();

		virtual void 
			ApplyShaderFX(RenderObject * able, int flag);

		virtual void 
			ApplyLightingFX(Light * light, RenderObject * able) {};

	protected:
		ShaderFX * mCasterFX;
		ShaderFX * mCasterFXSkined;

		ShaderFX * mReceiverFX;
		ShaderFX * mReceiverFXSkined;
	};

	//
	class ShadowRenderPipline : public RenderPipeline
	{
	public:
		ShadowRenderPipline(Shadow * shadow);
		virtual ~ShadowRenderPipline() {}

		virtual void
			DoRender();

	protected:
		Shadow * mShadow;
	};
}


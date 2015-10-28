/*
*	Shadow
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MCamera.h"
#include "MRenderProcess.h"
#include "MShaderProvider.h"
#include "MVisibleCuller.h"
#include "MRenderPipeline.h"

namespace Rad {

	class RenderContext;

	class M_ENTRY Shadow : public RenderProcess
	{
		DECLARE_RTTI();

	public:
		tEvent1<Shadow *> E_RenderDepth;
		tEvent1<Shadow *> E_RenderShadow;

	public:
		Shadow();
		virtual ~Shadow();

		void
			SetMapSize(int size);
		int
			GetMapSize() { return mMapSize; }

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


		virtual void
			OnEnable();
		virtual void
			OnDisable();
		virtual void 
			DoProcess();

		Camera *
			GetCamera() { return mCamera; }
		RenderContext *
			GetContext() { return mContext; }

	protected:
		void
			_updateRT();
		void
			_updateCamera();
		void
			_renderDepth();
		void
			_renderShadow();

	protected:
		int mMapSize;
		float mDistance;
		float mFadeRatio;
		float mOffset;
		Float4 mColor;
	
		Camera * mCamera;
		RenderContext * mContext;
		RenderContext * mOwnerContext;

		cListener0<Shadow> L_OnRender;
	};

	//
	class M_ENTRY ShadowVisibleCuller : public VisibleCuller
	{
	public:
		ShadowVisibleCuller();
		virtual ~ShadowVisibleCuller() {}

		virtual void 
			DoCull(int frameId, Camera * camera);
	};

	//
	class M_ENTRY ShadowShaderProvider : public ShaderProvider
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
	class M_ENTRY ShadowRenderPipline : public RenderPipeline
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


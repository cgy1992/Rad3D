/*
*	Bloom
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MShaderFX.h"
#include "MRenderContext.h"

namespace Rad {

	class Bloom
	{
	public:
		Bloom(RenderContext * context);
		virtual ~Bloom();

		void 
			SetThreshold(float v);
		float
			GetThreshold() { return mThreshold; }

		void
			SetDensity(float v);
		float
			GetDensity() { return mDensity; }

		void
			OnResize();
		void
			OnRender();

	protected:
		void 
			_doDownSample();
		void 
			_doBlurH();
		void 
			_doBlurV();
		void 
			_doBlend();

	protected:
		RenderContext * mContext;
		float mThreshold;
		float mDensity;
		Float4 mInvMapSize;

		ShaderFX * mTechDownSample;
		ShaderFX * mTechBlurH;
		ShaderFX * mTechBlurV;
		ShaderFX * mTechBlend;

		RenderTargetPtr mRTQuad1;
		RenderTargetPtr mRTQuad2;
	};

}
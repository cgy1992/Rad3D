/*
*	Bloom
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderProcess.h"
#include "MShaderFX.h"

namespace Rad {

	class M_ENTRY Bloom : public RenderProcess
	{
		DECLARE_RTTI();

	public:
		Bloom();
		virtual ~Bloom();

		void 
			SetThreshold(float v);
		float
			GetThreshold() { return mThreshold; }

		void
			SetDensity(float v);
		float
			GetDensity() { return mDensity; }

		virtual void
			OnEnable();
		virtual void
			OnDisable();
		virtual void
			DoProcess();

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
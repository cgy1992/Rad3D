#include "MBloom.h"
#include "MWorld.h"
#include "MRenderSystem.h"

namespace Rad {

	Bloom::Bloom(RenderContext * context)
		: mContext(context)
		, mTechDownSample(NULL)
		, mTechBlurH(NULL)
		, mTechBlurV(NULL)
		, mTechBlend(NULL)
	{
		SetThreshold(0.8f);
		SetDensity(0.6f);

		mTechDownSample = ShaderFXManager::Instance()->Load("BloomDownSample", "Bloom/DownSample.mfx");
		mTechBlurH = ShaderFXManager::Instance()->Load("BloomBlurH", "Bloom/BlurH.mfx");
		mTechBlurV = ShaderFXManager::Instance()->Load("BloomBlurV", "Bloom/BlurV.mfx");
		mTechBlend = ShaderFXManager::Instance()->Load("BloomBlend", "Bloom/Blend.mfx");

		OnResize();
	}

	Bloom::~Bloom()
	{
		ShaderFXManager::Instance()->Remove(mTechDownSample);
		ShaderFXManager::Instance()->Remove(mTechBlurH);
		ShaderFXManager::Instance()->Remove(mTechBlurV);
		ShaderFXManager::Instance()->Remove(mTechBlend);

		mRTQuad1 = NULL;
		mRTQuad2 = NULL;

		mTechDownSample = NULL;
		mTechBlurH = NULL;
		mTechBlurV = NULL;
		mTechBlend = NULL;
	}

	void Bloom::SetThreshold(float v)
	{
		mThreshold = v;
		mThreshold = Max(0.01f, mThreshold);
	}

	void Bloom::SetDensity(float v)
	{
		mDensity = v;
	}

	void Bloom::OnResize()
	{
		Viewport vp = mContext->GetViewport();
		int width = vp.w / 2;
		int height = vp.h / 2;

		mRTQuad1 = HWBufferManager::Instance()->NewRenderTarget(width, height);
		mRTQuad2 = HWBufferManager::Instance()->NewRenderTarget(width, height);
	}

	void Bloom::OnRender()
	{
		RenderContext * context = mContext;
		if (context != NULL && context->GetRenderTarget(0) != NULL)
		{
			mInvMapSize.x = 0.5f / RenderSystem::Instance()->GetConfig().width;
			mInvMapSize.y = 0.5f / RenderSystem::Instance()->GetConfig().height;

			_doDownSample();

			_doBlurH();

			_doBlurV();

			_doBlend();
		}
	}

	void Bloom::_doDownSample()
	{
		RenderContext * context = mContext;

		RenderSystem::Instance()->SetRenderTarget(0, mRTQuad1.c_ptr());
		RenderSystem::Instance()->SetDepthBuffer(NULL);
		RenderSystem::Instance()->PrepareRendering();

		RenderSystem::Instance()->SetViewport(Viewport());

		RenderSystem::Instance()->SetTexture(0, context->GetRenderTarget(0)->GetTexture().c_ptr());

		mTechDownSample->GetPass(0)->SetConst("u_Threshold", Float4(mThreshold, 1 / (1 - mThreshold), 0));

		RenderSystem::Instance()->RenderScreenQuad(mTechDownSample);
	}

	void Bloom::_doBlurH()
	{
		RenderSystem::Instance()->SetRenderTarget(0, mRTQuad2.c_ptr());
		RenderSystem::Instance()->SetDepthBuffer(NULL);
		RenderSystem::Instance()->PrepareRendering();

		RenderSystem::Instance()->SetViewport(Viewport());

		RenderSystem::Instance()->SetTexture(0, mRTQuad1->GetTexture().c_ptr());

		mTechBlurH->GetPass(0)->SetConst("u_InvMapSize", mInvMapSize);

		RenderSystem::Instance()->RenderScreenQuad(mTechBlurH);
	}

	void Bloom::_doBlurV()
	{
		RenderSystem::Instance()->SetRenderTarget(0, mRTQuad1.c_ptr());
		RenderSystem::Instance()->SetDepthBuffer(NULL);
		RenderSystem::Instance()->PrepareRendering();

		RenderSystem::Instance()->SetViewport(Viewport());

		RenderSystem::Instance()->SetTexture(0, mRTQuad2->GetTexture().c_ptr());

		mTechBlurV->GetPass(0)->SetConst("u_InvMapSize", mInvMapSize);

		RenderSystem::Instance()->RenderScreenQuad(mTechBlurV);
	}

	void Bloom::_doBlend()
	{
		RenderContext * context = mContext;

		RenderSystem::Instance()->SetRenderTarget(0, context->GetRenderTarget(0).c_ptr());
		RenderSystem::Instance()->SetDepthBuffer(NULL);
		RenderSystem::Instance()->PrepareRendering();

		RenderSystem::Instance()->SetViewport(Viewport());

		RenderSystem::Instance()->SetTexture(0, mRTQuad1->GetTexture().c_ptr());

		mTechBlend->GetPass(0)->SetConst("u_Density", Float4(mDensity, 0, 0));

		RenderSystem::Instance()->RenderScreenQuad(mTechBlend);
	}

}
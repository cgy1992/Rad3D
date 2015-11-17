#include "MRenderSystem.h"
#include "MImage.h"

namespace Rad {

	ImplementSingleton(RenderSystem);

    RenderSystem::RenderSystem()
		: mRenderHelper(NULL)
		, mBatchCount(0)
		, mPrimitiveCount(0)
		, mColorWriteEnable(true)
    {
		IMG_Reg(PNG_Test, PNG_Load);
		IMG_Reg(DDS_Test, DDS_Load);
		IMG_Reg(ETC_Test, ETC_Load);
		IMG_Reg(PVR_Test, PVR_Load);
		IMG_Reg(BMP_Test, BMP_Load);
		IMG_Reg(TGA_Test, TGA_Load);
		IMG_Reg(JPG_Test, JPG_Load);

		for (int i = 0; i < mCaps.pixelFormats.Size(); ++i)
			mCaps.pixelFormats[i] = true;
    }

    RenderSystem::~RenderSystem()
    {
		IMG_Clear();
    }

	void RenderSystem::_clearState()
	{
		mCurrentShaderPass = NULL;
		mShaderPassChanged = true;
		mRenderStateChanged = true;

		mCurrentRenderState.FillMode = eFillMode::SOLID;
		mCurrentRenderState.CullMode = eCullMode::FRONT;
		mCurrentRenderState.DepthMode = eDepthMode::LESS_EQUAL;
		mCurrentRenderState.BlendMode = eBlendMode::OPACITY;
		mCurrentRenderState.ColorMode = eColorMode::ALL;

		mMaterialCullMode = eCullMode::FRONT;
		mMaterialDepthMode = eDepthMode::LESS_EQUAL;
		mMaterialBlendMode = eBlendMode::OPACITY;

		for (int i = 0; i < MAX_HW_SAMPLER; ++i)
		{
			mCurrentTextures[i] = NULL;
			mTextureChanged[i] = true;
		}

		mCurrentVertexBuffer = NULL;

		for (int i = 0; i < MAX_HW_RENDERTARGET; ++i)
		{
			mCurrentRenderTarget[i] = NULL;
		}
		mCurrentDepthBuffer = NULL;
		mRenderTargetChanged = true;
	}

	void RenderSystem::OnInit()
	{
		d_assert (mRenderHelper == NULL);

		mRenderRegister = new RenderRegister;
		mRenderHelper = new RenderHelper;

		_clearState();
	}

	void RenderSystem::OnShutdown()
	{
		d_assert (mRenderHelper != NULL);

		_clearState();

		delete mRenderHelper;
		delete mRenderRegister;
	}

	void RenderSystem::SetWorldTM(const Mat4 & worldTM)
	{
		mRenderRegister->SetWorldTM(worldTM);
	}

	void RenderSystem::SetViewTM(const Mat4 & viewTM)
	{
		mRenderRegister->SetViewTM(viewTM);
	}

	void RenderSystem::SetProjTM(const Mat4 & projTM)
	{
		mRenderRegister->SetProjTM(projTM * _getAdjustProjTM());
	}

	void RenderSystem::SetCamera(Camera * camera)
	{
		mRenderRegister->SetCameraPosition(camera->GetPosition());
		
		mRenderRegister->SetCameraRightVector(camera->GetRotation().GetRightVector());
		mRenderRegister->SetCameraUpVector(camera->GetRotation().GetUpVector());
		mRenderRegister->SetCameraDirVector(camera->GetRotation().GetDirVector());

		mRenderRegister->SetClipPlane(camera->GetNearClip(), camera->GetFarClip());
	}

	void RenderSystem::SetMaterial(
		const Float3 & emissive, 
		const Float3 & ambient,
		const Float3 & diffuse,
		const Float3 & specular,
		float shininess,
		float opacity)
	{
		if (opacity != 1)
			mMaterialBlendMode = eBlendMode::ALPHA_BLEND;

		mRenderRegister->SetMaterial(emissive, ambient, diffuse, specular, shininess, opacity);
	}

	void RenderSystem::SetMaterialRenderState(
		eFillMode fillMode,
		eCullMode cullMode,
		eDepthMode depthMode,
		eBlendMode blendMode)
	{
		mMaterialFillMode = fillMode;
		mMaterialCullMode = cullMode;
		mMaterialDepthMode = depthMode;
		mMaterialBlendMode = blendMode;
	}

	void RenderSystem::SetLight(Light * light)
	{
		if (light)
		{
			mRenderRegister->SetLightPosition(light->GetPosition());
			mRenderRegister->SetLightDirection(light->GetWorldRotation().GetDirVector());
			mRenderRegister->SetLightColor(light->GetAmbient(), light->GetDiffuse(), light->GetSpecular());
			mRenderRegister->SetLightAttenParam(light->GetAttenParam());
			mRenderRegister->SetLightSpotParam(light->GetSpotParam());
		}
		else
		{
			mRenderRegister->SetLightPosition(Float3(0, 0, 0));
			mRenderRegister->SetLightDirection(Float3(0, 0, 1));
			mRenderRegister->SetLightColor(Float3(1, 1, 1), Float3(1, 1, 1), Float3(1, 1, 1));
			mRenderRegister->SetLightAttenParam(Float4(0, 0, 0, 0));
			mRenderRegister->SetLightSpotParam(Float4(0, 0, 0, 0));
		}
	}

	void RenderSystem::SetFog(const Float3 & fogColor, float fogNear, float fogFar)
	{
		mRenderRegister->SetFogColor(fogColor);
		mRenderRegister->SetFogParam(fogNear, fogFar);
	}
	
	void RenderSystem::SetClipPlane(float _near, float _far)
	{
		RenderRegister::Instance()->SetClipPlane(_near, _far);
	}

	void RenderSystem::SetTime(float time)
	{
		RenderRegister::Instance()->SetTime(time);
	}

	void RenderSystem::SetShaderPass(ShaderPass * pass, bool _bindState)
	{
		if (mCurrentShaderPass != pass)
		{
			mCurrentShaderPass = pass;
			mShaderPassChanged = true;
		}

		if (_bindState)
		{
			SetRenderState(
				pass->GetRenderState()->FillMode,
				pass->GetRenderState()->CullMode,
				pass->GetRenderState()->DepthMode,
				pass->GetRenderState()->BlendMode,
				pass->GetRenderState()->ColorMode);

			for (int i = 0; i < pass->GetSamplerCount(); ++i)
			{
				FX_Sampler * sampler = pass->GetSampler(i);

				if (sampler->BindType == eSamplerBindType::SAMPLER)
				{
					SetTexture(sampler->Index, sampler->SamplerTexture.c_ptr());
				}
			}
		}
	}

	void RenderSystem::SetRenderTarget(int i, RenderTarget * p)
	{
		if (mCurrentRenderTarget[i] != p)
		{
			mCurrentRenderTarget[i] = p;
			mRenderTargetChanged = true;
		}
	}

	void RenderSystem::SetDepthBuffer(DepthBuffer * p)
	{
		if (mCurrentDepthBuffer != p)
		{
			mCurrentDepthBuffer = p;
			mRenderTargetChanged = true;
		}
	}

	void RenderSystem::SetTexture(int index, Texture * texture)
	{
		if (mCurrentTextures[index] != texture)
		{
			mCurrentTextures[index] = texture;
			mTextureChanged[index] = true;
		}
	}
	
	void RenderSystem::SetRenderState(
		eFillMode fillMode, eCullMode cullMode, eDepthMode depthMode, eBlendMode blendMode, eColorMode colorMode)
	{
		if (fillMode == eFillMode::INHERIT)
			fillMode = mMaterialFillMode;

		if (cullMode == eCullMode::INHERIT)
			cullMode = mMaterialCullMode;

		if (depthMode == eDepthMode::INHERIT)
			depthMode = mMaterialDepthMode;

		if (blendMode == eBlendMode::INHERIT)
			blendMode = mMaterialBlendMode;

		if (mCurrentRenderState.FillMode != fillMode ||
			mCurrentRenderState.CullMode != cullMode ||
			mCurrentRenderState.DepthMode != depthMode ||
			mCurrentRenderState.BlendMode != blendMode ||
			mCurrentRenderState.ColorMode != colorMode)
		{
			mCurrentRenderState.FillMode = fillMode;
			mCurrentRenderState.CullMode = cullMode;
			mCurrentRenderState.DepthMode = depthMode;
			mCurrentRenderState.BlendMode = blendMode;
			mCurrentRenderState.ColorMode = colorMode;

			mRenderStateChanged = true;
		}
	}

	void RenderSystem::SetShaderState(float alphaTest, float reserved1, float reserved2, float reserved3)
	{
		mRenderRegister->SetShaderState(alphaTest, 0, 0, 0);
	}

	void RenderSystem::Render(ShaderFX * fx, RenderObject * able)
	{
		if (fx == NULL)
			return ;

		for (int i = 0; i < eRenderCallBack::MAX; ++i)
		{
			RenderCallBack * callback = able->GetRenderCallBack(i);
			if (callback)
			{
				callback->OnCallBack(able);
			}
		}

		able->_preRendering();

		E_PreRendering(able);

		Material * mtl = able->GetMaterial();
		RenderOp * rop = able->GetRenderOp();
		float opacity = able->_getOpacity();
		
		// Material
		SetMaterialRenderState(
			mtl->fillMode, 
			mtl->cullMode, 
			mtl->depthMode, 
			mtl->blendMode);

		SetMaterial(
			mtl->emissive, 
			mtl->ambient, 
			mtl->diffuse, 
			mtl->specular, 
			mtl->shininess, 
			opacity);


		// Transform
		Mat4 worldTm;
		able->_getWorldTM(worldTm);
		SetWorldTM(worldTm);

		// BoneTM
		int count = able->_getBoneTM(mBoneTM);
		mRenderRegister->SetBoneTM(mBoneTM, count);

		// ShaderPass
		for (int i = 0; i < fx->GetPassCount(); ++i)
		{
			ShaderPass * pass = fx->GetPass(i);

			SetShaderPass(pass, false);

			SetRenderState(
				pass->GetRenderState()->FillMode,
				pass->GetRenderState()->CullMode,
				pass->GetRenderState()->DepthMode,
				pass->GetRenderState()->BlendMode,
				pass->GetRenderState()->ColorMode);

			SetShaderState(mCurrentRenderState.BlendMode != eBlendMode::ALPHA_TEST ? 0.0f : 1.0f, 0, 0, 0);

			for (int i = 0; i < pass->GetSamplerCount(); ++i)
			{
				FX_Sampler * sampler = pass->GetSampler(i);

				switch(sampler->BindType)
				{
				case eSamplerBindType::EMISSIVE:
					SetTexture(sampler->Index, mtl->maps[eMapType::EMISSIVE].c_ptr());
					break;
				case eSamplerBindType::DIFFUSE:
					SetTexture(sampler->Index, mtl->maps[eMapType::DIFFUSE].c_ptr());
					break;
				case eSamplerBindType::SPECULAR:
					SetTexture(sampler->Index, mtl->maps[eMapType::SPECULAR].c_ptr());
					break;
				case eSamplerBindType::NORMAL:
					SetTexture(sampler->Index, mtl->maps[eMapType::NORMAL].c_ptr());
					break;
				case eSamplerBindType::LIGHTING_MAP:
					SetTexture(sampler->Index, mtl->maps[eMapType::LIGHTING_MAP].c_ptr());
					break;

				case eSamplerBindType::EXTERN0:
					SetTexture(sampler->Index, mtl->maps[eMapType::EXTERN0].c_ptr());
					break;
				case eSamplerBindType::EXTERN1:
					SetTexture(sampler->Index, mtl->maps[eMapType::EXTERN1].c_ptr());
					break;
				case eSamplerBindType::EXTERN2:
					SetTexture(sampler->Index, mtl->maps[eMapType::EXTERN2].c_ptr());
					break;
				case eSamplerBindType::EXTERN3:
					SetTexture(sampler->Index, mtl->maps[eMapType::EXTERN3].c_ptr());
					break;
				case eSamplerBindType::EXTERN4:
					SetTexture(sampler->Index, mtl->maps[eMapType::EXTERN4].c_ptr());
					break;
				case eSamplerBindType::EXTERN5:
					SetTexture(sampler->Index, mtl->maps[eMapType::EXTERN5].c_ptr());
					break;
				case eSamplerBindType::EXTERN6:
					SetTexture(sampler->Index, mtl->maps[eMapType::EXTERN6].c_ptr());
					break;
				case eSamplerBindType::EXTERN7:
					SetTexture(sampler->Index, mtl->maps[eMapType::EXTERN7].c_ptr());
					break;

				case eSamplerBindType::SAMPLER:
					SetTexture(sampler->Index, sampler->SamplerTexture.c_ptr());
					break;
				}
			}

			Render(rop);
		}

		able->_postRendering();

		E_PostRendering(able);
	}

	void RenderSystem::Render(ShaderFX * fx, RenderOp * rop)
	{
		for (int i = 0; i < fx->GetPassCount(); ++i)
		{
			ShaderPass * pass = fx->GetPass(i);

			SetShaderPass(pass, false);

			SetRenderState(
				pass->GetRenderState()->FillMode,
				pass->GetRenderState()->CullMode,
				pass->GetRenderState()->DepthMode,
				pass->GetRenderState()->BlendMode,
				pass->GetRenderState()->ColorMode);

			SetShaderState(mCurrentRenderState.BlendMode != eBlendMode::ALPHA_TEST ? 0.0f : 1.0f, 0, 0, 0);

			for (int i = 0; i < pass->GetSamplerCount(); ++i)
			{
				FX_Sampler * sampler = pass->GetSampler(i);

				if (sampler->BindType == eSamplerBindType::SAMPLER)
				{
					SetTexture(sampler->Index, sampler->SamplerTexture.c_ptr());
				}
			}

			Render(rop);
		}
	}

}
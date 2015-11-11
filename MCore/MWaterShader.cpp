#include "MWaterShader.h"
#include "MShaderFXManager.h"
#include "MHWBufferManager.h"
#include "MWater.h"
#include "MLog.h"

namespace Rad {

	//
	ImplementRTTI(WaterShader, IObject);

	WaterShader::WaterShader()
	{
	}

	WaterShader::~WaterShader()
	{
	}

	//
	ImplementRTTI(WaterShaderStandard, WaterShader);

	DF_PROPERTY_BEGIN(WaterShaderStandard)
		DF_PROPERTY_EX(WaterShaderStandard, mColor, "Material", "Color", "PT_Color3", PT_Float3)
		DF_PROPERTY(WaterShaderStandard, mUVScale, "Material", "UVScale", PT_Float)
		DF_PROPERTY(WaterShaderStandard, mAlphaScale, "Material", "AlphaScale", PT_Float)
		DF_PROPERTY_EX(WaterShaderStandard, mTextureName, "Animation", "Texture", "Filename", PT_String)
		DF_PROPERTY(WaterShaderStandard, mNumKeyframes, "Animation", "NumKeyFrame", PT_Int)
		DF_PROPERTY(WaterShaderStandard, mDurationTime, "Animation", "Duration", PT_Float)
	DF_PROPERTY_END()

	WaterShaderStandard::WaterShaderStandard()
		: mNumKeyframes(0)
		, mDurationTime(0)
		, mInternalTime(0)
		, mUVScale(0)
		, mAlphaScale(0)
		, mColor(0.1f, 0.3f, 0.3f)
	{
		mFX = ShaderFXManager::Instance()->Load("WaterFX", "Shader/MWater.mfx");
		d_assert (mFX != NULL && mFX->GetPassCount() > 0);

		mAlphaTexture = HWBufferManager::Instance()->LoadTexture("water_depth.tga");

		mUVScale = 0.002f;
		mAlphaScale = 0.005f;
		mDurationTime = 1.0f;
		mNumKeyframes = 28;
		SetTextureName("Water_0.dds");
	}

	WaterShaderStandard::~WaterShaderStandard()
	{
	}

	void WaterShaderStandard::SetTextureName(const String & name)
	{
		mTextureName = name;

		_updateTextures();
	}

	void WaterShaderStandard::SetNumKeyFrame(int numKeyFrames)
	{
		mNumKeyframes = numKeyFrames;

		_updateTextures();
	}

	void WaterShaderStandard::SetDurationTime(float durationTime)
	{
		mDurationTime = durationTime;
	}

	void WaterShaderStandard::_updateTextures()
	{
		String baseName = FileHelper::RemoveExternName(mTextureName);
		String extName = FileHelper::GetExternName(mTextureName);

		int len = baseName.Length();
		while (len >= 0)
		{
			if (baseName[len - 1] == '_')
			{
				baseName[len] = 0;
				break;
			}

			--len;
		}

		if (len > 1)
		{
			for (int i = 0; i < mNumKeyframes; ++i)
			{
				String filename = baseName + "" + i2str(i) + "." + extName;
				TexturePtr t = HWBufferManager::Instance()->LoadTexture(filename, -1);

				mTextures.PushBack(t);
			}
		}
		else
		{
			d_log("!: Water texture '%s' format error.", mTextureName.c_str());
		}
	}

	void WaterShaderStandard::Update(Water * water, float frameTime)
	{
		if (mDurationTime > 0 && mTextures.Size() > 0)
		{
			mInternalTime += frameTime;

			while (mInternalTime >= mDurationTime)
				mInternalTime -= mDurationTime;

			int k = (int)(mInternalTime / mDurationTime * mTextures.Size());
			k = Min(k, mTextures.Size() - 1);

			mKeyFrameIndex = k;

			water->GetMaterial()->maps[eMapType::DIFFUSE] = mTextures[mKeyFrameIndex];
			water->GetMaterial()->maps[eMapType::EXTERN0] = mAlphaTexture;
		}

		water->SetShaderFX(mFX);
	}

	void WaterShaderStandard::OnCallBack(RenderObject * obj)
	{
		mFX->GetPass(0)->SetConst("u_UVScale", Float4(mUVScale, mAlphaScale, 0, 0));
		mFX->GetPass(0)->SetConst("u_Color", Float4(mColor.r, mColor.g, mColor.b, 1));
	}

}
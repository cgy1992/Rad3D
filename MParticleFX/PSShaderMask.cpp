#include "PSShaderMask.h"

namespace Rad {

	ImplementRTTI(PS_ShaderMask, PS_Shader);

	DF_PROPERTY_BEGIN(PS_ShaderMask)
		DF_PROPERTY_EX(PS_ShaderMask, mTextureName, "Mask", "Texture", "PT_Filename", PT_String)
		DF_PROPERTY(PS_ShaderMask, mUVRects, "Mask", "UVRects", PT_Int2)
		DF_PROPERTY(PS_ShaderMask, mLooped, "Mask", "Looped", PT_Bool)
		DF_PROPERTY(PS_ShaderMask, mAnimInterval, "Mask", "AnimInterval", PT_Float)
	DF_PROPERTY_END()

	PS_ShaderMask::PS_ShaderMask()
		: mTextureName("")
		, mUVRects(1, 1)
		, mLooped(false)
		, mAnimInterval(0)
		, mUVIndex(0)
	{
		mFX = ShaderFXManager::Instance()->Load("Shader/MParticleMask.mfx", "Shader/MParticleMask.mfx");

		mTexture = RenderHelper::Instance()->GetWhiteTexture();
	}

	PS_ShaderMask::~PS_ShaderMask()
	{
	}

	void PS_ShaderMask::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mTextureName")
		{
			SetTexture(mTextureName);
		}
		else
		{
			PS_Shader::OnPropertyChanged(p);
		}
	}

	void PS_ShaderMask::Update(float frameTime)
	{
		mInternalTime += frameTime;

		int UVCount = mUVRects.x * mUVRects.y;

		if (mAnimInterval > 0)
		{
			while (mInternalTime > mAnimInterval)
			{
				mUVIndex += 1;
				mInternalTime -= mAnimInterval;
			}

			if (mUVIndex >= UVCount)
			{
				if (mLooped) 
				{
					do 
					{
						mUVIndex -= UVCount;
					} while (mUVIndex > UVCount);
				}
				else
				{
					mUVIndex = UVCount - 1;
				}
			}
		}

		int y = mUVIndex / mUVRects.x;
		int x = mUVIndex - y * mUVRects.x;

		mCurrentUV.x = x / (float)mUVRects.x;
		mCurrentUV.y = y / (float)mUVRects.y;
		mCurrentUV.z = (x + 1) / (float)mUVRects.x;
		mCurrentUV.w = (y + 1)  / (float)mUVRects.y;
	}

	void PS_ShaderMask::OnCallBack(RenderObject * obj)
	{
		obj->GetMaterial()->maps[eMapType::EXTERN0] = mTexture;

		mFX->GetPass(0)->SetConst("u_MaskUV", mCurrentUV);
	}

	void PS_ShaderMask::SetTexture(const String & filename)
	{
		mTextureName = filename;

		if (mTextureName != "")
		{
			mTexture = HWBufferManager::Instance()->LoadTexture(filename);
		}
		else
		{
			mTexture = RenderHelper::Instance()->GetWhiteTexture();
		}
	}

	void PS_ShaderMask::SetUVRects(const Int2 & uvs)
	{
		mUVRects = uvs;
	}

	void PS_ShaderMask::SetLooped(bool b)
	{
		mLooped = b;
	}

	void PS_ShaderMask::SetAnimInterval(float interval)
	{
		mAnimInterval = interval;
	}

}


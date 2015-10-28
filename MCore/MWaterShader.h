/*
*	WaterShader
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"

namespace Rad {

	class Water;

	class M_ENTRY WaterShader : public IObject, public RenderCallBack
	{
		DECLARE_REF();
		DECLARE_RTTI();

	public:
		WaterShader();
		virtual ~WaterShader();

		void
			Release() { delete this; }

		virtual void
			Update(Water * water, float frameTime) = 0;
	};

	typedef SmartPtr<WaterShader> WaterShaderPtr;

	//
	class M_ENTRY WaterShaderStandard : public WaterShader
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(WaterShader);

	protected:
		String mTextureName;
		int mNumKeyframes;
		float mDurationTime;
		Float3 mColor;
		float mUVScale;
		float mAlphaScale;

	public:
		WaterShaderStandard();
		virtual ~WaterShaderStandard();

		void
			SetTextureName(const String & name);
		const String & 
			GetTextureName() { return mTextureName; }

		void
			SetNumKeyFrame(int numKeyFrames);
		int
			GetNumKeyFrame() { return mNumKeyframes; }

		void
			SetDurationTime(float durationTime);
		float
			GetDurationTime() { return mDurationTime; }

		void
			SetColor(const Float3 & color) { mColor = color; }
		const Float3 &
			GetColor() { return mColor; }

		void 
			SetUVScale(float scale) { mUVScale = scale; }
		float 
			GetUVScale() { return mUVScale; }

		void 
			SetAlphaScale(float scale) { mAlphaScale = scale; }
		float 
			GetAlphaScale() { return mAlphaScale; }

		virtual void
			Update(Water * water, float frameTime);
		virtual void
			OnCallBack(RenderObject * obj);

	protected:
		void 
			_updateTextures();

	protected:
		ShaderFX * mFX;

		TexturePtr mAlphaTexture;
		Array<TexturePtr> mTextures;
		float mInternalTime;
		int mKeyFrameIndex;
	};

}
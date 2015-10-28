/*
*	PS_ShaderMask
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_ShaderMask : public PS_Shader
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Shader);

	public:
		String mTextureName;
		Int2 mUVRects;
		bool mLooped;
		float mAnimInterval;

	public:
		PS_ShaderMask();
		virtual ~PS_ShaderMask();

		virtual void 
			OnPropertyChanged(const Property * p);

		virtual void
			Update(float frameTime);

		virtual void
			OnCallBack(RenderObject * obj);

		void
			SetTexture(const String & filename);
		const String &
			GetTexture() const { return mTextureName; }
		TexturePtr
			_getTexture() { return mTexture; }

		void
			SetUVRects(const Int2 & uvs);
		const Int2 &
			GetUVRects() const { return mUVRects; }

		void
			SetLooped(bool b);
		bool
			GetLooped() const { return mLooped; }

		void
			SetAnimInterval(float interval);
		float
			GetAnimInterval() const { return mAnimInterval; }

	protected:
		TexturePtr mTexture;

		int mUVIndex;
		float mInternalTime;
		Float4 mCurrentUV;
	};

}
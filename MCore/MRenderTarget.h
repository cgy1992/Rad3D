/*
*	RenderTarget
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTexture.h"

namespace Rad {

	class M_ENTRY RenderTarget
	{
		DECLARE_REF();

	public:
		RenderTarget();
		virtual ~RenderTarget();

		virtual void
			Release() = 0;

		int
			GetWidth() { return mWidth; }
		int
			GetHeight() { return mHeight; }
		ePixelFormat
			GetFormat() { return mFormat; }
		TexturePtr
			GetTexture() { return mTexture; }

		virtual void
			Resize(int w, int h) = 0;
		
	public:
		int mWidth;
		int mHeight;
		ePixelFormat mFormat;
		TexturePtr mTexture;;
	};

	typedef SmartPtr<RenderTarget> RenderTargetPtr;

}
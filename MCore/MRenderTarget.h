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

		bool
			IsMRT();

		int
			GetWidth() { return mWidth; }
		int
			GetHeight() { return mHeight; }
		eSizeAlign
			GetAlign() { return mAlign; }
		ePixelFormat
			GetFormat(int i = 0) { return mFormats[i]; }
		TexturePtr
			GetTexture(int i = 0) { return mTextures[i]; }

		virtual void
			Resize(int w, int h) = 0;
		
	public:
		int mWidth;
		int mHeight;
		eSizeAlign mAlign;
		ePixelFormat mFormats[MAX_HW_RENDERTARGET];
		TexturePtr mTextures[MAX_HW_RENDERTARGET];
	};

	typedef SmartPtr<RenderTarget> RenderTargetPtr;

}
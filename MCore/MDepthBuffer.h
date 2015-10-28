/*
*	DepthBuffer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTexture.h"

namespace Rad {

	class M_ENTRY DepthBuffer
	{
		DECLARE_REF();

	public:
		DepthBuffer();
		virtual ~DepthBuffer();

		virtual void
			Release() = 0;

		int
			GetWidth() { return mWidth; }
		int
			GetHeight() { return mHeight; }
		eSizeAlign
			GetAlign() { return mAlign; }
		ePixelFormat
			GetFormat() { return mFormat; }
		bool
			HasStencilBuffer() { return mFormat == ePixelFormat::D24S8; }

		virtual void
			Resize(int w, int h) = 0;

	protected:
		int mWidth;
		int mHeight;
		eSizeAlign mAlign;
		ePixelFormat mFormat;
	};

	typedef SmartPtr<DepthBuffer> DepthBufferPtr;

}
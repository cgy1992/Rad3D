#include "MRenderTarget.h"
#include "MHWBufferManager.h"

namespace Rad {

	RenderTarget::RenderTarget()
	{
		mWidth = mHeight = 0;

		for (int i = 0; i < MAX_HW_RENDERTARGET; ++i)
		{
			mFormats[i] = ePixelFormat::UNKNOWN;
		}
	}

	RenderTarget::~RenderTarget()
	{
	}

	bool RenderTarget::IsMRT()
	{
		return mTextures[1] != NULL || mTextures[2] != NULL || mTextures[3] != NULL;
	}
}
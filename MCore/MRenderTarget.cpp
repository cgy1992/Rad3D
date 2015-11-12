#include "MRenderTarget.h"
#include "MHWBufferManager.h"

namespace Rad {

	RenderTarget::RenderTarget()
	{
		mWidth = mHeight = 0;
		mFormat = ePixelFormat::UNKNOWN;
	}

	RenderTarget::~RenderTarget()
	{
	}

}
#include "MDepthBuffer.h"
#include "MHWBufferManager.h"

namespace Rad {

	DepthBuffer::DepthBuffer()
		: mWidth(0)
		, mHeight(0)
		, mFormat(ePixelFormat::UNKNOWN)
		, mAlign(eSizeAlign::NONE)
	{
	}

	DepthBuffer::~DepthBuffer()
	{
	}

}
/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDepthBuffer.h"

namespace Rad {

	class GLHWBufferManager;

	class NullDepthBuffer : public DepthBuffer
	{
		friend class NullHWBufferManager;

	public:
		NullDepthBuffer() {}
		virtual ~NullDepthBuffer() {}

		virtual void
			Release() { delete this; }

		virtual void
			Resize(int w, int h) {}
	};
	

}
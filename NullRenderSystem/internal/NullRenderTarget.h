/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderTarget.h"

namespace Rad {

	class NullHWBufferManager;

	class NullRenderTarget : RenderTarget
	{
		friend class NullHWBufferManager;

	public:
		NullRenderTarget() {}
		virtual ~NullRenderTarget() {}

		virtual void
			Release() { delete this; }

		virtual void
			Resize(int w, int h) {}
	};

}
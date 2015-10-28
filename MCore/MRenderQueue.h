/*
*	Render Queue
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"
#include "MCamera.h"
#include "MNode.h"

namespace Rad {

	class M_ENTRY RenderQueue
	{
		DECLARE_ALLOC();

	public:
		RenderQueue();
		~RenderQueue();

		void 
			Clear();

		void 
			AddRenderObject(RenderObject * able);

		Array<RenderObject *> & 
			GetSolidObjects() { return mSolidObjects; }
		Array<RenderObject *> & 
			GetAlphaObjects() { return mAlphaObjects; }

	protected:
		Array<RenderObject *> mSolidObjects;
		Array<RenderObject *> mAlphaObjects;
	};

}

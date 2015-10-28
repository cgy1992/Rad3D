#include "MRenderQueue.h"

namespace Rad {

	RenderQueue::RenderQueue()
	{
		mAlphaObjects.Alloc(256);
		mSolidObjects.Alloc(256);
	}

	RenderQueue::~RenderQueue()
	{
	}

	void RenderQueue::Clear()
	{
		mAlphaObjects.Clear();
		mSolidObjects.Clear();
	}

	void RenderQueue::AddRenderObject(RenderObject * able)
	{
		float opacity = able->_getOpacity();

		if (able->GetRenderOp()->primCount > 0 && opacity > 0)
		{
			if (opacity >= 1)
			{
				eBlendMode bm = able->GetMaterial()->blendMode;

				if (bm == eBlendMode::OPACITY || bm == eBlendMode::ALPHA_TEST)
					mSolidObjects.PushBack(able);
				else
					mAlphaObjects.PushBack(able);
			}
			else
			{
				mAlphaObjects.PushBack(able);
			}
		}
	}

}


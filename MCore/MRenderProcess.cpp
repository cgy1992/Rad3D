#include "MRenderProcess.h"

namespace Rad {

	ImplementRTTI(RenderProcess, IObject);

	RenderProcess::RenderProcess(bool post)
		: mOrder(post ? POST_PROCESS_MASK : 0)
		, mEnable(false)
	{
	}

	RenderProcess::~RenderProcess()
	{
	}

	void RenderProcess::SetEnable(bool enable)
	{
		if (mEnable != enable)
		{
			mEnable = enable;

			mEnable ? OnEnable() : OnDisable();
		}
	}

	void RenderProcess::SetOrder(int order)
	{
		d_assert (order > 0);

		mOrder = IsPost() ? POST_PROCESS_MASK + order : order;
	}
}

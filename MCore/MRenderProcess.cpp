#include "MRenderProcess.h"
#include "MRenderContext.h"

namespace Rad {

	ImplementRTTI(RenderProcess, IObject);

	RenderProcess::RenderProcess(RenderContext * context, int order)
		: mContext(context)
		, mOrder(order)
		, mEnable(false)
	{
		mContext->_addProcess(this);
	}

	RenderProcess::~RenderProcess()
	{
		mContext->_removeProcess(this);
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

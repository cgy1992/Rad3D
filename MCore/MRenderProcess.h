/*
*	RenderProcess
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderTarget.h"

namespace Rad {

#define POST_PROCESS_MASK 10000

	class M_ENTRY RenderProcess : public IObject
	{
		DECLARE_RTTI();

	public:
		RenderProcess(bool post);
		virtual ~RenderProcess();

		bool
			IsPost() { return mOrder >= POST_PROCESS_MASK; }

		void
			SetEnable(bool enable);
		bool
			IsEnable() { return mEnable; }

		void
			SetOrder(int order);
		int
			GetOrder() { return IsPost() ? mOrder - POST_PROCESS_MASK : mOrder; }

		virtual void
			OnEnable() = 0;
		virtual void
			OnDisable() = 0;
		virtual void
			DoProcess() = 0;

	protected:
		bool mEnable;
		int mOrder;
	};

}
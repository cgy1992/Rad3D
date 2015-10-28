/*
*	VertexBuffer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"

namespace Rad {

	class M_ENTRY VertexBuffer
	{
		DECLARE_REF();

	public:
		VertexBuffer() {}
		virtual ~VertexBuffer() {}

		virtual void
			Release() = 0;

		virtual void * 
			Lock(eLockFlag flag) = 0;
		virtual void 
			Unlock() = 0;

		int 
			GetStride() const { return mStride; }
		int 
			GetCount() const { return mCount; }
		eUsage 
			GetUsage() const { return mUsage; }

	protected:
		int mStride;
		int mCount;
		eUsage mUsage;
	};

	typedef SmartPtr<VertexBuffer> VertexBufferPtr;

}

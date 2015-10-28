/*
*	IndexBuffer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRef.h"
#include "MTypes.h"

namespace Rad {

	class M_ENTRY IndexBuffer
	{
		DECLARE_REF();

	public:
		IndexBuffer() {}
		virtual ~IndexBuffer() {}

		virtual void
			Release() = 0;

		virtual void * 
			Lock(eLockFlag flag) = 0;
		virtual void 
			Unlock() = 0;
		
		int 
			GetCount() const { return mCount; }
		int
			GetStride() const { return mStride; }
		eUsage 
			GetUsage() const { return mUsage; }

	protected:
		int mCount;
		int mStride;
		eUsage mUsage;
	};

	typedef SmartPtr<IndexBuffer> IndexBufferPtr;

}

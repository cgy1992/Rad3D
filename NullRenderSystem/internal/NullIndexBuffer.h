/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MIndexBuffer.h"

namespace Rad {

	class NullHWBufferManager;

	class NullIndexBuffer : public IndexBuffer
	{
		friend class NullHWBufferManager;

	public:
		NullIndexBuffer();
		virtual ~NullIndexBuffer();

		virtual void
			Release() { delete this; }

		virtual void * 
			Lock(eLockFlag flag);
		virtual void 
			Unlock();

	protected:
		byte * mIndexData;
		int mLockFlag;
	};

}

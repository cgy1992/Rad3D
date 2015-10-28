/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MVertexBuffer.h"

namespace Rad {

	class NullHWBufferManager;

	class NullVertexBuffer : public VertexBuffer
	{
		friend class NullHWBufferManager;

	public:
		NullVertexBuffer();
		virtual ~NullVertexBuffer();

		virtual void
			Release() { delete this; }

		virtual void * 
			Lock(eLockFlag flag);
		virtual void 
			Unlock();

	protected:
		byte * mVertexData;
		int mLockFlag;
	};

}

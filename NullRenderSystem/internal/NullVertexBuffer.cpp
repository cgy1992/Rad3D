#include "NullVertexBuffer.h"

namespace Rad {

	NullVertexBuffer::NullVertexBuffer()
		: mVertexData(NULL)
		, mLockFlag(0)
	{
	}

	NullVertexBuffer::~NullVertexBuffer()
	{
		safe_delete_array(mVertexData);
	}

	void * NullVertexBuffer::Lock(eLockFlag flag)
	{
		d_assert(mLockFlag == 0);

		mLockFlag = flag;

		if (mVertexData == NULL && (mLockFlag & eLockFlag::WRITE))
		{
			mVertexData = new byte[mStride * mCount];
		}

		return mVertexData;
	}

	void NullVertexBuffer::Unlock()
	{
		d_assert(mLockFlag != 0);

		if (mUsage == eUsage::STATIC || mUsage == eUsage::DYNAMIC)
			safe_delete_array(mVertexData);

		mLockFlag = 0;
	}

}
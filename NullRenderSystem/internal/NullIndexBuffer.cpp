#include "NullIndexBuffer.h"

namespace Rad {

	NullIndexBuffer::NullIndexBuffer()
		: mIndexData(NULL)
		, mLockFlag(0)
	{
	}

	NullIndexBuffer::~NullIndexBuffer()
	{
		safe_delete_array(mIndexData);
	}


	void * NullIndexBuffer::Lock(eLockFlag flag)
	{
		d_assert (mLockFlag == 0);

		mLockFlag = flag;

		if (mIndexData == NULL && (mLockFlag & eLockFlag::WRITE))
		{
			mIndexData = new byte[mCount * mStride];
		}

		return mIndexData;
	}

	void NullIndexBuffer::Unlock()
	{
		d_assert (mLockFlag != 0);

		if (mUsage == eUsage::STATIC || mUsage == eUsage::DYNAMIC)
		{
			safe_delete_array(mIndexData);
		}

		mLockFlag = 0;
	}
}
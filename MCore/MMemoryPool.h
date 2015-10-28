/*
*	MemoryPool
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDebug.h"
#include "MMemory.h"

namespace Rad {

	template <int size>
	class MemoryPoolAlloc
	{
	public:
		MemoryPoolAlloc()
			: mTotal(0)
			, mActive(0)
			, mFree(NULL)
			, mElements(NULL)
			, mMaxRequest(0)
		{
		}

		~MemoryPoolAlloc()
		{
			Shutdown();
		}

		void Init(int count)
		{
			d_assert (!mElements);

			mElements = new element[count];

			for (int i = 0; i < count; ++i)
			{
				mElements[i].next = mFree;
				mFree = &mElements[i];
			}

			mTotal = count;
		}

		void Shutdown()
		{
			d_assert(mActive == 0);

			safe_delete_array(mElements);

			mFree = NULL;
			mActive = 0;
			mTotal = 0;
		}

		void * Alloc()
		{
			if (!mFree)
			{
				return NULL;
			}
			else
			{
				++mActive;
				if (mActive > mMaxRequest)
					mMaxRequest = mActive;

				element * e = mFree;
				mFree = mFree->next;
				e->next = NULL;
				return &e->mem;
			}
		}

		void Free(void * mem)
		{
			element * e = (element*)((char*)mem -  sizeof(element*));

			d_assert(!e->next && e - mElements >= 0 && e - mElements < (int)mTotal);

			e->next = mFree;
			mFree = e;
			--mActive;
		}

		int GetTotalCount() const
		{
			return mTotal;
		}

		int GetActiveCount() const
		{
			return mActive;
		}

		int GetFreeCount() const
		{
			return mTotal - mActive;
		}

		int GetMaxRequest() const
		{
			return mMaxRequest;
		}

	protected:
		struct element
		{
			element *   next;
			char        mem[size];
		};

		int mTotal;
		int mActive;
		int mMaxRequest;
		element * mFree;
		element * mElements;
	};

	class MemoryPool
	{
		friend class Memory;

	public:
		MemoryPool();
		~MemoryPool();

		void 
			Init(int _16c,int _32c, int _64c, int _128c, int _256c);
		void 
			Shutdown();

		void * 
			Alloc(int size, short & flag);
		void 
			Free(void * mem, short flag);

	protected:
		MemoryPoolAlloc<16> mAlloc16;
		MemoryPoolAlloc<32> mAlloc32;
		MemoryPoolAlloc<64> mAlloc64;
		MemoryPoolAlloc<128> mAlloc128;
		MemoryPoolAlloc<256> mAlloc256;
	};

}

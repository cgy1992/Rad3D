#include "MMemoryPool.h"

namespace Rad {

	MemoryPool::MemoryPool()
	{
	}

	MemoryPool::~MemoryPool()
	{
		d_assert (
			mAlloc16.GetTotalCount() == 0 &&
			mAlloc32.GetTotalCount() == 0 &&
			mAlloc64.GetTotalCount() == 0 &&
			mAlloc128.GetTotalCount() == 0 &&
			mAlloc256.GetTotalCount() == 0);
	}

	void MemoryPool::Init(int _16c,int _32c, int _64c, int _128c, int _256c)
	{
		mAlloc16.Init(_16c);
		mAlloc32.Init(_32c);
		mAlloc64.Init(_64c);
		mAlloc128.Init(_128c);
		mAlloc256.Init(_256c);
	}

	void MemoryPool::Shutdown()
	{
		mAlloc16.Shutdown();
		mAlloc32.Shutdown();
		mAlloc64.Shutdown();
		mAlloc128.Shutdown();
		mAlloc256.Shutdown();
	}

	void * MemoryPool::Alloc(int size, short & flag)
	{
		void * mem = NULL;

		if (size <= 16 && !mem)
		{   
			mem = mAlloc16.Alloc();
			flag = MEMORY_IN_POOL16;
		}

		if (size <= 32 && !mem)
		{
			mem = mAlloc32.Alloc();
			flag = MEMORY_IN_POOL32;
		}

		if (size <= 64 && !mem)
		{
			mem = mAlloc64.Alloc();
			flag = MEMORY_IN_POOL64;
		}

		if (size <= 128 && !mem)
		{
			mem = mAlloc128.Alloc();
			flag = MEMORY_IN_POOL128;
		}

		if (size <= 256 && !mem)
		{
			mem = mAlloc256.Alloc();
			flag = MEMORY_IN_POOL256;
		}

		return mem;
	}

	void MemoryPool::Free(void * mem, short flag)
	{
		switch (flag)
		{
		case MEMORY_IN_POOL16:
			mAlloc16.Free(mem);
			break;

		case MEMORY_IN_POOL32:
			mAlloc32.Free(mem);
			break;

		case MEMORY_IN_POOL64:
			mAlloc64.Free(mem);
			break;

		case MEMORY_IN_POOL128:
			mAlloc128.Free(mem);
			break;

		case MEMORY_IN_POOL256:
			mAlloc256.Free(mem);
			break;

		default:
			d_assert(0 && "Bad Memory.");
			break;
		}
	}

}
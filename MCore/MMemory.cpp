#include "MMemory.h"
#include "MMemoryPool.h"
#include "MThread.h"
#include "MLog.h"
#include "MMap.h"

using namespace Rad;

/*  Memory Allocate Data Format
-----------------------------------------------------------------------------------
    
    | align data | block info | block head | head | used data | end | 

    align data: 0 - 32 bytes
    block info: 8 bytes, 4 bytes actual size and 4 bytes request size 
    block head: 4 bytes,  2 bytes point offset and 2 bytes pool flag
    used data:  actual used data.
    head:       16 bytes, must be zero
    end:        16 bytes, must be zero

-----------------------------------------------------------------------------------
*/

#define BLOCK_SIZE 8
#define BLOCK_DATA 0x80

struct MemoryBlockHead
{
	short PointOffset;
    short PoolFlag;
};

struct MemoryBlockData
{
    unsigned char data[BLOCK_SIZE];
};

struct MemoryBlockInfo
{
	int	ActualSize;
	int RequestSize;
    //char AllocFile[ALLOC_FILE_SIZE];
    //int AllocLine;
};

#ifdef M_DEBUG_MEMORY

#define MemoryBlockBegPointer(p)            (MemoryBlockData*)((char*)(p) - sizeof(MemoryBlockData))
#define MemoryBlockEndPointer(p, size)      (MemoryBlockData*)((char*)(p) + size)
#define MemoryBlockHeadPointer(p)	        (MemoryBlockHead*)((char*)(p) - sizeof(MemoryBlockData) - sizeof(MemoryBlockHead))
#define MemoryBlockInfoPointer(p)	        (MemoryBlockInfo*)((char*)(p) - sizeof(MemoryBlockData) - sizeof(MemoryBlockHead) - sizeof(MemoryBlockInfo))

#else

#define MemoryBlockHeadPointer(p)	        (MemoryBlockHead*)((char*)(p) - sizeof(MemoryBlockHead))

#endif

//
#ifdef M_DEBUG

struct __mm_alloc_info
{
	const char * filename;
	int line;

	__mm_alloc_info()
		: filename(NULL), line(0)
	{
	}

	__mm_alloc_info(const char * _filename, int _line) 
		: filename(_filename), line(_line)
	{
	}
};

Map<byte *, __mm_alloc_info> _mm_debuger;

#endif

//
#ifndef M_NO_MULTI_THREAD

Mutex _mm_mutex;

#define _mm_enter() MUTEX_ENTER(_mm_mutex)

#else

#define _mm_enter()

#endif

//
struct __static_memory
{
	byte * data;
	int size;
	int capacity;
};

__static_memory _static_memory_[M_STATIC_MEMORY_MAX_CHANEL];

//
MemoryPool * _mmpool;


//
void Memory::Init(int _16BytePoolCount,
                  int _32BytePoolCount,
                  int _64BytePoolCount,
                  int _128BytePoolCount,
                  int _256BytePoolCount)
{
	d_assert (_mmpool == NULL);

	_mmpool = new MemoryPool;
    _mmpool->Init(_16BytePoolCount,
                 _32BytePoolCount,
                 _64BytePoolCount,
                 _128BytePoolCount,
                 _256BytePoolCount);

	for (int i = 0; i < M_STATIC_MEMORY_MAX_CHANEL; ++i)
	{
		_static_memory_[i].data = NULL;
		_static_memory_[i].size = 0;
		_static_memory_[i].capacity = 0;
	}
}

void Memory::Shutdown()
{
	d_assert (_mmpool != NULL);
	d_assert (_mm_debuger.Size() == 0 && "Memory Leak!");

	for (int i = 0; i < M_STATIC_MEMORY_MAX_CHANEL; ++i)
	{
		if (_static_memory_[i].data)
		{
			delete[] (_static_memory_[i].data);
		}
	}

	_mmpool->Shutdown();
	delete _mmpool;
	_mmpool = NULL;
}

bool Memory::IsInit()
{
	return _mmpool != NULL;
}

void * Memory::QueryStaticMemory(int chanel, int size)
{
	d_assert (chanel < M_STATIC_MEMORY_MAX_CHANEL && size > 0);
	d_assert (_static_memory_[chanel].size == 0);

	_static_memory_[chanel].size = size;

	if (size > _static_memory_[chanel].capacity)
	{
		if (_static_memory_[chanel].data)
		{
			delete[] (_static_memory_[chanel].data);
		}

		_static_memory_[chanel].capacity = size;
		_static_memory_[chanel].data = new byte[size + 2];
		_static_memory_[chanel].data[0] = (byte)chanel;
	}

	// set end 0xFF, check memory overflow in free.
	byte * p = _static_memory_[chanel].data + 1;

	p[size] = 0xFF;

	return p;
}

void Memory::FreeStaticMemory(void * p)
{
	byte * data = (byte *)p - 1;
	int chanel = data[0];

	d_assert (chanel < M_STATIC_MEMORY_MAX_CHANEL);
	d_assert (_static_memory_[chanel].size > 0);

	byte * check = (byte *)p + _static_memory_[chanel].size;
	d_assert (*check == 0xFF && "Bad Memory");

	// more then 4M, free it
	if (_static_memory_[chanel].capacity > 4096 * 1024)
	{
		delete[] (_static_memory_[chanel].data);
		_static_memory_[chanel].data = NULL;
		_static_memory_[chanel].capacity = 0;
	}

	_static_memory_[chanel].size = 0;
}

#ifdef M_DEBUG
void * Memory::PoolAlloc(size_t iSize, int Align, const char * filename, int line)
#else
void * Memory::PoolAlloc(size_t iSize, int Align)
#endif
{
	d_assert (_mmpool);

	_mm_enter();

    int				    iActualSize = 0;
    short               iMemoryPool = MEMORY_IN_SYSTEM;
    char *				pActualMem = NULL;
    char *				pUsedMem = NULL;
    MemoryBlockHead *	pHead = NULL;
	MemoryBlockInfo *	pBlockInfo = NULL;
	MemoryBlockData *	pBlockBeg = NULL;
	MemoryBlockData *	pBlockEnd = NULL;

    d_assert(iSize > 0 && "Invalidate Parameters");

#ifdef M_DEBUG_MEMORY

    iActualSize = Align + sizeof(MemoryBlockHead) + sizeof(MemoryBlockInfo) + 
                  sizeof(MemoryBlockData) + iSize + sizeof(MemoryBlockData);

#else

    iActualSize = Align + sizeof(MemoryBlockHead) + iSize;

#endif

    pActualMem = (char*)_mmpool->Alloc(iActualSize, iMemoryPool);
    if (!pActualMem)
    {
        iMemoryPool = MEMORY_IN_SYSTEM;
        pActualMem = (char*)::malloc(iActualSize);
    }

    d_assert(pActualMem && "Allocating Error");

#ifdef M_DEBUG_MEMORY

    pUsedMem = pActualMem + Align + sizeof(MemoryBlockInfo) + sizeof(MemoryBlockHead) + sizeof(MemoryBlockData);

#else

    pUsedMem = pActualMem + Align + sizeof(MemoryBlockHead);

#endif

    if (Align)
        pUsedMem = (char*)((int)(pUsedMem) & ~(Align - 1));

    pHead = MemoryBlockHeadPointer(pUsedMem);
    pHead->PointOffset = static_cast<short>((char*)pHead - (char*)pActualMem);
    pHead->PoolFlag = iMemoryPool;

#ifdef M_DEBUG_MEMORY

    pBlockInfo = MemoryBlockInfoPointer(pUsedMem);
	pBlockBeg = MemoryBlockBegPointer(pUsedMem);
    pBlockEnd = MemoryBlockEndPointer(pUsedMem, iSize);

    memset(pBlockBeg, BLOCK_DATA, sizeof(MemoryBlockData));
    memset(pBlockEnd, BLOCK_DATA, sizeof(MemoryBlockData));

    pBlockInfo->ActualSize = iActualSize;
    pBlockInfo->RequestSize = iSize;

#endif

    d_assert(CheckMemory(pUsedMem) && "Bad Memory Allocated.");

#ifdef M_DEBUG
	_mm_debuger.Insert((byte *)pUsedMem, __mm_alloc_info(filename, line));
#endif

    return pUsedMem;
}

void Memory::PoolFree(void * pMem)
{
	d_assert (_mmpool);

    _mm_enter();

    d_assert(CheckMemory(pMem) && "Bad Memory Used.");

    char *				pActualMem;
    MemoryBlockHead *	pHead;

    pHead = MemoryBlockHeadPointer(pMem);
    pActualMem = (char*)pHead - pHead->PointOffset;

    if (pHead->PoolFlag != MEMORY_IN_SYSTEM)
        _mmpool->Free(pActualMem, pHead->PoolFlag);
    else
        ::free(pActualMem);

#ifdef M_DEBUG
	_mm_debuger.EraseEx((byte *)pMem);
#endif
}

bool Memory::CheckMemory(void * pMem)
{
    MemoryBlockHead * pHead = MemoryBlockHeadPointer(pMem);
	MemoryBlockInfo * pBlockInfo = NULL;
	MemoryBlockData * pBlockBeg = NULL;
	MemoryBlockData * pBlockEnd = NULL;

    // check head
    if (pHead->PointOffset < 0 || pHead->PointOffset >= 32 ||
        (pHead->PoolFlag != MEMORY_IN_SYSTEM && 
         pHead->PoolFlag != MEMORY_IN_POOL16 &&
         pHead->PoolFlag != MEMORY_IN_POOL32 &&
         pHead->PoolFlag != MEMORY_IN_POOL64 &&
         pHead->PoolFlag != MEMORY_IN_POOL128 &&
         pHead->PoolFlag != MEMORY_IN_POOL256))
        return false;

#ifdef M_DEBUG_MEMORY

    //chek beg data
	pBlockInfo = MemoryBlockInfoPointer(pMem);
    pBlockBeg = MemoryBlockBegPointer(pMem);
    pBlockEnd = MemoryBlockEndPointer(pMem, pBlockInfo->RequestSize);

    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        if (pBlockBeg->data[i] != BLOCK_DATA)
            return false;
    }

    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        if (pBlockEnd->data[i] != BLOCK_DATA)
            return false;
    }

#endif

   return true;
}

void Memory::OutputMemoryPoolStatus()
{
    d_log ("\n****** Memory Pool Status ******\n" \
                 "  16 bytes pool:            \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n" \
                 "                            \n" \
                 "  32 bytes pool:            \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n" \
                 "                            \n" \
                 "  64 bytes pool:            \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n" \
                 "                            \n" \
                 "  128 bytes pool:           \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n" \
                 "                            \n" \
                 "  256 bytes pool:           \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n\n",

                 _mmpool->mAlloc16.GetTotalCount(),
                 _mmpool->mAlloc16.GetMaxRequest(),
                 _mmpool->mAlloc16.GetActiveCount(),
                 _mmpool->mAlloc16.GetFreeCount(),

                 _mmpool->mAlloc32.GetTotalCount(),
                 _mmpool->mAlloc32.GetMaxRequest(),
                 _mmpool->mAlloc32.GetActiveCount(),
                 _mmpool->mAlloc32.GetFreeCount(),

                 _mmpool->mAlloc64.GetTotalCount(),
                 _mmpool->mAlloc64.GetMaxRequest(),
                 _mmpool->mAlloc64.GetActiveCount(),
                 _mmpool->mAlloc64.GetFreeCount(),

                 _mmpool->mAlloc128.GetTotalCount(),
                 _mmpool->mAlloc128.GetMaxRequest(),
                 _mmpool->mAlloc128.GetActiveCount(),
                 _mmpool->mAlloc128.GetFreeCount(),

                 _mmpool->mAlloc256.GetTotalCount(),
                 _mmpool->mAlloc256.GetMaxRequest(),
                 _mmpool->mAlloc256.GetActiveCount(),
                 _mmpool->mAlloc256.GetFreeCount());
}


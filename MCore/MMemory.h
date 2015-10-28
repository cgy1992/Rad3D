/*
*	Memory
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDebug.h"

namespace Rad {

#ifdef M_DEBUG
#define _pool_alloc(size, align) Memory::PoolAlloc(size, align, __FILE__, __LINE__)
#else
#define _pool_alloc(size, align) Memory::PoolAlloc(size, align)
#endif

#define _pool_free(p) Memory::PoolFree(p)

#define pool_alloc(size) _pool_alloc(size, MEMORY_ALIGN_0)
#define pool_free(p) _pool_free(p)

#define safe_pool_free(p) \
	{ if (p) { pool_free(p); (p) = 0;} }

#define safe_delete(p) \
	{ if (p) { delete (p); (p) = 0; } }
#define safe_delete_array(p) \
	{ if (p) { delete[] (p); (p) = 0; } }

#define M_STATIC_MEMORY_MAX_CHANEL 8
#define M_STATIC_MEMORY_I_CHANEL 7

class M_ENTRY Memory
{
public:
    static void
		Init(int _16BytePoolCount = 1000,
             int _32BytePoolCount = 1000,
             int _64BytePoolCount = 1000,
             int _128BytePoolCount = 1000,
             int _256BytePoolCount = 1000);
    static void 
		Shutdown();
	static bool
		IsInit();

	static void *
		QueryStaticMemory(int chanel, int size);
	static void
		FreeStaticMemory(void * p);

#ifdef M_DEBUG
    static void * 
		PoolAlloc(size_t size, int align, const char * filename, int line);
#else
	static void * 
		PoolAlloc(size_t size, int align);
#endif
    static void 
		PoolFree(void * p);

    static bool 
		CheckMemory(void * pMem);

    static void 
		OutputMemoryPoolStatus();
};

#define DECLARE_ALLOC()
//public: \
//    void * operator new(size_t size, int align, const char * file, int line) \
//    { \
//        return _alloc(size, align,  file, line); \
//    } \
//      \
//    void * operator new[](size_t size, int align, const char * file, int line) \
//    { \
//        return _alloc(size, align, file, line); \
//    } \
//      \
//    void * operator new(size_t size) \
//    { \
//        return _alloc(size, MEMORY_ALIGN_0, "unknown", 0); \
//    } \
//      \
//    void * operator new[](size_t size) \
//    { \
//        return _alloc(size, MEMORY_ALIGN_0, "unknown", 0); \
//    } \
//      \
//    void operator delete(void * p, const char * file, int line) \
//    { \
//        _free(p, file, line); \
//    } \
//      \
//    void operator delete[](void * p, const char * file, int line) \
//    { \
//        _free(p, file, line); \
//    } \
//      \
//    void operator delete(void * p) \
//    { \
//        _free(p, "unknown", 0); \
//    } \
//      \
//    void operator delete[](void * p) \
//    { \
//        _free(p, "unknown", 0); \
//    }
//

#define DECLARE_POOL_ALLOC() \
public: \
   void * operator new(size_t size, int align) \
    { \
        return _pool_alloc(size, align); \
    } \
      \
    void * operator new[](size_t size, int align) \
    { \
        return _pool_alloc(size, align); \
    } \
      \
    void * operator new(size_t size) \
    { \
        return _pool_alloc(size, MEMORY_ALIGN_0); \
    } \
      \
    void * operator new[](size_t size) \
    { \
        return _pool_alloc(size, MEMORY_ALIGN_0); \
    } \
      \
    void operator delete(void * p) \
    { \
        _pool_free(p); \
    } \
      \
    void operator delete[](void * p) \
    { \
        _pool_free(p); \
    }


	struct M_ENTRY static_memory
	{
		static_memory()
		{
			i_data = NULL;
			i_size = 0;
		}

		static_memory(int chanel, int size)
		{
			i_data = NULL;
			i_size = size;
			query(chanel, size);
		}

		~static_memory()
		{
			free();
		}

		void * data()
		{
			d_assert (i_data != NULL);
			return i_data;
		}

		int size()
		{
			return i_size;
		}

		void * query(int chanel, int size)
		{
			d_assert (i_data == NULL);

			i_data = Memory::QueryStaticMemory(chanel, size);
			i_size = size;
			
			return i_data;
		}

		void free()
		{
			if (i_data != NULL)
			{
				Memory::FreeStaticMemory(i_data);
			}

			i_data = NULL;
			i_size = 0;
		}

	private:
		// no copy
		static_memory(const static_memory & r) {} 
		static_memory & operator =(const static_memory & r) { return *this; }

	protected:
		void * i_data;
		int i_size;
	};

}



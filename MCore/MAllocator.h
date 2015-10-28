/*
*	Allocator
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

namespace Rad {

	template <class T, int size>
	class Allocator
	{
	public:
		Allocator()
			: mTotal(0)
			, mActive(0)
			, mFree(NULL)
			, mBlocks(NULL)
		{
		}

		~Allocator()
		{
			Clear();
		}

		void Clear()
		{
			while (mBlocks)
			{
				block * bl = mBlocks;
				mBlocks = mBlocks->next;
				delete bl;
			}

			mBlocks = NULL;
			mFree = NULL;
			mActive = 0;
			mTotal = 0;
		}

		T * Alloc()
		{
			if (!mFree)
			{
				block * bl = new block();
				bl->next = mBlocks;
				mBlocks = bl;

				for (int i = 0; i < size; ++i)
				{
					bl->elements[i].next = mFree;
					mFree = &bl->elements[i];
				}

				mTotal += size;
			}

			++mActive;
			element * e = mFree;
			mFree = mFree->next;
			e->next = NULL;
			return &e->mem;
		}

		void Free(T * mem)
		{
			element * e = (element*)(((char*)mem) -  sizeof(element*));
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

	protected:
		struct element
		{
			element *   next;
			T			mem;
		};

		struct block
		{
			block *     next;
			element     elements[size];
		};

		int         mTotal;
		int         mActive;
		element *   mFree;
		block *     mBlocks;
	};

	//
	template <class T>
	struct t_alloc
	{
		int i_capacity;

		t_alloc() : i_capacity(0) {}

		T * alloc(int size)
		{
			if (size < 16)
				size = 16;

			if (size <= i_capacity * 2)
			{
				i_capacity *= 2;
			}
			else
			{
				i_capacity = size;
			}

			return new T[i_capacity];
		}

		void free(T * m)
		{
			delete[] m;
		}

		int capacity() const
		{
			return i_capacity;
		}
	};

	//
	template <class T, int POOL_SIZE>
	struct t_alloc_static
	{
		T i_member[POOL_SIZE];

		t_alloc_static() {}

		T * alloc(int size)
		{
			d_assert(size <= POOL_SIZE);

			return i_member;
		}

		void free(T * m)
		{
		}

		int capacity() const
		{
			return POOL_SIZE;
		}
	};

	//
	template <class T>
	struct t_alloc_pool
	{
		int i_capacity;

		t_alloc_pool() : i_capacity(0) {}

		T * alloc(int size)
		{
			if (size < 16)
				size = 16;

			if (size <= i_capacity * 2)
			{
				i_capacity *= 2;
			}
			else
			{
				i_capacity = size;
			}

			return (T *)pool_alloc(sizeof(T) * i_capacity);
		}

		void free(T * p)
		{
			pool_free(p);
		}

		int capacity() const
		{
			return i_capacity;
		}
	};
}


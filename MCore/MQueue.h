/*
*	Queue
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAllocator.h"

namespace Rad {

	template <class T>
	class Queue
	{
	protected:
		struct _member
		{
			T value;
			_member * next;
		};

	public:
		Queue()
		{
			mFront = mBack = NULL;
		}

		~Queue()
		{
			Clear();
		}

		T & Front()
		{
			d_assert(Size() > 0);

			return mFront->value;
		}

		const T & Front() const
		{
			d_assert(Size() > 0);

			return mFront->value;
		}

		T & Back()
		{
			d_assert(Size() > 0);

			return mBack->value;
		}

		const T & Back() const
		{
			d_assert(Size() > 0);

			return mBack->value;
		}

		void Push(const T & Val)
		{
			if (Size() > 0)
			{
				_member * new_mem = mAllocator.Alloc();
				new_mem->value = Val;
				new_mem->next = NULL;
				mBack->next = new_mem;
				mBack = new_mem;
			}
			else
			{
				mFront = mAllocator.Alloc();
				mFront->value = Val;
				mFront->next = NULL;
				mBack = mFront;
			}
		}

		void Pop()
		{
			d_assert(Size() > 0);

			_member * mem = mFront;

			mFront = mem->next;

			if (!mFront)
				mBack = NULL;

			mAllocator.Free(mem);
		}

		int Size() const
		{
			return mAllocator.GetActiveCount();
		}

		bool Empty() const
		{
			return Size() == 0;
		}

		void Clear()
		{
			mAllocator.Clear();
			mFront = mBack = NULL;
		}

		Queue(const Queue & q)
		{
			mFront = mBack = NULL;

			*this = q;
		}

		Queue & operator =(const Queue & q)
		{
			Clear();

			_member * m = q.mFront;
			while (m != NULL)
			{
				Push(m->value);

				m = m->next;
			}

			return *this;
		}

	protected:
		Allocator<_member, 128> mAllocator;
		_member * mFront;
		_member * mBack;
	};

}

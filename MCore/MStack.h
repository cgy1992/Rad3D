/*
*	Stack
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAllocator.h"

namespace Rad {

	template <class T>
	class Stack
	{
		struct _member
		{
			T value;
			_member *  next;
		};

	public:
		Stack()
		{
			mTop = NULL;
		}

		~Stack()
		{
			Clear();
		}

		void Clear()
		{
			mAlloc.Clear();
			mTop = NULL;
		}

		bool Empty() const
		{
			return Size() == 0;
		}

		void Push(const T & val)
		{
			_member * mem = mAlloc.Alloc();

			mem->value = val;
			mem->next = mTop;
			mTop = mem;
		}

		void Pop()
		{
			d_assert(Size() > 0);

			_member * mem = mTop;
			mTop = mem->next;
			mAlloc.Free(mem);
		}

		int Size() const
		{
			return mAlloc.GetActiveCount();
		}

		T & Top()
		{
			d_assert(Size() > 0);

			return mTop->value;
		}

		const T & Top() const
		{
			d_assert(Size() > 0);

			return mTop->value;
		}

		Stack(const Stack & s)
		{
			mTop = NULL;

			*this = s;
		}

		Stack & operator =(const Stack & s)
		{
			Clear();

			_member * m = s.mTop;
			_member * back = NULL;

			if (m != NULL)
			{
				_member * mem = mAlloc.Alloc();
				mem->value = m->value;
				mem->next = NULL;

				back = mem;

				m = m->next;
			}

			mTop = back;

			while (m != NULL)
			{
				_member * mem = mAlloc.Alloc();
				mem->value = m->value;
				mem->next = NULL;

				back->next = mem;
				back = mem;

				m = m->next;
			}

			return *this;
		}

	protected:
		Allocator<_member, 128> mAlloc;
		_member * mTop;
	};

}

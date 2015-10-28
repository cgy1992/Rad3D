/*
*	Array
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAllocator.h"

namespace Rad {

	template <
		class T, 
		class T_ALLOC = t_alloc<T>
	>
	class Array
	{
	public:
		Array() : mSize(0), mMember(0)
		{
		}

		~Array()
		{
			if (mMember != NULL)
			{
				i_alloc.free(mMember);
			}
		}

		void Alloc(int size)
		{
			if (mMember == NULL || size > Capacity())
			{
				T * pTmp = i_alloc.alloc(size);

				for (int i = 0; i < mSize; ++i)
				{
					pTmp[i] = mMember[i];
				}

				if (mMember != NULL)
				{
					i_alloc.free(mMember);
				}

				mMember = pTmp;
			}
		}

		int Capacity() const
		{
			return i_alloc.capacity();
		}

		int Size() const
		{
			return mSize;
		}

		void Resize(int size)
		{
			Alloc(size);

			mSize = size;
		}

		bool Empty() const
		{
			return Size() == 0;
		}

		void Clear()
		{
			mSize = 0;
		}

		void Insert(int index, const T & val)
		{
			d_assert(index >= 0 && index <= mSize);

			Alloc(mSize + 1);

			int i = mSize;
			while (i > index)
			{
				mMember[i] = mMember[i - 1];
				--i;
			}

			mMember[index] = val;
			++mSize;
		}

		template <class OP>
		int InsertT(const T & val, OP & OP_)
		{
			int start = 0, end = mSize - 1;
			int mid = 0;

			while (start <= end)
			{
				mid = (start + end) / 2;

				int cmp = OP_(val, mMember[mid]);
				if (cmp < 0)
				{
					end = mid - 1;
				}
				else if (cmp > 0)
				{
					start = mid + 1;
				}
				else
				{
					start = mid;
					break;
				}
			}

			Insert(start, val);

			return start;
		}

		template <class OP>
		int FindT(OP & OP_)
		{
			int start = 0, end = mSize - 1;
			int mid = 0;

			while (start <= end)
			{
				mid = (start + end) / 2;

				int cmp = OP_(mMember[mid]);
				if (cmp < 0)
				{
					end = mid - 1;
				}
				else if (cmp > 0)
				{
					start = mid + 1;
				}
				else
				{
					start = mid;
					break;
				}
			}

			return start;
		}

		void Erase(int index, int count = 1)
		{
			d_assert(index >= 0 && count > 0 && index + count <= mSize);

			int num_copy = mSize - (index + count);
			int i = index;

			while (num_copy--)
			{
				mMember[i] = mMember[i + count];

				++i;
			}

			mSize -= count;
		}

		void PushFront(const T & v)
		{
			Insert(0, v);
		}

		void PopFront()
		{
			Erase(0);
		}

		void PushBack(const T & v)
		{
			Insert(mSize, v);
		}

		void PopBack()
		{
			d_assert(mSize > 0);
			--mSize;
		}

		T & Front()
		{
			return At(0);
		}

		const T & Front() const
		{
			return At(0);
		}

		T & Back()
		{
			return At(mSize - 1);
		}

		const T & Back() const
		{
			return At(mSize - 1);
		}

		T & At(int i)
		{
			d_assert (i >= 0 && i < Size());

			return mMember[i];
		}

		const T & At(int i) const
		{
			d_assert (i >= 0 && i < Size());

			return mMember[i];
		}

		const T * c_ptr(int i = 0) const
		{
			d_assert(i >= 0 && i < Size());

			return &mMember[i];
		}

		T * c_ptr(int i = 0)
		{
			d_assert(i >= 0 && i < Size());

			return &mMember[i];
		}

		T & operator [](int i)
		{
			return At(i);
		}

		const T & operator [](int i) const
		{
			return At(i);
		}

		Array(const Array & rk)
			: mSize(0)
			, mMember(NULL)
		{
			Alloc(rk.Size());

			mSize = rk.Size();
			for (int i = 0; i < mSize; i++)
			{
				mMember[i] = rk.mMember[i];
			}
		}

		Array & operator =(const Array & rk)
		{
			Clear();
			Alloc(rk.Size());

			mSize = rk.Size();
			for (int i = 0; i < rk.Size(); i++)
			{
				mMember[i] = rk.mMember[i];
			}

			return *this;
		}

	protected:
		int mSize;
		T * mMember;

		T_ALLOC i_alloc;
	};

	template <class T, int POOL_SIZE>
	class FixedArray : public Array<T, t_alloc_static<T, POOL_SIZE> >
	{
	};

}

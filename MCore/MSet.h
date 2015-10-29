/*
*	Set
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAllocator.h"
#include "MFunctional.h"

namespace Rad {

	template <
		class T,
		class T_CMP = t_compare<T>,
		class T_ALLOC = t_alloc<T>
	>
	class Set
	{
		DECLARE_ALLOC();

	public:
		Set() : mSize(0), mMember(0)
		{
		}

		~Set()
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

		bool Empty() const
		{
			return Size() == 0;
		}

		void Clear()
		{
			mSize = 0;
		}

		void Destroy()
		{
			Clear();

			if (mMember != NULL)
			{
				i_alloc.free(mMember);
				i_alloc.clear();
			}
		}

		int Insert(const T & val)
		{
			T_CMP cmp;
			int start = 0, end = mSize - 1;

			while (start <= end)
			{
				int mid = (start + end) / 2;

				int p = cmp(val, mMember[mid]);
				if (p < 0)
				{
					end = mid - 1;
				}
				else if (p > 0)
				{
					start = mid + 1;
				}
				else
				{
					start = mid;
					return start;
				}
			}

			Alloc(mSize + 1);

			int index = start;
			int i = mSize;

			while (i > index)
			{
				mMember[i] = mMember[i - 1];
				--i;
			}

			mMember[index] = val;
			mSize += 1;

			return start;
		}

		int Find(const T & val)
		{
			T_CMP cmp;
			int start = 0, end = mSize - 1;

			while (start <= end)
			{
				int mid = (start + end) / 2;

				int p = cmp(val, mMember[mid]);
				if (p < 0)
				{
					end = mid - 1;
				}
				else if (p > 0)
				{
					start = mid + 1;
				}
				else
				{
					return mid;
				}
			}

			return -1;
		}

		void Erase(int index)
		{
			d_assert(index >= 0 && index < mSize);

			Erase(index, 1);
		}

		void Erase(int index, int count)
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

		T & Front()
		{
			d_assert (mSize > 0);

			return mMember[0];
		}

		const T & Front() const
		{
			d_assert (mSize > 0);

			return mMember[0];
		}

		T & Back()
		{
			d_assert (mSize > 0);

			return mMember[mSize - 1];
		}

		const T & Back() const
		{
			d_assert (mSize > 0);

			return mMember[mSize - 1];
		}

		T & operator [](int index)
		{
			d_assert(index >= 0 && index < mSize);
			return mMember[index];
		}

		const T & operator [](int index) const
		{
			d_assert(index >= 0 && index < mSize);
			return mMember[index];
		}

		Set(const Set & rk)
			: mSize(0)
			, mMember(NULL)
		{
			Alloc(rk.Size());

			mSize = rk.mSize;
			for (int i = 0; i < mSize; i++)
			{
				mMember[i] = rk.mMember[i];
			}
		}

		Set & operator =(const Set & rk)
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

	template <class T, int POOL_SIZE, class T_CMP = t_compare<T> >
	class FixedSet : public Set<T, T_CMP, t_alloc_static<T, POOL_SIZE> >
	{
	};

}
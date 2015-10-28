/*
*	Map
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAllocator.h"
#include "MFunctional.h"

namespace Rad {

	template <class KEY, class T>
	struct T_KEY_VALUE
	{
		KEY key;
		T value;

		T_KEY_VALUE() {}
		T_KEY_VALUE(KEY _k, T _v) { key = _k; value = _v; }
	};

	template <
		class KEY, class T,
		class T_CMP = t_compare<KEY>,
		class T_ALLOC = t_alloc< T_KEY_VALUE<KEY, T> >
	>
	class Map
	{
	public:
		typedef T_KEY_VALUE<KEY, T> KEY_VALUE;

	public:
		Map() : mSize(0), mMember(0)
		{
		}

		~Map()
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
				KEY_VALUE * pTmp = i_alloc.alloc(size);

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

		int Insert(const KEY & key, const T & val)
		{
			T_CMP cmp;
			int start = 0, end = mSize - 1;

			while (start <= end)
			{
				int mid = (start + end) / 2;

				int p = cmp(key, mMember[mid].key);
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
					mMember[mid].value = val;
					return mid;
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

			mMember[index].key = key;
			mMember[index].value = val;
			mSize += 1;

			return start;
		}

		int Find(const KEY & key) const
		{
			T_CMP cmp;
			int start = 0, end = mSize - 1;

			while (start <= end)
			{
				int mid = (start + end) / 2;

				int p = cmp(key, mMember[mid].key);
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

		void EraseEx(const KEY & key)
		{
			int index = Find(key);
			if (index != -1)
			{
				Erase(index);
			}
		}

		KEY_VALUE & Front()
		{
			return (*this)[0];
		}

		const KEY_VALUE & Front() const
		{
			return (*this)[0];
		}

		KEY_VALUE & Back()
		{
			return (*this)[mSize - 1];
		}

		const KEY_VALUE & Back() const
		{
			return (*this)[mSize - 1];
		}

		KEY_VALUE & operator [](int i)
		{
			d_assert(i >= 0 && i < mSize);

			return mMember[i];
		}

		const KEY_VALUE & operator [](int i) const
		{
			d_assert(i >= 0 && i < mSize);

			return mMember[i];
		}

		Map(const Map & rk)
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

		Map & operator =(const Map & rk)
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
		KEY_VALUE * mMember;

		T_ALLOC i_alloc;
	};

	template <class T, int POOL_SIZE, class T_CMP = t_compare<T> >
	class FixedMap : public Map<T, T_CMP, t_alloc_static<T, POOL_SIZE> >
	{
	};

}

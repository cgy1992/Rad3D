/*
*	Field
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDebug.h"

namespace Rad {

	template <class T, int _SIZE>
	class Field
	{
	public:
		Field()
		{
		}

		~Field()
		{
		}

		int Size() const
		{
			return _SIZE;
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
			return At(Size() - 1);
		}

		const T & Back() const
		{
			return At(Size() - 1);
		}

		T & operator [](int i)
		{
			return At(i);
		}

		const T & operator [](int i) const
		{
			return At(i);
		}

		Field(const Field & rk)
		{
			for (int i = 0; i < Size(); ++i)
			{
				mMember[i] = rk.mMember[i];
			}
		}

		Field & operator =(const Field & rk)
		{
			for (int i = 0; i < Size(); ++i)
			{
				mMember[i] = rk.mMember[i];
			}

			return *this;
		}

	protected:
		T mMember[_SIZE];
	};

}
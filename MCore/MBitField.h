/*
*	BitField
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

namespace Rad {

	template<int BIT_COUNT>
	class BitField
	{
		DECLARE_ALLOC();

	public:
		BitField()
		{
			ClearAll();
		}

		BitField(const BitField & rk)
		{
			*this = rk;
		}

		~BitField()
		{
		}

		BitField & operator =(const BitField & rk)
		{ 
			for (int i = 0; i < IntCount(); ++i)
			{
				mBits[i] = rk.mBits[i];
			}

			return *this;
		}

		bool operator[](int bit) const
		{
			return Get(bit);
		}

		void ClearAll()
		{
			for (int i = 0; i < IntCount(); ++i)
			{
				mBits[i] = 0;
			}
		}

		bool Get(int bit) const
		{
			int i = bit / 32;
			int b = bit & (32 - 1);

			return (mBits[i] & (1 << b)) != 0;
		}

		void Set(int bit, bool val)
		{
			val ? Set(bit) : Clear(bit);
		}

		void Set(int bit)
		{
			d_assert (bit < BitCount());

			int i = bit / 32;
			int b = bit & (32 - 1);

			mBits[i] |= 1 << b;
		}

		void Clear(int bit)
		{
			d_assert (bit < BitCount());

			int i = bit / 32;
			int b = bit & (32 - 1);

			mBits[i] &= ~(1 << b);
		}

		int BitCount() const
		{
			return BIT_COUNT;
		}

		int IntCount() const
		{
			return (BIT_COUNT + 31) / 32;
		}

		int * Data() const
		{
			return mBits;
		}

	protected:
		int mBits[(BIT_COUNT + 31) / 32];
	};

}


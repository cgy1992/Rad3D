/*
*	RingBuffer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

namespace Rad {

	template <
		class T,
		class T_ALLOC = t_alloc<T>
	>
	class RingBuffer
	{
	public:
		RingBuffer()
			: mSize(0)
			, mMember(NULL)
		{
			i_first = i_last = 0;
			i_capacity = 0;
		}

		~RingBuffer()
		{
			if (mMember != NULL)
				i_alloc.free(mMember);
		}

		void Alloc(int size)
		{
			if (mMember == NULL || size > Capacity())
			{
				int newSize = Capacity();
				if (newSize > 0)
				{
					while (newSize < size)
					{
						newSize *= 2;
					}
				}
				else
				{
					newSize = 16;
				}

				T * temp = i_alloc.alloc(newSize);

				if (mSize > 0)
					Copy(temp, mSize);

				if (mMember != NULL)
					i_alloc.free(mMember);

				mMember = temp;
				i_first = 0;
				i_last = mSize;
				i_capacity = i_alloc.capacity();
			}
		}

		int Capacity() const
		{
			return i_capacity;
		}

		int Size() const
		{
			return mSize;
		}

		void Clear()
		{
			mSize = 0;
			i_first = i_last = 0;
		}

		void Destroy()
		{
			Clear()

			if (mMember != NULL)
			{
				i_alloc.free(mMember);
				i_alloc.clear();
			}

			i_capacity = 0;
		}

		T & At(int i)
		{
			d_assert (i < Size());

			return mMember[(i + i_first) & (Capacity() - 1)];
		}

		const T & At(int i) const
		{
			d_assert (i < Size());

			return mMember[(i + i_first) & (Capacity() - 1)];
		}

		int Copy(T * dest, int size)
		{
			int first = i_first;
			int length = mSize;

			int i = 0;
			while (i < size && length > 0)
			{
				dest[i++] = mMember[first++];

				--length;

				first &= Capacity() - 1;
			}

			return i;
		}

		int Read(T * dest, int size)
		{
			int i = Copy(dest, size);

			mSize -= i;
			i_first += i;
			i_first &= Capacity() - 1;

			return i;
		}

		int Write(T * dest, int size)
		{
			Alloc(mSize + size);

			int i = 0;
			while (i < size)
			{
				mMember[i_last++] = dest[i++];


				i_last &= Capacity() - 1;
			}

			mSize += i;

			return i;
		}

		RingBuffer(const RingBuffer & rk)
			: mSize(0)
			, mMember(NULL)
			, i_first(0)
			, i_last(0)
			, i_capacity(0)
		{
			*this = rk;
		}

		RingBuffer & operator =(const RingBuffer & rk)
		{
			Clear();
			Alloc(rk.Size());

			if (rk.Size() > 0)
				Copy(mMember, rk.Size());

			mSize = rk.Size();
			i_first = 0;
			i_last = mSize;

			return *this;
		}

	protected:
		T * mMember;
		int mSize;

		int i_first;
		int i_last;
		int i_capacity;

		T_ALLOC i_alloc;
	};

	template <class T, int POOL_SIZE>
	class FixedRingBuffer : public RingBuffer<T, t_alloc_static<T, POOL_SIZE> >
	{
	};

}
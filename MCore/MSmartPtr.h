/*
*	SmartPtr
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDebug.h"

namespace Rad {

	template <class T>
	class SmartPtr
	{
	public:
		SmartPtr() : ptr(NULL)
		{
		}

		SmartPtr(T * p) : ptr(NULL)
		{
			*this = p;
		}

		SmartPtr(const SmartPtr & p) : ptr(NULL)
		{
			*this = p;
		}

		~SmartPtr()
		{
			Release();
		}

		void Release()
		{
			if (ptr && ptr->i_reference._refdec() == 0)
			{
				ptr->Release();
				ptr = NULL;
			}
		}

		T *  c_ptr() const
		{
			return ptr;
		}

		SmartPtr & operator =(T * p)
		{
			Release();

			ptr = p;
			if (ptr)
				ptr->i_reference._refinc();

			return *this;
		}

		SmartPtr & operator =(const SmartPtr & p)
		{
			Release();

			ptr = p.ptr;
			if (ptr)
				ptr->i_reference._refinc();

			return *this;
		}

		bool operator ==(const SmartPtr & p) const
		{
			return ptr == p.ptr;
		}

		bool operator !=(const SmartPtr & p) const
		{
			return ptr != p.ptr;
		}

		bool operator ==(const T * p) const
		{
			return ptr == p;
		}

		bool operator !=(const T * p) const
		{
			return ptr != p;
		}

		T * operator ->() const
		{
			return ptr;
		}

	protected:
		T *    ptr;
	};

}

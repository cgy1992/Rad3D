/*
*	Reference Counter
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

namespace Rad {

	class M_ENTRY RefCounter
	{
	public:
		RefCounter() : i_ref_count(0)
		{
		}

		~RefCounter()
		{
			d_assert(i_ref_count == 0);
		}

		int 
			_refcount() { return i_ref_count; } 

		int 
			_refinc() { return ++i_ref_count; }

		int 
			_refdec() { d_assert(i_ref_count > 0); return --i_ref_count; }

	protected:
		int i_ref_count;
	};

	//
	class M_ENTRY RefCounterMT
	{
	public:
		RefCounterMT() : i_ref_count(0)
		{
		}

		~RefCounterMT()
		{
			d_assert(i_ref_count == 0);
		}

		int 
			_refcount() const { return i_ref_count; }
		int 
			_refinc();
		int 
			_refdec();

	protected:
		int i_ref_count;
	};

#define DECLARE_REF() \
public: \
	RefCounter i_reference;

#define DECLARE_REF_MT() \
public: \
	RefCounterMT i_reference;
}


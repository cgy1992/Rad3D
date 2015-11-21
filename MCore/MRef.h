/*
*	Reference
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

namespace Rad {

	class M_ENTRY Ref
	{
	public:
		Ref() : i_ref_count(0)
		{
		}

		~Ref()
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
	class M_ENTRY RefMT
	{
	public:
		RefMT() : i_ref_count(0)
		{
		}

		~RefMT()
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
	Ref i_reference;

#define DECLARE_REF_MT() \
public: \
	RefMT i_reference;
}


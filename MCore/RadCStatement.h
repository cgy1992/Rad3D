/*
*	RadC Statement
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "RadC.h"

namespace Rad {
	
	struct radc_exp;

	struct radc_stat
	{
		DECLARE_POOL_ALLOC();
		DECLARE_RTTI_ROOT(radc_stat);

		int line;
		int jump;
		const char * str;
		radc_exp * exp_tree;

		radc_stat();
		virtual ~radc_stat();

		virtual void
			build() = 0;

		virtual int
			execute() = 0;
	};

	struct rstat_empty : public radc_stat
	{
		DECLARE_RTTI();

		rstat_empty() {}
		virtual ~rstat_empty() {}

		virtual void
			build() {}

		virtual int  
			execute() { return jump; }
	};

	struct rstat_entry : public radc_stat
	{
		DECLARE_RTTI();

		FixedArray<radc_var *, 8> args;

		rstat_entry() {}
		virtual ~rstat_entry() {}

		virtual void
			build();

		virtual int 
			execute();
	};

	struct rstat_exp : public radc_stat
	{
		DECLARE_RTTI();

		rstat_exp() {}
		virtual ~rstat_exp() {}

		virtual void
			build();

		virtual int  
			execute();
	};

	struct rstat_if : public rstat_exp
	{
		DECLARE_RTTI();

		rstat_if() {}
		virtual ~rstat_if() {}

		virtual int  
			execute();
	};

	struct rstat_elseif : public rstat_if
	{
		DECLARE_RTTI();

		rstat_elseif() {}
		virtual ~rstat_elseif() {}
	};

	struct rstat_else : public rstat_empty
	{
		DECLARE_RTTI();

		rstat_else() {}
		virtual ~rstat_else() {}
	};

	struct rstat_end : public rstat_empty
	{
		DECLARE_RTTI();

		rstat_end() {}
		virtual ~rstat_end() {}
	};

	struct rstat_return : public rstat_exp
	{
		DECLARE_RTTI();

		rstat_return() {}
		virtual ~rstat_return() {}

		virtual int 
			execute();
	};

	struct rstat_while : public rstat_if
	{
		DECLARE_RTTI();

		rstat_while() {}
		virtual ~rstat_while() {}
	};

}
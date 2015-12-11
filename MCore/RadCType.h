/*
*	RadC Type
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRoot.h"
#include "MArray.h"

namespace Rad {

#define RADC_OPATION_CACHE 1
#define RADC_OPATION_COMPILE_ONLY 2

	struct M_ENTRY radc_enum
	{
		enum enum_t {
			none,

			// types
			type_void,
			type_real,
			type_string,
			type_ptr,

			// operator
			op_neg = 1,

			op_less,		// <
			op_greater,		// >
			op_less_equal,	// <=
			op_greater_equal,// >=
			op_not_equal,	// !=
			op_equal_equal, // ==
			op_and_and,		// &&
			op_or_or,		// ||

			op_equal,		// =
			op_add_equal,	// +=
			op_sub_equal,	// -=
			op_mul_equal,	// *=
			op_div_equal,	// /=
			//op_mod_equal,	// %=

			op_add,			// +
			op_sub,			// -
			op_mul,			// *
			op_div,			// /
			//op_mod,			// %

			op_left_b,		// (
			op_right_b,		// )
			op_comma,		// ,
		};

		M_ENUMERATION(radc_enum);
	};

	//
	typedef double radc_real;
	
	//
	struct M_ENTRY radc_var
	{
		DECLARE_POOL_ALLOC();

	public:
		radc_var(const char * _name = NULL);
		radc_var(const radc_var & v);
		~radc_var();

		radc_enum 
			getType() const { return (radc_enum::enum_t)i_type; }
		const char * 
			getName() const { return i_name; }

		void
			setConst() { i_flag |= 0x01; }
		bool
			isConst() { return i_flag & 0x01; }

		void
			clear();

		void
			setBool(bool v) { setReal(v ? 1 : 0); }
		void
			setInt(int v) { setReal((radc_real)v); }
		void 
			setReal(radc_real v);
		void 
			setString(const char * v, bool fmt = false);
		void 
			setPtr(void * v);

		radc_real 
			getReal() const { d_assert(i_type == radc_enum::type_real); return r_value; }
		const char * 
			getString() const { d_assert(i_type == radc_enum::type_string); return s_value; }
		void * 
			getPtr() const { d_assert(i_type == radc_enum::type_ptr); return p_value; }

		radc_var
			up_type(int r_type);
		radc_var
			down_type(int r_type);

		radc_var
			operator <(const radc_var & rk) const;
		radc_var 
			operator <=(const radc_var & rk) const;
		radc_var
			operator >(const radc_var & rk) const;
		radc_var 
			operator >=(const radc_var & rk) const;
		radc_var
			operator ==(const radc_var & rk) const;
		radc_var
			operator !=(const radc_var & rk) const;
		radc_var
			operator ||(const radc_var & rk) const;
		radc_var
			operator &&(const radc_var & rk) const;

		radc_var &
			operator =(const radc_var & rk);
		radc_var &
			operator +=(const radc_var & rk);
		radc_var &
			operator -=(const radc_var & rk);
		radc_var &
			operator *=(const radc_var & rk);
		radc_var &
			operator /=(const radc_var & rk);

		radc_var
			operator +(const radc_var & rk);
		radc_var
			operator -(const radc_var & rk);
		radc_var
			operator *(const radc_var & rk);
		radc_var
			operator /(const radc_var & rk);

	protected:
		void
			fmtString();

	protected:
		int i_flag;
		radc_enum i_type;
		const char * i_name;

		union {
			radc_real r_value;
			char * s_value;
			void * p_value;
		};
	};

	//
	struct M_ENTRY radc_varlist
	{
	public:
		radc_varlist();
		~radc_varlist();

		void
			clear();
		int
			size() const;

		void 
			push(radc_real v);
		void 
			push(const char * v);
		void 
			push(void * v);
		void
			push(const radc_var & v);

		bool
			getBool(int index) const { return getReal(index) == 1; }
		int
			getInt(int index) const { return (int)getReal(index); }
		radc_real
			getReal(int index) const;
		const char *
			getString(int index) const;
		void *
			getPtr(int index) const;

		int
			getType(int index) const;

		const radc_var &
			at(int index) const { return varlist[index]; }

	protected:
		FixedArray<radc_var, 8> varlist;
	};

	//
	typedef bool (*radc_function)(radc_var * result, const radc_varlist * args);

	struct M_ENTRY radc_reg
	{
		int _lib;
		const char * name;
		radc_function pfn;
		const char * desc;
	};

#define RADC_REG2(name, pfn) { -1, name, pfn, NULL }

#ifndef RADC_NO_INFO
#define RADC_REG(name, pfn, desc) { -1, name, pfn, desc }
#else
#define RADC_REG(name, pfn, desc) RADC_REG2(name, pfn)
#endif

	typedef void (*radc_error_reporter)(const char * err_str);

}
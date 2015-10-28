/*
*	RadDB SQL
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "RadDB.h"
#include "RadDBTable.h"
#include "MToken.h"

namespace Rad {

	struct rsql_exp
	{
		DECLARE_POOL_ALLOC();
		DECLARE_RTTI_ROOT(rsql_exp);

		rsql_exp * _first_child;
		rsql_exp * _last_child;
		rsql_exp * _next_sibling;

		Array<rsql_exp *, t_alloc_pool<rsql_exp *> > output;

		rsql_exp();
		virtual ~rsql_exp();

		void
			append(rsql_exp * child);

		void
			parse(Token::iterator i, Token::iterator end);

		virtual void
			build();
		virtual void 
			execute(Array<rdb_value> & results);
	};

	// 
	struct rsql_constant : public rsql_exp
	{
		DECLARE_RTTI();

		rdb_value _var;

		rsql_constant() {}
		virtual ~rsql_constant() {}

		static bool 
			match(Token::iterator i, Token::iterator end, Token::iterator & r, rsql_exp * parent);

		virtual void
			build() {}
		virtual void 
			execute(Array<rdb_value> & result);
	};

	//
	struct rsql_property : public rsql_exp
	{
		DECLARE_RTTI();

		const Property * _prop;

		rsql_property() {}
		virtual ~rsql_property() {}

		static bool 
			match(Token::iterator i, Token::iterator end, Token::iterator & r, rsql_exp * parent);

		virtual void
			build() {}
		virtual void 
			execute(Array<rdb_value> & result);

		static void
			get_value(rdb_value & v, const Property * p, const IObject * obj);
		static int
			compare(const Property * p, const IObject * obj1, const IObject * obj2);
		static int
			compare(const IObject * obj, const Property * p, const rdb_value & v);
	};

	//
	struct rsql_invoke : public rsql_exp
	{
		DECLARE_RTTI();

		rdb_function _pfn;
		FixedArray<rsql_exp *, RSQL_MAX_ARGS> _args;

		rsql_invoke() : _pfn(NULL) {}
		virtual ~rsql_invoke();

		static bool 
			match(Token::iterator i, Token::iterator end, Token::iterator & r, rsql_exp * parent);

		void
			parseArg(Token::iterator i, Token::iterator end);

		virtual void
			build();
		virtual void 
			execute(Array<rdb_value> & result);
	};

	//
	struct rsql_operator : public rsql_exp
	{
		DECLARE_RTTI();

		enum enum_t {
			op_none, 

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

			op_add,			// +
			op_sub,			// -
			op_mul,			// *
			op_div,			// /
			op_and,			// &
			op_or,			// |

			op_left_b,		// (
			op_right_b,		// )

			op_comma,		// ,
		};

		enum_t _op;
		int _priority;

		rsql_operator() : _op(op_none), _priority(0) {}
		virtual ~rsql_operator() {}

		static bool 
			match(Token::iterator i, Token::iterator end, Token::iterator & r, rsql_exp * parent);
		static int
			get_priority(int _op);

		virtual void
			build() {}
		virtual void 
			execute(Array<rdb_value> & result);

	protected:
		rdb_value
			_do_compare(rdb_value & p1, rdb_value & p2);
		rdb_value
			_do_arithmetic(rdb_value & p1, rdb_value & p2);
		rdb_value
			_do_assignment(rdb_value & p1, rdb_value & p2);
	};

	//
	struct rsql_engine : public Singleton<rsql_engine>
	{
		enum {
			RSQL_UNKNOWN,

			RSQL_SELECT,
			RSQL_INSERT,
			RSQL_DELETE,
			RSQL_UPDATE,

			RSQL_CREATE_USER,
			RSQL_DELETE_USER,
			RSQL_ALTER_USER,

			RSQL_CREATE_TABLE,
			RSQL_DELETE_TABLE,
			RSQL_OPTIMIZE_TABLE,

			RSQL_QUERY_MAX,
		};

		char i_error[128];
		Token * i_token;

		Array<char> i_buffer;
		int i_m;
		int i_op;
		rdb_table * i_table;
		char * i_args;
		char * i_where;
		char * i_option;
		rsql_exp * i_exp;

		int i_first, i_last;
		IObject * i_record;

		int option_count;
		int option_offset;
		rdb_value option_vmin[4], option_vmax[4];
		bool option_indexed[4];
		FixedArray<const Property *, 4> option_orderby;

		Map<int, rdb_function> i_functions;

		rsql_engine();
		~rsql_engine();

		void
			set_error(const char * fmt, ...);
		const char *
			get_error();
		bool
			is_error();
		
		void
			reg_function(const char * name, rdb_function pfn);
		rdb_function
			get_function(const char * name);

		void
			begin();
		void
			end();

		int
			compile(const char * str, int m);
		int
			execute(IObject * obj, rdb_collection * collection);

	protected:
		void
			r_build_index();

		int
			r_select(rdb_collection * collection);
		int
			r_insert(IObject * obj);
		int
			r_delete();
		int
			r_update(IObject * obj);

		int
			r_createuser();
		int
			r_deleteuser();
		int
			r_alteruser();

		int
			r_createtable();
		int
			r_deletetable();
		int
			r_optimizetable();
	};

#define rsql_get_compiler() rsql_engine & compiler = *rsql_engine::Instance();

}
/*
*	RadC Expression
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "RadC.h"
#include "RadCType.h"

namespace Rad {

	struct radc_stat;

	struct radc_exp
	{
		DECLARE_POOL_ALLOC();
		DECLARE_RTTI_ROOT(radc_exp);

		radc_exp * _first_child;
		radc_exp * _last_child;
		radc_exp * _next_sibling;

		Array<radc_exp *, t_alloc_pool<radc_exp *> > _output;

		radc_exp();
		virtual ~radc_exp();

		void
			append(radc_exp * child);

		void
			parse(Token::iterator i, Token::iterator end, radc_stat * stat);

		virtual void
			build(radc_stat * stat);
		virtual void 
			execute(Array<radc_var> & result, radc_stat * stat);
	};

	// 
	struct rexp_constant : public radc_exp
	{
		DECLARE_RTTI();

		radc_var _var;

		rexp_constant() {}
		virtual ~rexp_constant() {}

		static bool 
			match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat);

		virtual void
			build(radc_stat * stat) {}
		virtual void 
			execute(Array<radc_var> & results, radc_stat * stat);
	};

	// 
	struct rexp_variable : public radc_exp
	{
		DECLARE_RTTI();

		radc_var * _var;
		radc_enum _op;
		radc_exp * _exp;
		bool _enable_const;

		rexp_variable() : _var(NULL), _exp(NULL), _enable_const(false) {}
		virtual ~rexp_variable() { safe_delete(_exp); }

		static rexp_variable * 
			match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat);

		virtual void
			build(radc_stat * stat);
		virtual void 
			execute(Array<radc_var> & results, radc_stat * stat);
	};

	//
	struct rexp_vardef : public radc_exp
	{
		DECLARE_RTTI();

		rexp_vardef() {}
		virtual ~rexp_vardef() {}

		static bool 
			match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat);

		virtual void
			build(radc_stat * stat) {}
		virtual void 
			execute(Array<radc_var> & results, radc_stat * stat) {}
	};

	// 
	struct rexp_operator : public radc_exp
	{
		DECLARE_RTTI();

		radc_enum _op;
		int _priority;

		rexp_operator() : _op(radc_enum::none), _priority(0) {}
		virtual ~rexp_operator() {}

		static bool 
			match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat);
		static int
			get_priority(radc_enum _op);

		virtual void
			build(radc_stat * stat) {}
		virtual void 
			execute(Array<radc_var> & results, radc_stat * stat);

	protected:
		radc_var
			_do_compare(radc_var & p1, radc_var & p2, radc_stat * stat);
		radc_var
			_do_arithmetic(radc_var & p1, radc_var & p2, radc_stat * stat);
	};

	// 
	struct rexp_invoke : public radc_exp
	{
		DECLARE_RTTI();

		radc_function _pfn;
		FixedArray<radc_exp *, 8> _args;

		rexp_invoke() : _pfn(NULL) {}
		virtual ~rexp_invoke();

		static bool 
			match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat);

		virtual void
			build(radc_stat * stat);
		virtual void 
			execute(Array<radc_var> & results, radc_stat * stat);

		void
			parseArg(Token::iterator i, Token::iterator end, radc_stat * stat);
	};

}
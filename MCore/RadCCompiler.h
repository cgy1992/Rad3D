/*
*	RadC Compiler
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once 

#include "RadC.h"
#include "RadCExpression.h"

namespace Rad {

	//
	struct radc_stat;

	struct radc_program
	{
		Array<radc_var *> varlist;
		Array<radc_stat *> statements;

		radc_program();
		~radc_program();
	};

	//
	struct radc_compiler : public radc_program, public Singleton<radc_compiler>
	{
		radc_error_reporter error_reporter;
		FixedString256 error;

		char charmap[256];
		Array<char> buffer;
		Token * token;

		Array<FixedString32> libs;
		Map<int, radc_reg> functions;

		const radc_varlist * inputlist;
		radc_varlist * outputlist;
		Array<radc_var *> global_varlist;

		Map<const char *, radc_program *> programs;
		int _cache_program;
		int _options;
		bool _break;

		radc_compiler();

		void 
			set_error(const char * fmt, ...);
		void 
			clear_error();
		bool 
			is_error();
		bool
			is_break();

		void 
			init();
		void 
			shutdown();

		void 
			begin(const char * str, const radc_varlist * _varlist);
		void 
			end(const char * str);
		
		radc_var * 
			decl_variable(const char * name, radc_enum type, bool is_static = false);
		radc_var * 
			get_variable(const char * name);

		void
			reg_function(const char * name, const radc_reg & reg);
		radc_function
			get_function(const char * name);
		
		void
			compile(int options);
		void 
			execute();
	};

#define radc_get_compiler() radc_compiler & compiler = *radc_compiler::Instance();
}
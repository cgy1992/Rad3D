#include "RadC.h"
#include "RadCCompiler.h"
#include "RadCFoundation.h"
#include "MCodeTimer.h"

namespace Rad {

	radc_compiler compiler;

	void radc_init()
	{
		compiler.init();

		radc_regFunc("Print", radc_print, NULL);
		radc_regLib("Math", radc_mathlib);
	}

	void radc_shutdown()
	{
		radc_clear();

		compiler.shutdown();
	}

	void radc_clear()
	{
		for (int i = 0; i < compiler.programs.Size(); ++i)
		{
			delete compiler.programs[i].value;
		}

		compiler.programs.Clear();
	}

	void radc_clear(const char * script)
	{
		int i = compiler.programs.Find(script);
		if (i != -1)
		{
			delete compiler.programs[i].value;
			compiler.programs.Erase(i);
		}
	}

	void radc_regFunc(const char * name, radc_function pfn, const char * desc)
	{
		if (!Root::Instance()->InEditor())
		{
			radc_reg reg = { -1, NULL, pfn, NULL };

			compiler.reg_function(name, reg);
		}
		else
		{
			radc_reg reg = { -1, name, pfn, desc };

			compiler.reg_function(name, reg);
		}
	}

	void radc_regLib(const char * libname, radc_reg * regs)
	{
		d_assert (regs != NULL && libname != NULL);

		String prefix = libname, funcname;
		prefix += ":";

		for (int i = 0; regs[i].name != NULL && regs[i].pfn != NULL; ++i)
		{
			funcname = prefix + regs[i].name;

			regs[i]._lib = compiler.libs.Size();

			compiler.reg_function(funcname.c_str(), regs[i]);
		}

		if (Root::Instance()->InEditor())
		{
			compiler.libs.PushBack(libname);
		}
	}

	int radc_getRegSize()
	{
		return compiler.functions.Size();
	}

	const radc_reg & radc_getReg(int i)
	{
		return compiler.functions[i].value;
	}

	FixedString32 radc_getLibName(int i)
	{
		return i != -1 ? compiler.libs[i] : "";
	}

	const radc_varlist * __internal_args = NULL;

	bool radc_do(const char * script, int options, const radc_varlist * varlist, const radc_varlist * internalArgs)
	{
		profile_code();

		__internal_args = internalArgs;

		compiler.begin(script, varlist);

		compiler.compile(options);

		if ((options & RADC_OPATION_COMPILE_ONLY) == 0)
			compiler.execute();

		compiler.end(script);

		if (compiler.is_error() && compiler.error_reporter != NULL)
			compiler.error_reporter(radc_getError());

		__internal_args = NULL;

		return !compiler.is_error();
	}

	const radc_varlist * radc_result()
	{
		return compiler.outputlist;
	}

	radc_varlist * radc_result_()
	{
		return compiler.outputlist;
	}

	const radc_varlist * radc_internalArgs()
	{
		return __internal_args;
	}

	void radc_break()
	{
		compiler._break = true;
	}

	void radc_setError(const char * fmt, ...)
	{
		if (!compiler.is_error())
		{
			char text[256];
			va_list arglist;

			va_start(arglist, fmt);
			vsprintf(text, fmt, arglist);
			va_end(arglist);

			compiler.error = text;
		}
	}

	const char * radc_getError()
	{
		return compiler.error.c_str();
	}

	void radc_setErrorReporter(radc_error_reporter reporter)
	{
		compiler.error_reporter = reporter;
	}
}
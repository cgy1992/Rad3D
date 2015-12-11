/*
*	RadC
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"
#include "MLinker.h"
#include "MMap.h"
#include "MToken.h"
#include "RadCType.h"

namespace Rad {

	M_ENTRY void
		radc_init();
	M_ENTRY void
		radc_shutdown();
	M_ENTRY void
		radc_clear();
	M_ENTRY void
		radc_clear(const char * script);

	M_ENTRY void
		radc_regFunc(const char * name, radc_function pfn, const char * desc = NULL);
	M_ENTRY void
		radc_regLib(const char * libname, radc_reg * regs, const char * parent);
	M_ENTRY int
		radc_getRegSize();
	M_ENTRY const radc_reg &
		radc_getReg(int i);
	M_ENTRY int
		radc_getLib(const FixedString32 & name);
	M_ENTRY FixedString32
		radc_getLibName(int i);

	M_ENTRY void
		radc_setError(const char * fmt, ...);
	M_ENTRY const char * 
		radc_getError();
	M_ENTRY void
		radc_setErrorReporter(radc_error_reporter reporter);

	M_ENTRY bool 
		radc_do(const char * script, int options, const radc_varlist * args, const radc_varlist * internalArgs = NULL);
	M_ENTRY const radc_varlist *
		radc_result();
	M_ENTRY radc_varlist *
		radc_result_();
	M_ENTRY const radc_varlist *
		radc_internalArgs();
	M_ENTRY void
		radc_break();

#define radc_check_result(type) (radc_result()->size() == 1 && radc_result()->getType(0) == type)

#define radc_check_args1(args, type0) (args)->size() >= 1 && \
		(args)->getType(0) == type0

#define radc_check_args2(args, type0, type1) (args)->size() >= 2 && \
		(args)->getType(0) == type0 &&\
		(args)->getType(1) == type1

#define radc_check_args3(args, type0, type1, type2) (args)->size() >= 3 && \
		(args)->getType(0) == type0 && \
		(args)->getType(1) == type1 && \
		(args)->getType(2) == type2

#define radc_check_args4(args, type0, type1, type2, type3) (args)->size() >= 4 && \
	(args)->getType(0) == type0 && \
	(args)->getType(1) == type1 && \
	(args)->getType(2) == type2 && \
	(args)->getType(3) == type3

#define radc_check_args5(args, type0, type1, type2, type3, type4) (args)->size() >= 5 && \
	(args)->getType(0) == type0 && \
	(args)->getType(1) == type1 && \
	(args)->getType(2) == type2 && \
	(args)->getType(3) == type3 && \
	(args)->getType(4) == type4

#define radc_check_args6(args, type0, type1, type2, type3, type4, type5) (args)->size() >= 6 && \
	(args)->getType(0) == type0 && \
	(args)->getType(1) == type1 && \
	(args)->getType(2) == type2 && \
	(args)->getType(3) == type3 && \
	(args)->getType(4) == type4 && \
	(args)->getType(5) == type5

#define radc_check_args7(args, type0, type1, type2, type3, type4, type5, type6) (args)->size() >= 7 && \
	(args)->getType(0) == type0 && \
	(args)->getType(1) == type1 && \
	(args)->getType(2) == type2 && \
	(args)->getType(3) == type3 && \
	(args)->getType(4) == type4 && \
	(args)->getType(5) == type5 && \
	(args)->getType(6) == type6

#define radc_check_args8(args, type0, type1, type2, type3, type4, type5, type6, type7) (args)->size() >= 8 && \
	(args)->getType(0) == type0 && \
	(args)->getType(1) == type1 && \
	(args)->getType(2) == type2 && \
	(args)->getType(3) == type3 && \
	(args)->getType(4) == type4 && \
	(args)->getType(5) == type5 && \
	(args)->getType(6) == type6 && \
	(args)->getType(7) == type7

}
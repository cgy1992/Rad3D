/*
*	RadC Foundation
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "RadCType.h"

namespace Rad {

	bool
		radc_print(radc_var * result, const radc_varlist * args);

	bool 
		radc_math_max(radc_var * result, const radc_varlist * args);
	bool 
		radc_math_min(radc_var * result, const radc_varlist * args);
	bool
		radc_math_abs(radc_var * result, const radc_varlist * args);
	bool
		radc_math_sign(radc_var * result, const radc_varlist * args);
	bool
		radc_math_clamp(radc_var * result, const radc_varlist * args);
	bool
		radc_math_sqrt(radc_var * result, const radc_varlist * args);
	bool
		radc_math_sin(radc_var * result, const radc_varlist * args);
	bool
		radc_math_cos(radc_var * result, const radc_varlist * args);
	bool
		radc_math_tan(radc_var * result, const radc_varlist * args);
	bool
		radc_math_cot(radc_var * result, const radc_varlist * args);
	bool
		radc_math_asin(radc_var * result, const radc_varlist * args);
	bool
		radc_math_acos(radc_var * result, const radc_varlist * args);
	bool
		radc_math_floor(radc_var * result, const radc_varlist * args);
	bool
		radc_math_ceil(radc_var * result, const radc_varlist * args);
	bool
		radc_math_mod(radc_var * result, const radc_varlist * args);
	bool
		radc_math_exp(radc_var * result, const radc_varlist * args);
	bool
		radc_math_log(radc_var * result, const radc_varlist * args);
	bool
		radc_math_pow(radc_var * result, const radc_varlist * args);
	bool
		radc_math_rand(radc_var * result, const radc_varlist * args);
	bool
		radc_math_randrange(radc_var * result, const radc_varlist * args);

	extern radc_reg 
		radc_mathlib[];
}
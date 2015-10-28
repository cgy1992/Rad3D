#include "RadCFoundation.h"
#include "RadC.h"

namespace Rad {

	radc_reg radc_mathlib[] = {
		RADC_REG("Max", radc_math_max, "args: a, b"),
		RADC_REG("Min", radc_math_min, "args: a, b"),
		RADC_REG("Abs", radc_math_abs, "args: x"),
		RADC_REG("Sign", radc_math_sign, "args: x"),
		RADC_REG("Clamp", radc_math_clamp, "args: x, min, max"),
		RADC_REG("Sqrt", radc_math_sqrt, "args: x"),
		RADC_REG("Sin", radc_math_sin, "args: x"),
		RADC_REG("Cos", radc_math_cos, "args: x"),
		RADC_REG("Tan", radc_math_tan, "args: x"),
		RADC_REG("Cot", radc_math_cot, "args: x"),
		RADC_REG("Asin", radc_math_asin, "args: x"),
		RADC_REG("Acos", radc_math_acos, "args: x"),
		RADC_REG("Floor", radc_math_floor, "args: x"),
		RADC_REG("Ceil", radc_math_ceil, "args: x"),
		RADC_REG("Mod", radc_math_mod, "args: x, y"),
		RADC_REG("Exp", radc_math_exp, "args: x"),
		RADC_REG("Log", radc_math_log, "args: x"),
		RADC_REG("Pow", radc_math_pow, "args: x, y"),
		RADC_REG("Rand", radc_math_rand, NULL ),
		RADC_REG("RandRange", radc_math_randrange, "args: a, b"),
		RADC_REG(NULL, NULL, NULL)
	};

	bool radc_print(radc_var * result, const radc_varlist * args)
	{
		if (args->size() == 1)
		{
			if (args->getType(0) == radc_enum::type_real)
			{
				d_printf(f2str((float)args->getReal(0), true).c_str());

				return true;
			}
			else if (args->getType(0) == radc_enum::type_string)
			{
				d_printf(args->getString(0));

				return true;
			}
		}

		return false;
	}

	bool radc_math_max(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args2(args, radc_enum::type_real, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);
			radc_real b = args->getReal(1);

			result->setReal(Max(a, b));

			return true;
		}

		return false;
	}

	bool radc_math_min(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args2(args, radc_enum::type_real, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);
			radc_real b = args->getReal(1);

			result->setReal(Min(a, b));

			return true;
		}

		return false;
	}

	bool radc_math_abs(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(abs(a));

			return true;
		}

		return false;
	}

	bool radc_math_sign(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(Math::Sign(a));

			return true;
		}

		return false;
	}

	bool radc_math_clamp(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args3(args, radc_enum::type_real, radc_enum::type_real, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);
			radc_real b = args->getReal(1);
			radc_real c = args->getReal(2);

			result->setReal(Math::Clamp(a, b, c));

			return true;
		}

		return false;
	}

	bool radc_math_sqrt(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(sqrt(a));

			return true;
		}

		return false;
	}

	bool radc_math_sin(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(sin(a));

			return true;
		}

		return false;
	}

	bool radc_math_cos(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(cos(a));

			return true;
		}

		return false;
	}

	bool radc_math_tan(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(tan(a));

			return true;
		}

		return false;
	}

	bool radc_math_cot(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal((radc_real)1.0 / tan(a));

			return true;
		}

		return false;
	}

	bool radc_math_asin(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(asin(a));

			return true;
		}

		return false;
	}

	bool radc_math_acos(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(acos(a));

			return true;
		}

		return false;
	}

	bool radc_math_floor(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(floor(a));

			return true;
		}

		return false;
	}

	bool radc_math_ceil(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(ceil(a));

			return true;
		}

		return false;
	}

	bool radc_math_mod(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args2(args, radc_enum::type_real, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);
			radc_real b = args->getReal(1);

			result->setReal(fmod(a, b));

			return true;
		}

		return false;
	}

	bool radc_math_exp(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(exp(a));

			return true;
		}

		return false;
	}

	bool radc_math_log(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args1(args, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);

			result->setReal(log(a));

			return true;
		}

		return false;
	}

	bool radc_math_pow(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args2(args, radc_enum::type_real, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);
			radc_real b = args->getReal(1);

			result->setReal(pow(a, b));

			return true;
		}

		return false;
	}

	bool radc_math_rand(radc_var * result, const radc_varlist * args)
	{
		if (args->size() == 0)
		{
			result->setReal((radc_real)Math::Random());

			return true;
		}

		return false;
	}

	bool radc_math_randrange(radc_var * result, const radc_varlist * args)
	{
		if (radc_check_args2(args, radc_enum::type_real, radc_enum::type_real))
		{
			radc_real a = args->getReal(0);
			radc_real b = args->getReal(1);

			result->setReal((radc_real)Math::RandRange((int)a, (int)b));

			return true;
		}

		return false;
	}

}
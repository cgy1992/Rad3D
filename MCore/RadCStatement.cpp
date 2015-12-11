#include "RadCStatement.h"
#include "RadCType.h"
#include "RadCCompiler.h"
#include "RadCExpression.h"

namespace Rad {

	ImplementRootRTTI(radc_stat);

	radc_stat::radc_stat()
	{
		line = 0;
		jump = 0;
		str = 0;
		exp_tree = NULL;
	}

	radc_stat::~radc_stat()
	{
		safe_delete(exp_tree);
	}

	//
	ImplementRTTI(rstat_empty, radc_stat);

	//
	ImplementRTTI(rstat_entry, radc_stat);

	void rstat_entry::build()
	{
		radc_get_compiler();

		compiler.token->Clear();
		compiler.token->Parse((char *)str);

		exp_tree = new radc_exp;

		Token::iterator token = compiler.token->Begin();

		if (!token.valid() || token->type() != radc_enum::op_left_b)
		{
			compiler.set_error("Error: [%d] main.", line);
		}

		token = token.next();

		while (token.valid() && token.next().valid())
		{
			if (token->type() == 0 && token.next()->type() == 0)
			{
				radc_var * var = NULL;
				const char * type = token->str;
				const char * name = token.next()->str;

				if (strcmp(type, "int") == 0)
				{
					var = compiler.decl_variable(name, radc_enum::type_real);
				}
				else if (strcmp(type, "real") == 0)
				{
					var = compiler.decl_variable(name, radc_enum::type_real);
				}
				else if (strcmp(type, "string") == 0)
				{
					var = compiler.decl_variable(name, radc_enum::type_string);
				}
				else if (strcmp(type, "ptr") == 0)
				{
					var = compiler.decl_variable(name, radc_enum::type_ptr);
				}
				else
				{
					compiler.set_error("Error: [%d] 无效类型 '%s'.", line, type);
					break;
				}

				if (var == NULL)
				{
					compiler.set_error("Error: [%d] 变量定义错误 '%s'.", line, name);
					break;
				}

				args.PushBack(var);
			}
			else
			{
				compiler.set_error("Error: [%d] main.", line);
				break ;
			}

			token = token.next().next();

			if (token->type() == radc_enum::op_right_b)
			{
				break;
			}
			else if (token->type() != radc_enum::op_comma)
			{
				compiler.set_error("Error: [%d] main.", line);
				break;
			}

			token = token.next();
		}
	}

	int rstat_entry::execute()
	{
		radc_get_compiler();

		if (args.Size() > 0 && compiler.inputlist == NULL)
		{
			compiler.set_error("Error: [%d] 输入参数为空.", line);
			return 0;
		}

		for (int i = 0; i < args.Size(); ++i)
		{
			if (args[i]->getType() != compiler.inputlist->getType(i))
			{
				compiler.set_error("Error: [%d] 参数类型不匹配.", line);
				return 0;
			}

			*args[i] = compiler.inputlist->at(i);
		}

		return jump;
	}

	//
	ImplementRTTI(rstat_exp, radc_stat);

	void rstat_exp::build()
	{
		radc_get_compiler();

		compiler.token->Clear();
		compiler.token->Parse((char *)str);

		exp_tree = new radc_exp;

		exp_tree->parse(compiler.token->Begin(), compiler.token->End(), this);

		exp_tree->build(this);
	}

	int rstat_exp::execute()
	{
		Array<radc_var> results;
		exp_tree->execute(results, this);

		return jump;
	}

	//
	ImplementRTTI(rstat_if, rstat_exp);

	int rstat_if::execute()
	{
		radc_get_compiler();

		Array<radc_var> results;
		exp_tree->execute(results, this);

		if (results.Size() != 1)
		{
			compiler.set_error("Error: [%d] 条件表达式错误.", line);
			return 0;
		}

		if (results[0].getType() != radc_enum::type_real)
		{
			compiler.set_error("Error: [%d] 条件表达式错误.", line);
			return 0;
		}

		return results[0].getReal()!= 0 ? 0 : jump;
	}

	//
	ImplementRTTI(rstat_elseif, rstat_if);

	//
	ImplementRTTI(rstat_else, rstat_empty);

	//
	ImplementRTTI(rstat_end, rstat_empty);

	//
	ImplementRTTI(rstat_return, rstat_exp);

	int rstat_return::execute()
	{
		radc_get_compiler();

		Array<radc_var> results;
		exp_tree->execute(results, this);

		if (!compiler.is_break())
		{
			if (exp_tree->_first_child != NULL && results.Size() == 0)
			{
				compiler.set_error("Error: [%d], 表达式错误.", line);
			}
			else if (results.Size() > 1)
			{
				compiler.set_error("Error: [%d], 表达式错误.", line);
			}
			else if (results.Size() == 1)
			{
				radc_var & v = results[0];

				switch (v.getType())
				{
				case radc_enum::type_real:
					compiler.outputlist->push(v.getReal());
					break;

				case radc_enum::type_string:
					compiler.outputlist->push(v.getString());
					break;

				case radc_enum::type_ptr:
					compiler.outputlist->push(v.getPtr());
					break;

				default:
					compiler.set_error("Error: [%d], 表达式错误.", line);
					break;
				}
			}
		}

		return jump;
	}

	//
	ImplementRTTI(rstat_while, rstat_if);

	//
	/*
	ImplementRTTI(rstat_for, rstat_exp);

	rstat_for::rstat_for()
	{
		b_first = false;
		i_first = NULL;
		i_next = NULL;
	}

	rstat_for::~rstat_for()
	{
		safe_delete (i_first);
		safe_delete (i_next);
	}

	void rstat_for::build()
	{
		radc_get_compiler();

		compiler.token->Clear();
		compiler.token->Parse((char *)str);

		Token::iterator beg = compiler.token->Begin();
		Token::iterator end = compiler.token->End();

		int d = 0;
		Token::iterator i = beg;
		while (i < end)
		{
			if (i->type() == radc_enum::op_left_b)
			{
				d += 1;
			}
			else if (i->type() == radc_enum::op_right_b)
			{
				d -= 1;
			}
			else if (i->type() == radc_enum::op_comma)
			{
				if (d == 0)
				{
					i_first = new radc_exp;
					i_first->parse(beg, i, this);
					break;
				}
			}

			i = i.next();
		}

		if (i_first == NULL)
		{
			compiler.set_error("Error: [%d], 表达式错误.", line);
			return ;
		}

		beg = i.next(), i = beg;
		while (i < end)
		{
			if (i->type() == radc_enum::op_left_b)
			{
				d += 1;
			}
			else if (i->type() == radc_enum::op_right_b)
			{
				d -= 1;
			}
			else if (i->type() == radc_enum::op_comma)
			{
				if (d == 0)
				{
					exp_tree = new radc_exp;
					exp_tree->parse(beg, i, this);
					break;
				}
			}
		}

		if (exp_tree == NULL)
		{
			compiler.set_error("Error: [%d], 表达式错误.", line);
			return ;
		}

		beg = i.next();
		if (beg >= end)
		{
			compiler.set_error("Error: [%d], 表达式错误.", line);
			return ;
		}

		i_next = new radc_exp;
		i_next->parse(beg, end, this);

		i_first->build(this);
		exp_tree->build(this);
		i_next->build(this);
	}

	int rstat_for::execute()
	{
		radc_get_compiler();

		if (!b_first)
		{
			Array<radc_var> _r;
			i_next->execute(_r, this);
		}
		else
		{
			Array<radc_var> _r;
			i_first->execute(_r, this);
			b_first = false;
		}

		Array<radc_var> results;
		exp_tree->execute(results, this);

		if (results.Size() != 1)
		{
			compiler.set_error("Error: [%d] 条件表达式错误.", line);
			return 0;
		}

		if (results[0].getType() != radc_enum::type_real)
		{
			compiler.set_error("Error: [%d] 条件表达式错误.", line);
			return 0;
		}

		return results[0].getReal() != 0 ? 0 : jump;
	}
	*/

}
#include "RadCExpression.h"
#include "RadCCompiler.h"
#include "RadCStatement.h"
#include "RadCType.h"

namespace Rad {

	ImplementRootRTTI(radc_exp)

	radc_exp::radc_exp() 
		: _first_child(NULL)
		, _last_child(NULL)
		, _next_sibling(NULL)
	{
	}

	radc_exp::~radc_exp()
	{
		radc_exp * e = _first_child;
		while (e != NULL)
		{
			radc_exp * next = e->_next_sibling;

			delete e;

			e = next;
		}
	}

	void radc_exp::append(radc_exp * child)
	{
		if (_last_child != 0)
		{
			_last_child->_next_sibling = child;
			_last_child = child;
		}
		else
		{
			_first_child = _last_child = child;
		}
	}

	void radc_exp::parse(Token::iterator i, Token::iterator end, radc_stat * stat)
	{
		radc_get_compiler();

		Token::iterator beg = i;
		bool slipted = false;
		int left_b = 0;

		while (i < end)
		{
			if (i->type() == radc_enum::op_left_b)
			{
				left_b += 1;
			}
			else if (i->type() == radc_enum::op_right_b)
			{
				left_b -= 1;
			}

			// 分割符
			if (left_b == 0 && i->type() == radc_enum::op_comma)
			{
				slipted = true;

				radc_exp * child = new radc_exp;
				child->parse(beg, i, stat);
				append(child);

				beg = i.next();
			}

			i = i.next();
		}

		if (!slipted)
		{
			i = beg;
			for (Token::iterator r(NULL, -1); !compiler.is_error() && i.valid() && i < end; i = r)
			{
				bool matched = false;

				if (!compiler.is_error() && rexp_invoke::match(i, end, r, this, stat))
				{
					matched = true;
				}
				else if (!compiler.is_error() && rexp_vardef::match(i, end, r, this, stat))
				{
					matched = true;
				}
				else if (!compiler.is_error() && rexp_variable::match(i, end, r, this, stat))
				{
					matched = true;
				}
				else if (!compiler.is_error() && rexp_operator::match(i, end, r, this, stat))
				{
					matched = true;
				}
				else if (!compiler.is_error() && rexp_constant::match(i, end, r, this, stat))
				{
					matched = true;
				}

				if (!compiler.is_error() && !matched)
				{
					compiler.set_error("Error:[%d], invalid statement", stat->line);
				}
			}
		}
		else if (beg < i)
		{
			radc_exp * child = new radc_exp;
			child->parse(beg, i, stat);
			append(child);
		}
	}

	void radc_exp::build(radc_stat * stat)
	{
		radc_get_compiler();

		Array<rexp_operator *, t_alloc_pool<rexp_operator *> > op_stack;

		for (radc_exp * e = _first_child; e != NULL && !compiler.is_error(); e = e->_next_sibling)
		{
			e->build(stat);

			if (TYPE_OF(rexp_operator, e))
			{
				rexp_operator * eop = static_cast<rexp_operator *>(e);

				if (eop->_op == radc_enum::op_left_b)
				{
					op_stack.PushBack(eop);
				}
				else if (eop->_op == radc_enum::op_right_b)
				{
					while (op_stack.Size() && op_stack.Back()->_op != radc_enum::op_left_b)
					{
						_output.PushBack(op_stack.Back());
						
						op_stack.PopBack();
					}

					if (op_stack.Size() == 0 || op_stack.Back()->_op != radc_enum::op_left_b)
					{
						compiler.set_error("Error:[%d] () not matched.", stat->line);
					}
					else
					{
						op_stack.PopBack();
					}
				}
				else
				{
					if (op_stack.Size() > 0)
					{
						if (eop->_priority <= op_stack.Back()->_priority)
						{
							_output.PushBack(op_stack.Back());
							
							op_stack.PopBack();
							op_stack.PushBack(eop);
						}
						else
						{
							op_stack.PushBack(eop);
						}
					}
					else
					{
						op_stack.PushBack(eop);
					}
				}
			}
			else
			{
				_output.PushBack(e);
			}
		}

		while (op_stack.Size() && !compiler.is_error())
		{
			if (op_stack.Back()->_op == radc_enum::op_left_b)
			{
				compiler.set_error("Error:[%d] () not matched.", stat->line);
				return ;
			}

			_output.PushBack(op_stack.Back());
			
			op_stack.PopBack();
		}
	}

	void radc_exp::execute(Array<radc_var> & results, radc_stat * stat)
	{
		radc_get_compiler();

		for (int i = 0; i < _output.Size() && !compiler.is_error() && !compiler.is_break(); ++i)
		{
			_output[i]->execute(results, stat);
		}
	}

	//
	ImplementRTTI(rexp_constant, radc_exp);

	bool rexp_constant::match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat)
	{
		d_assert (i.valid());

		radc_get_compiler();

		radc_var var;

		if (i->type() == 0)
		{
			const char * str = i->str;
			if (*str == '\"')
			{
				FixedString256 text;
				str = str_substring(&text[0], 256, str + 1, '\"');

				if (*str != '\"')
				{
					compiler.set_error("Error:[%d], \"\" not matched.", stat->line);
					return NULL;
				}

				var.setString(text.c_str(), true);
			}
			else if (*str >= '0' && *str <= '9')
			{
				var.setReal((radc_real)atof(str));
			}
			else if (strcmp(str, "true") == 0)
			{
				var.setInt(1);
			}
			else if (strcmp(str, "false") == 0)
			{
				var.setInt(0);
			}

			if (var.getType() != radc_enum::type_void)
			{
				Token::iterator n = i.next();
				if (n.valid())
				{
					if (n->type() == 0)
					{
						compiler.set_error("Error:[%d], expression.", stat->line);
					}
					else if (n->type() == radc_enum::op_left_b)
					{
						compiler.set_error("Error:[%d], expression.", stat->line);
					}
				}
			}
		}

		if (!compiler.is_error() && var.getType() != radc_enum::type_void)
		{
			r = i.next();

			rexp_constant * e = new rexp_constant;
			e->_var = var;
			parent->append(e);

			return true;
		}

		return false;
	}

	void rexp_constant::execute(Array<radc_var> & results, radc_stat * stat)
	{
		results.PushBack(_var);
	}

	//
	ImplementRTTI(rexp_variable, radc_exp);

	rexp_variable * rexp_variable::match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat)
	{
		if (i->type() != 0)
			return NULL;

		radc_get_compiler();

		const char * str = i->str;
		if ((*str >= 'a' && *str <= 'z') || (*str) > 'A' && (*str) < 'Z')
		{
			Token::iterator n = i.next();
			if (n.valid())
			{
				if (n->type() == 0)
				{
					compiler.set_error("Error:[%d], expression.", stat->line);
				}
				else if (n->type() == radc_enum::op_left_b)
				{
					compiler.set_error("Error:[%d], expression.", stat->line);
				}
			}
		}
		else
		{
			return NULL;
		}

		if (!compiler.is_error())
		{
			radc_var * var = compiler.get_variable(str);
			if (var != NULL)
			{
				r = i.next();

				rexp_variable * e = new rexp_variable;
				e->_var = var;
				e->_op = radc_enum::none;
				e->_exp = NULL;

				parent->append(e);

				if (r.valid() && (
					r->type() == radc_enum::op_equal ||
					r->type() == radc_enum::op_add_equal ||
					r->type() == radc_enum::op_sub_equal ||
					r->type() == radc_enum::op_mul_equal ||
					r->type() == radc_enum::op_div_equal)
					)
				{
					e->_op = (radc_enum::enum_t)r->type();

					Token::iterator beg = r.next();
					int left_b = 0;

					i = beg;
					while (i < end)
					{
						if (i->type() == radc_enum::op_left_b)
							++left_b;
						else if (i->type() == radc_enum::op_right_b)
							--left_b;

						// 分割符
						if (left_b < 0)
							break;

						if (left_b == 0 && i->type() == radc_enum::op_comma)
							break;

						i = i.next();
					}

					if (beg.valid() && beg <= i)
					{
						e->_exp = new radc_exp;
						e->_exp->parse(beg, i, stat);

						r = i;
						if (r.valid() && r->type() == radc_enum::op_comma)
						{
							r = r.next();
						}
					}
					else
					{
						compiler.set_error("Error:[%d], 赋值语句错误.", stat->line);
					}
				}

				return e;
			}
			else
			{
				compiler.set_error("Error:[%d], variable defined?", stat->line);
			}
		}
		
		return NULL;
	}

	void rexp_variable::build(radc_stat * stat)
	{
		radc_get_compiler();

		if (_exp != NULL)
		{
			if (_var->isConst() && !_enable_const)
			{
				compiler.set_error("Error:[%d], const variable can't set value?", stat->line);
				return ;
			}

			_exp->build(stat);
		}
	}

	void rexp_variable::execute(Array<radc_var> & results, radc_stat * stat)
	{
		radc_get_compiler();

		Array<radc_var> vl;
		if (_exp != NULL)
		{
			_exp->execute(vl, stat);

			if (vl.Size() != 1)
			{
				compiler.set_error("Error:[%d], 赋值表达式错误.", stat->line);
				return ;
			}

			radc_var & p1 = *_var;
			radc_var & p2 = vl[0];

			if (p1.getType() != p2.getType())
			{
				if (p1.getType() > p2.getType())
				{
					p2 = p2.up_type(p1.getType());
				}
				else
				{
					p2.down_type(p1.getType());
				}
			}

			if (p1.getType() != p2.getType() ||
				p1.getType() == radc_enum::type_void ||
				p2.getType() == radc_enum::type_void)
			{
				compiler.set_error("Error:[%d], 赋值表达式错误.", stat->line);
				return ;
			}

			if (_op == radc_enum::op_equal)
			{
				(p1) = (p2);
			}
			else if (_op == radc_enum::op_add_equal)
			{
				(p1) += (p2);
			}
			else if (_op == radc_enum::op_sub_equal)
			{
				(p1) -= (p2);
			}
			else if (_op == radc_enum::op_mul_equal)
			{
				(p1) *= (p2);
			}
		}

		results.PushBack(*_var);
	}

	//
	bool rexp_vardef::match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat)
	{
		if (i->type() != 0)
			return false;

		radc_get_compiler();

		bool is_static = false;
		bool is_const = false;

		if (strcmp(i->str, "static") == 0)
		{
			is_static = true;
			i = i.next();

			if (!i.valid())
			{
				compiler.set_error("Error:[%d], 变量定义错误.", stat->line);
				return false;
			}
		}

		if (strcmp(i->str, "const") == 0)
		{
			is_const = true;
			i = i.next();

			if (!i.valid())
			{
				compiler.set_error("Error:[%d], 变量定义错误.", stat->line);
				return false;
			}
		}

		radc_enum type;
		if (strcmp(i->str, "real") == 0)
		{
			type = radc_enum::type_real;
		}
		else if (strcmp(i->str, "string") == 0)
		{
			type = radc_enum::type_string;
		}
		else if (strcmp(i->str, "ptr") == 0)
		{
			type = radc_enum::type_ptr;
		}
		else
		{
			if (is_static || is_const)
				compiler.set_error("Error:[%d], 变量定义错误, const, static是关键字.", stat->line);

			return false;
		}

		i = i.next();
		if (!i.valid())
		{
			compiler.set_error("Error:[%d], 变量定义错误.", stat->line);
			return false;
		}

		const char * str = i->str;
		if ((*str >= 'a' && *str <= 'z') || (*str) > 'A' && (*str) < 'Z')
		{
			Token::iterator n = i.next();
			if (n.valid())
			{
				if (n->type() == 0)
				{
					compiler.set_error("Error:[%d], expression.", stat->line);
				}
				else if (n->type() == radc_enum::op_left_b)
				{
					compiler.set_error("Error:[%d], expression.", stat->line);
				}
			}
		}
		
		if (!compiler.is_error())
		{
			radc_var * var = compiler.decl_variable(str, type, is_static);
			if (var != NULL)
			{
				if (is_const)
					var->setConst();

				rexp_variable * e = rexp_variable::match(i, end, r, parent, stat);
				if (e != NULL)
				{
					e->_enable_const = true;
					return true;
				}
				else
				{
					compiler.set_error("Error:[%d], variable declare failed?", stat->line);
				}
			}
			else
			{
				compiler.set_error("Error:[%d], variable declare failed?", stat->line);
			}
		}

		return false;
	}

	//
	ImplementRTTI(rexp_operator, radc_exp);

	bool rexp_operator::match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat)
	{
		radc_get_compiler();

		if (i->type() != 0)
		{
			Token::iterator n = i.next();
			radc_enum _op = (radc_enum::enum_t)i->type();

			if (_op == radc_enum::op_left_b)
			{
				if (n < end)
				{
					if (n->type() != radc_enum::op_left_b &&
						n->type() != radc_enum::op_sub &&
						n->type() != 0)
					{
						compiler.set_error("Error:[%d], expression.", stat->line);
					}
				}
				else
				{
					compiler.set_error("Error:[%d], expression.", stat->line);
				}
			}
			else if (_op == radc_enum::op_right_b)
			{
				if (n < end)
				{
					if (n->type() != radc_enum::op_left_b && n->type() == 0)
					{
						compiler.set_error("Error:[%d], expression.", stat->line);
					}
				}
			}
			else if (_op == radc_enum::op_sub)
			{
				if (parent->_last_child == NULL)
				{
					_op = radc_enum::op_neg;

					if (n < end)
					{
						if (n->type() != 0 && n->type() != radc_enum::op_left_b)
						{
							compiler.set_error("Error:[%d], expression.", stat->line);
						}
					}
					else
					{
						compiler.set_error("Error:[%d], expression.", stat->line);
					}
				}
				else if (TYPE_OF(rexp_operator, parent->_last_child))
				{
					radc_enum last_op = static_cast<rexp_operator *>(parent->_last_child)->_op;
					if (last_op != radc_enum::op_right_b)
					{
						_op = radc_enum::op_neg;
					}

					if (n < end)
					{
						if (_op == radc_enum::op_neg)
						{
							if (n->type() != 0 && n->type() != radc_enum::op_left_b)
							{
								compiler.set_error("Error:[%d], expression.", stat->line);
							}
						}
						else
						{
							if (n->type() != 0 && n->type() != radc_enum::op_left_b && n->type() != radc_enum::op_sub)
							{
								compiler.set_error("Error:[%d], expression.", stat->line);
							}
						}
					}
					else
					{
						compiler.set_error("Error:[%d], expression.", stat->line);
					}
				}
			}
			else
			{
				if (n < end)
				{
					if (n->type() != 0 && n->type() != radc_enum::op_sub && n->type() != radc_enum::op_left_b)
					{
						compiler.set_error("Error:[%d], expression.", stat->line);
					}
				}
				else
				{
					compiler.set_error("Error:[%d], expression.", stat->line);
				}
			}

			if (!compiler.is_error())
			{
				r = i.next();

				rexp_operator * e = new rexp_operator;
				e->_op = _op;
				e->_priority = get_priority(_op);
				parent->append(e);

				return true;
			}
		}

		return false;
	}

	int rexp_operator::get_priority(radc_enum _op)
	{
		switch (_op.value)
		{
		case radc_enum::op_neg:
			return 10;

		case radc_enum::op_less:
		case radc_enum::op_greater:
		case radc_enum::op_less_equal:
		case radc_enum::op_greater_equal:
		case radc_enum::op_not_equal:
		case radc_enum::op_equal_equal:
			return 3;

		case radc_enum::op_and_and:
		case radc_enum::op_or_or:
			return 2;

		case radc_enum::op_equal:
		case radc_enum::op_add_equal:
		case radc_enum::op_sub_equal:
		case radc_enum::op_mul_equal:
		case radc_enum::op_div_equal:
			return 1;

		case radc_enum::op_add:
		case radc_enum::op_sub:
			return 4;
		case radc_enum::op_mul:
		case radc_enum::op_div:
			return 5;

		case radc_enum::op_left_b:
		case radc_enum::op_right_b:
		case radc_enum::op_comma:
			return 0;
		}

		return 0;
	}

	void rexp_operator::execute(Array<radc_var> & results, radc_stat * stat)
	{
		radc_get_compiler();

		radc_var result;

		if (_op != radc_enum::op_neg)
		{
			if (results.Size() >= 2)
			{
				radc_var p1 = results[results.Size() - 2];
				radc_var p2 = results[results.Size() - 1];

				results.Erase(results.Size() - 2, 2);

				if (_op == radc_enum::op_less || _op == radc_enum::op_greater || _op == radc_enum::op_less_equal || _op == radc_enum::op_greater_equal ||
					_op == radc_enum::op_not_equal || _op == radc_enum::op_equal_equal || _op == radc_enum::op_and_and || _op == radc_enum::op_or_or)
				{
					result = _do_compare(p1, p2, stat);
					if (result.getType() == radc_enum::type_void)
					{
						compiler.set_error("Error: [%d], expression.", stat->line);
					}
				}
				else if (_op == radc_enum::op_equal || _op == radc_enum::op_add_equal || 
					_op == radc_enum::op_sub_equal || _op == radc_enum::op_mul_equal || _op == radc_enum::op_div_equal)
				{
					compiler.set_error("Error: [%d], expression.", stat->line);
				}
				else if (_op == radc_enum::op_add || _op == radc_enum::op_sub || _op == radc_enum::op_mul || _op == radc_enum::op_div)
				{
					result = _do_arithmetic(p1, p2, stat);
					if (result.getType() == radc_enum::type_void)
					{
						compiler.set_error("Error: [%d], expression.", stat->line);
					}
				}
			}
			else
			{
				compiler.set_error("Error: [%d], expression.", stat->line);
			}
		}
		else
		{
			if (results.Size() >= 1)
			{
				radc_var p1 = results[results.Size() - 1];
				results.Erase(results.Size() - 1, 1);

				if (p1.getType() == radc_enum::type_real)
				{
					result.setReal(-p1.getReal());
				}
				else
				{
					compiler.set_error("Error: [%d], negative only for real.", stat->line);
				}
			}
		}

		if (result.getType() != radc_enum::type_void)
		{
			results.PushBack(result);
		}
	}

	radc_var rexp_operator::_do_compare(radc_var & p1, radc_var & p2, radc_stat * stat)
	{
		radc_var result;

		if (p1.getType() != p2.getType())
		{
			if (p1.getType() > p2.getType())
			{
				p2 = p2.up_type(p1.getType());
			}
			else
			{
				p2.down_type(p1.getType());
			}
		}

		if (p1.getType() == radc_enum::type_void ||
			p2.getType() == radc_enum::type_void || 
			p1.getType() != p2.getType())
		{
			return result;
		}

		if (_op == radc_enum::op_less)
		{
			result = (p1) < (p2);
		}
		else if (_op == radc_enum::op_less_equal)
		{
			result = (p1) <= (p2);
		}
		else if (_op == radc_enum::op_greater)
		{
			result = (p1) > (p2);
		}
		else if (_op == radc_enum::op_greater_equal)
		{
			result = (p1) >= (p2);
		}
		else if (_op == radc_enum::op_not_equal)
		{
			result = (p1) != (p2);
		}
		else if (_op == radc_enum::op_equal_equal)
		{
			result = (p1) == (p2);
		}
		else if (_op == radc_enum::op_and_and)
		{
			result = (p1) && (p2);
		}
		else if (_op == radc_enum::op_or_or)
		{
			result = (p1) || (p2);
		}

		return result;
	}

	radc_var rexp_operator::_do_arithmetic(radc_var & p1, radc_var & p2, radc_stat * stat)
	{
		radc_var result;

		if (p1.getType() != p2.getType())
		{
			if (p1.getType() > p2.getType())
			{
				p2 = p2.up_type(p1.getType());
			}
			else
			{
				p2.down_type(p1.getType());
			}
		}

		if (p1.getType() == radc_enum::type_void ||
			p2.getType() == radc_enum::type_void || 
			p1.getType() != p2.getType())
		{
			return result;
		}

		if (_op == radc_enum::op_add)
		{
			result = (p1) + (p2);
		}
		else if (_op == radc_enum::op_sub)
		{
			result = (p1) - (p2);
		}
		else if (_op == radc_enum::op_mul)
		{
			result = (p1) * (p2);
		}
		else if (_op == radc_enum::op_div)
		{
			result = (p1) / (p2);
		}

		return result;
	}

	//
	ImplementRTTI(rexp_invoke, radc_exp);

	rexp_invoke::~rexp_invoke()
	{
		for (int i = 0; i < _args.Size(); ++i)
		{
			delete _args[i];
		}
	}

	bool rexp_invoke::match(Token::iterator i, Token::iterator end, Token::iterator & r, radc_exp * parent, radc_stat * stat)
	{
		if (i->type() != 0)
			return false;

		radc_get_compiler();

		Token::iterator beg = i;

		i = i.next();
		if (i < end && i.valid() && i->type() == radc_enum::op_left_b)
		{
			int left_b = 0;
			bool matched = false;

			while (i.valid())
			{
				if (i->type() == radc_enum::op_left_b)
				{
					left_b += 1;
				}
				else if (i->type() == radc_enum::op_right_b)
				{
					left_b -= 1;
				}

				if (left_b == 0)
				{
					matched = true;
					break;
				}

				i = i.next();
			}

			if (!matched)
			{
				compiler.set_error("Error: [%d], invoke expression.", stat->line);
				return false;
			}

			radc_function pfn = compiler.get_function(beg->str);

			if (pfn == NULL)
			{
				compiler.set_error("Error: [%d], function not found.", stat->line);
				return false;
			}

			rexp_invoke * e = new rexp_invoke;
			e->_pfn = pfn;

			beg = beg.next().next();
			end = i;
			i = beg;

			left_b = 0;
			while (i < end)
			{
				if (i->type() == radc_enum::op_left_b)
				{
					left_b += 1;
				}
				else if (i->type() == radc_enum::op_right_b)
				{
					left_b -= 1;
				}

				// 分割符号
				if (left_b == 0 && i->type() == radc_enum::op_comma)
				{
					e->parseArg(beg, i, stat);
					beg = i.next();
				}

				i = i.next();
			}

			if (beg < i)
			{
				e->parseArg(beg, i, stat);
			}

			parent->append(e);

			r = end.next();

			return true;
		}

		return false;
	}

	void rexp_invoke::parseArg(Token::iterator i, Token::iterator end, radc_stat * stat)
	{
		radc_get_compiler();

		if (_args.Size() >= _args.Capacity())
		{
			compiler.set_error("Error: [%d], function args too mush.", stat->line);
			return ;
		}

		radc_exp * e = new radc_exp;
		e->parse(i, end, stat);

		_args.PushBack(e);
	}

	void rexp_invoke::build(radc_stat * stat)
	{
		for (int i = 0; i < _args.Size(); ++i)
		{
			_args[i]->build(stat);
		}
	}

	void rexp_invoke::execute(Array<radc_var> & results, radc_stat * stat)
	{
		radc_get_compiler();

		Array<radc_var> result;
		for (int i = 0; i < _args.Size(); ++i)
		{
			_args[i]->execute(result, stat);
		}

		if (_args.Size() != result.Size())
		{
			compiler.set_error("Error: [%d], function args.", stat->line);
			return ;
		}

		radc_varlist varlist;
		for (int i = 0; i < result.Size(); ++i)
		{
			varlist.push(result[i]);
		}

		radc_var v;
		if (!_pfn(&v, &varlist))
		{
			compiler.set_error("Error: [%d], function execute.", stat->line);
		}

		results.PushBack(v);
	}
}
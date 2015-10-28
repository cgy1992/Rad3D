#include "RadDBSql.h"
#include "RadDBManager.h"
#include "RadDBTable.h"
#include "RadDBScript.h"
#include "MFunctional.h"

namespace Rad {

	ImplementRootRTTI(rsql_exp);

	rsql_exp::rsql_exp()
		: _first_child(NULL)
		, _last_child(NULL)
		, _next_sibling(NULL)
	{
	}

	rsql_exp::~rsql_exp()
	{
		rsql_exp * e = _first_child;
		while (e != NULL)
		{
			rsql_exp * next = e->_next_sibling;

			delete e;

			e = next;
		}
	}

	void rsql_exp::append(rsql_exp * child)
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

	void rsql_exp::parse(Token::iterator i, Token::iterator end)
	{
		rsql_get_compiler();

		Token::iterator beg = i;

		i = beg;
		for (Token::iterator r(NULL, -1); !compiler.is_error() && i.valid() && i < end; i = r)
		{
			bool matched = false;

			if (!compiler.is_error() && rsql_property::match(i, end, r, this))
			{
				matched = true;
			}
			else if (!compiler.is_error() && rsql_operator::match(i, end, r, this))
			{
				matched = true;
			}
			else if (!compiler.is_error() && rsql_constant::match(i, end, r, this))
			{
				matched = true;
			}
			else if (!compiler.is_error() && rsql_invoke::match(i, end, r, this))
			{
				matched = true;
			}
			if (!compiler.is_error() && !matched)
			{
				compiler.set_error("Error: invalid statement");
			}
		}
	}

	void rsql_exp::build()
	{
		rsql_get_compiler();

		Array<rsql_operator *, t_alloc_pool<rsql_operator *> > op_stack;

		for (rsql_exp * e = _first_child; e != NULL && !compiler.is_error(); e = e->_next_sibling)
		{
			e->build();

			if (TYPE_OF(rsql_operator, e))
			{
				rsql_operator * eop = static_cast<rsql_operator *>(e);

				if (eop->_op == rsql_operator::op_left_b)
				{
					op_stack.PushBack(eop);
				}
				else if (eop->_op == rsql_operator::op_right_b)
				{
					while (op_stack.Size() && op_stack.Back()->_op != rsql_operator::op_left_b)
					{
						output.PushBack(op_stack.Back());
						op_stack.PopBack();
					}

					if (op_stack.Size() == 0 || op_stack.Back()->_op != rsql_operator::op_left_b)
					{
						compiler.set_error("Error: () not matched.");
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
							output.PushBack(op_stack.Back());
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
				output.PushBack(e);
			}
		}

		while (op_stack.Size() && !compiler.is_error())
		{
			if (op_stack.Back()->_op == rsql_operator::op_left_b)
			{
				compiler.set_error("Error: () not matched.");
				return ;
			}

			output.PushBack(op_stack.Back());
			op_stack.PopBack();
		}
	}

	void rsql_exp::execute(Array<rdb_value> & results)
	{
		rsql_get_compiler();

		for (int i = 0; i < output.Size() && !compiler.is_error(); ++i)
		{
			output[i]->execute(results);
		}
	}

	//
	ImplementRTTI(rsql_constant, rsql_exp)

	bool rsql_constant::match(Token::iterator i, Token::iterator end, Token::iterator & r, rsql_exp * parent)
	{
		d_assert (i.valid());

		rsql_get_compiler();

		rdb_value v;

		if (i->type() == 0)
		{
			const char * str = i->str;
			if (*str == '\"')
			{
				FixedString256 text;
				str = str_substring(&text[0], 256, str + 1, '\"');

				if (*str != '\"')
				{
					compiler.set_error("Error: \"\" not matched.");
					return false;
				}

				v.SetString(text.c_str());
			}
			else if (*str >= '0' && *str <= '9')
			{
				bool real = false;

				const char * tmp = str;
				while (*tmp)
				{
					if (*tmp == '.')
					{
						real = true;
					}

					++tmp;
				}

				if (!real)
				{
					v.SetInt(atoi64(str));
				}
				else
				{
					v.SetReal(atof(str));
				}
			}
			else if (strcmp(str, "NULL") == 0)
			{
				v.SetNull();
			}
			
			if (v.Type() != rdb_value::TYPE_VOID)
			{
				Token::iterator n = i.next();
				if (n.valid())
				{
					if (n->type() == 0)
					{
						compiler.set_error("Error: expression.");
					}
					else if (n->type() == rsql_operator::op_left_b)
					{
						compiler.set_error("Error: expression.");
					}
				}
			}
		}

		if (!compiler.is_error() && v.Type() != rdb_value::TYPE_VOID)
		{
			r = i.next();

			rsql_constant * e = new rsql_constant;
			e->_var = v;
			parent->append(e);

			return true;
		}

		return false;
	}

	void rsql_constant::execute(Array<rdb_value> & result)
	{
		result.PushBack(_var);
	}

	//
	ImplementRTTI(rsql_property, rsql_exp);

	bool rsql_property::match(Token::iterator i, Token::iterator end, Token::iterator & r, rsql_exp * parent)
	{
		rsql_get_compiler();

		if (i->type() == 0)
		{
			rdb_table * table = (rdb_table *)compiler.i_table;

			const char * str = i->str;
			if (*str == '\'')
			{
				FixedString256 text;
				str = str_substring(&text[0], 256, str + 1, '\'');

				if (*str != '\'')
				{
					compiler.set_error("Error: \'\' not matched.");
					return false;
				}

				const Property * p = table->i_object->GetProperty(text.c_str());
				if (p != NULL)
				{
					if (p->type == PT_Int || p->type == PT_Float || p->type == PT_FixedString || p->type == PT_String)
					{
						rsql_property * e = new rsql_property;
						e->_prop = p;
						parent->append(e);

						r = i.next();

						return true;
					}
					else
					{
						compiler.set_error("Error: property '%s' type error.", text.c_str());
					}
				}
				else
				{
					compiler.set_error("Error: property '%s' not exist.", text.c_str());
				}
			}
		}

		return false;
	}

	void rsql_property::execute(Array<rdb_value> & result)
	{
		rsql_get_compiler();

		rdb_value v;

		if (compiler.i_record != NULL)
		{
			get_value(v, _prop, compiler.i_record);
		}

		if (v.Type() != rdb_value::TYPE_VOID)
		{
			result.PushBack(v);
		}
		else
		{
			compiler.set_error("Error: property execute.");
		}
	}

	void rsql_property::get_value(rdb_value & v, const Property * p, const IObject * obj)
	{
		const void * data = obj->GetPropertyData(p);

		switch (p->type)
		{
		case PT_Bool:
			v.SetInt(p->AsBool(data) ? 1 : 0);
			break;

		case PT_Int:
			if (p->flag & PF_Unsigned)
			{
				rdb_uint i = 0;
				memcpy(&i, data, p->size);

				v.SetUInt(i);
			}
			else
			{
				rdb_int i = 0;
				memcpy(&i, data, p->size);

				v.SetInt(i);
			}
			break;

		case PT_Float:
			if (p->size == sizeof(float))
			{
				v.SetReal(p->AsFloat(data));
			}
			else if (p->size == sizeof(double))
			{
				v.SetReal(p->AsDouble(data));
			}
			break;

		case PT_FixedString:
			v.SetString(p->AsFixedString(data));
			break;

		case PT_String:
			v.SetString(p->AsString(data));
			break;
		}
	}

	int rsql_property::compare(const Property * p, const IObject * obj1, const IObject * obj2)
	{
		const void * data1 = obj1->GetPropertyData(p);
		const void * data2 = obj2->GetPropertyData(p);

		switch (p->type)
		{
		case PT_Int:
			d_assert (p->size <= 8);

			if (p->flag & PF_Unsigned)
			{
				rdb_uint i1 = 0, i2 = 0;
				memcpy(&i1, data1, p->size);
				memcpy(&i2, data2, p->size);

				return i1 < i2 ? -1 : (i1 > i2 ? 1 : 0);
			}
			else
			{
				rdb_int i1 = 0, i2 = 0;

				switch (p->size)
				{
				case 4:
					i1 = *(int32 *)data1;
					i2 = *(int32 *)data2;
					break;

				case 2:
					i1 = *(int16 *)data1;
					i2 = *(int16 *)data2;
					break;

				case 1:
					i1 = *(int8 *)data1;
					i2 = *(int8 *)data2;
					break;

				case 8:
					i1 = *(int64 *)data1;
					i2 = *(int64 *)data2;
					break;
				}

				return i1 < i2 ? -1 : (i1 > i2 ? 1 : 0);
			}
			break;

		case PT_Float:
			if (p->size == sizeof(float))
			{
				float f1, f2;
				f1 = p->AsFloat(data1);
				f2 = p->AsFloat(data2);

				return f1 < f2 ? -1 : (f1 > f2 ? 1 : 0);
			}
			else if (p->size == sizeof(double))
			{
				double f1, f2;
				f1 = p->AsDouble(data1);
				f2 = p->AsDouble(data2);

				return f1 < f2 ? -1 : (f1 > f2 ? 1 : 0);
			}
			break;

		case PT_FixedString:
			if (p->flag & PF_Stricmp)
				return stricmp(p->AsFixedString(data1), p->AsFixedString(data2));
			else
				return strcmp(p->AsFixedString(data1), p->AsFixedString(data2));
			break;

		case PT_String:
			if (p->flag & PF_Stricmp)
				return stricmp(p->AsString(data1), p->AsString(data2));
			else
				return strcmp(p->AsString(data1), p->AsString(data2));
			break;
		}

		d_assert (0);

		return 0;
	}

	int rsql_property::compare(const IObject * obj, const Property * p, const rdb_value & v)
	{
		const void * data1 = obj->GetPropertyData(p);

		switch (p->type)
		{
		case PT_Int:
			d_assert (p->size <= 8);

			if (p->flag & PF_Unsigned)
			{
				rdb_uint i1 = 0, i2 = 0;
				memcpy(&i1, data1, p->size);
				i2 = v.AsUInt();

				return i1 < i2 ? -1 : (i1 > i2 ? 1 : 0);
			}
			else
			{
				rdb_int i1 = 0, i2 = 0;
				switch (p->size)
				{
				case 4:
					i1 = *(int32 *)data1;
					break;

				case 2:
					i1 = *(int16 *)data1;
					break;

				case 1:
					i1 = *(int8 *)data1;
					break;

				case 8:
					i1 = *(int64 *)data1;
					break;
				}

				i2 = v.AsInt();

				return i1 < i2 ? -1 : (i1 > i2 ? 1 : 0);
			}
			break;

		case PT_Float:
			if (p->size == sizeof(float))
			{
				float f1 = 0, f2 = 0;
				f1 = p->AsFloat(data1);
				f2 = (float)v.AsReal();

				return f1 < f2 ? -1 : (f1 > f2 ? 1 : 0);
			}
			else if (p->size == sizeof(double))
			{
				double f1 = 0, f2 = 0;
				f1 = p->AsDouble(data1);
				f2 = v.AsReal();

				return f1 < f2 ? -1 : (f1 > f2 ? 1 : 0);
			}
			break;

		case PT_FixedString:
			if (p->flag & PF_Stricmp)
			{
				return stricmp(p->AsFixedString(data1), v.AsString());
			}
			else
			{
				return strcmp(p->AsFixedString(data1), v.AsString());
			}
			break;

		case PT_String:
			if (p->flag & PF_Stricmp)
			{
				return stricmp(p->AsString(data1), v.AsString());
			}
			else
			{
				return strcmp(p->AsString(data1), v.AsString());
			}
			break;
		}

		d_assert (0);

		return 0;
	}

	//
	ImplementRTTI(rsql_invoke, rsql_exp);

	rsql_invoke::~rsql_invoke()
	{
		for (int i = 0; i < _args.Size(); ++i)
		{
			delete _args[i];
		}
	}

	bool rsql_invoke::match(Token::iterator i, Token::iterator end, Token::iterator & r, rsql_exp * parent)
	{
		if (i->type() != 0)
			return false;

		rsql_get_compiler();

		Token::iterator beg = i;

		i = i.next();
		if (i < end && i.valid() && i->type() == rsql_operator::op_left_b)
		{
			int left_b = 0;
			bool matched = false;

			while (i.valid())
			{
				if (i->type() == rsql_operator::op_left_b)
				{
					left_b += 1;
				}
				else if (i->type() == rsql_operator::op_right_b)
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
				compiler.set_error("Error: invoke expression.");
				return false;
			}

			rdb_function pfn = compiler.get_function(beg->str);
			if (pfn == NULL)
			{
				compiler.set_error("Error: function not found.");
				return false;
			}

			rsql_invoke * e = new rsql_invoke;
			e->_pfn = pfn;

			beg = beg.next().next();
			end = i;
			i = beg;

			left_b = 0;
			while (i < end)
			{
				if (i->type() == rsql_operator::op_left_b)
				{
					left_b += 1;
				}
				else if (i->type() == rsql_operator::op_right_b)
				{
					left_b -= 1;
				}

				// ·Ö¸î·ûºÅ
				if (left_b == 0 && i->type() == rsql_operator::op_comma)
				{
					e->parseArg(beg, i);
					beg = i.next();
				}

				i = i.next();
			}

			if (beg < i)
			{
				e->parseArg(beg, i);
			}

			parent->append(e);

			r = end.next();

			return true;
		}

		return false;
	}

	void rsql_invoke::parseArg(Token::iterator i, Token::iterator end)
	{
		rsql_get_compiler();

		if (_args.Size() > RSQL_MAX_ARGS)
		{
			compiler.set_error("Error: function args too mush.");
			return ;
		}

		rsql_exp * e = new rsql_exp;
		e->parse(i, end);

		_args.PushBack(e);
	}

	void rsql_invoke::build()
	{
		for (int i = 0; i < _args.Size(); ++i)
		{
			_args[i]->build();
		}
	}

	void rsql_invoke::execute(Array<rdb_value> & results)
	{
		rsql_get_compiler();

		Array<rdb_value> args;
		for (int i = 0; i < _args.Size(); ++i)
		{
			_args[i]->execute(args);
		}

		if (_args.Size() != args.Size())
		{
			compiler.set_error("Error: function args.");
			return ;
		}

		rdb_vlist varlist;
		for (int i = 0; i < args.Size(); ++i)
		{
			varlist.PushBack(args[i]);
		}

		rdb_value v;
		if (!_pfn(compiler.i_record, &v, &varlist))
		{
			compiler.set_error("Error: function execute.");
		}

		results.PushBack(v);
	}

	//
	ImplementRTTI(rsql_operator, rsql_exp);

	bool rsql_operator::match(Token::iterator i, Token::iterator end, Token::iterator & r, rsql_exp * parent)
	{
		rsql_get_compiler();

		if (i->type() != 0)
		{
			Token::iterator n = i.next();
			enum_t _op = (enum_t)i->type();

			if (_op == op_left_b)
			{
				if (n < end)
				{
					if (n->type() != op_left_b &&
						n->type() != op_sub &&
						n->type() != 0)
					{
						compiler.set_error("Error: expression.");
					}
				}
				else
				{
					compiler.set_error("Error: expression.");
				}
			}
			else if (_op == op_right_b)
			{
				if (n < end)
				{
					if (n->type() != op_left_b && n->type() == 0)
					{
						compiler.set_error("Error: expression.");
					}
				}
			}
			else if (_op == op_sub)
			{
				if (parent->_last_child == NULL)
				{
					_op = op_neg;

					if (n < end)
					{
						if (n->type() != 0 && n->type() != op_left_b)
						{
							compiler.set_error("Error: expression.");
						}
					}
					else
					{
						compiler.set_error("Error: expression.");
					}
				}
				else if (TYPE_OF(rsql_operator, parent->_last_child))
				{
					enum_t last_op = static_cast<rsql_operator *>(parent->_last_child)->_op;
					if (last_op != op_right_b)
					{
						_op = op_neg;
					}

					if (n < end)
					{
						if (_op == op_neg)
						{
							if (n->type() != 0 && n->type() != op_left_b)
							{
								compiler.set_error("Error: expression.");
							}
						}
						else
						{
							if (n->type() != 0 && n->type() != op_left_b && n->type() != op_sub)
							{
								compiler.set_error("Error: expression.");
							}
						}
					}
					else
					{
						compiler.set_error("Error: expression.");
					}
				}
			}
			else
			{
				if (n < end)
				{
					if (n->type() != 0 && n->type() != op_sub && n->type() != op_left_b)
					{
						compiler.set_error("Error: expression.");
					}
				}
				else
				{
					compiler.set_error("Error: expression.");
				}
			}

			if (!compiler.is_error())
			{
				r = i.next();

				rsql_operator * e = new rsql_operator;
				e->_op = _op;
				e->_priority = get_priority(_op);
				parent->append(e);

				return true;
			}
		}

		return false;
	}

	int rsql_operator::get_priority(int _op)
	{
		switch (_op)
		{
		case op_neg:
			return 10;

		case op_less:
		case op_greater:
		case op_less_equal:
		case op_greater_equal:
		case op_not_equal:
		case op_equal_equal:
			return 3;

		case op_and_and:
		case op_or_or:
			return 2;

		case op_equal:
		case op_add_equal:
		case op_sub_equal:
		case op_mul_equal:
		case op_div_equal:
			return 1;

		case op_and:
		case op_or:
		case op_add:
		case op_sub:
			return 4;
		case op_mul:
		case op_div:
			return 5;

		case op_left_b:
		case op_right_b:
		case op_comma:
			return 0;
		}

		return 0;
	}

	void rsql_operator::execute(Array<rdb_value> & results)
	{
		rsql_get_compiler();

		rdb_value v;

		if (_op != op_neg)
		{
			if (results.Size() >= 2)
			{
				rdb_value p1 = results[results.Size() - 2];
				rdb_value p2 = results[results.Size() - 1];

				results.Erase(results.Size() - 2, 2);

				if (_op == op_less || 
					_op == op_greater || 
					_op == op_less_equal || 
					_op == op_greater_equal ||
					_op == op_not_equal || 
					_op == op_equal_equal || 
					_op == op_and_and || 
					_op == op_or_or)
				{
					v = _do_compare(p1, p2);
				}
				else if (
					_op == op_equal || 
					_op == op_add_equal || 
					_op == op_sub_equal || 
					_op == op_mul_equal || 
					_op == op_div_equal)
				{
					v = _do_assignment(p1, p2);
				}
				else if (
					_op == op_add || 
					_op == op_sub || 
					_op == op_mul || 
					_op == op_div || 
					_op == op_and || 
					_op == op_or)
				{
					v = _do_arithmetic(p1, p2);
				}
			}
			else
			{
				compiler.set_error("Error: expression.");
			}
		}
		else
		{
			if (results.Size() >= 1)
			{
				rdb_value p1 = results[results.Size() - 1];
				results.Erase(results.Size() - 1, 1);

				v = -p1;
			}
		}

		if (v.Type() != rdb_value::TYPE_VOID)
		{
			results.PushBack(v);
		}
		else
		{
			compiler.set_error("Error: expression.");
		}
	}

	rdb_value rsql_operator::_do_compare(rdb_value & p1, rdb_value & p2)
	{
		if (p1.Type() != p2.Type())
		{
			if (p1.Type() < p2.Type())
			{
				p1 = p1.UpType(p2.Type());
			}
			else
			{
				p2 = p2.UpType(p1.Type());
			}
		}

		rdb_value v;

		if (p1.Type() == p2.Type())
		{
			if (_op == op_less)
			{
				v = p1 < p2;
			}
			else if (_op == op_less_equal)
			{
				v = p1 <= p2;
			}
			else if (_op == op_greater)
			{
				v = p1 > p2;
			}
			else if (_op == op_greater_equal)
			{
				v = p1 >= p2;
			}
			else if (_op == op_not_equal)
			{
				v = p1 != p2;
			}
			else if (_op == op_equal_equal)
			{
				v = p1 == p2;
			}
			else if (_op == op_and_and)
			{
				v = p1 && p2;
			}
			else if (_op == op_or_or)
			{
				v = p1 || p2;
			}
		}

		return v;
	}

	rdb_value rsql_operator::_do_arithmetic(rdb_value & p1, rdb_value & p2)
	{
		if (p1.Type() != p2.Type())
		{
			if (p1.Type() < p2.Type())
			{
				p1 = p1.UpType(p2.Type());
			}
			else
			{
				p2 = p2.DownType(p1.Type());
			}
		}

		rdb_value v;

		if (p1.Type() == p2.Type())
		{
			switch (_op)
			{
			case op_add:
				v = p1 + p2;
				break;
			case op_sub:
				v = p1 - p2;
				break;
			case op_mul:
				v = p1 * p2;
				break;
			case op_div:
				v = p1 / p2;
				break;
			case op_and:
				v = p1 & p2;
				break;
			case op_or:
				v = p1 | p2;
				break;
			}
		}

		return v;
	}

	rdb_value rsql_operator::_do_assignment(rdb_value & p1, rdb_value & p2)
	{
		if (p1.Type() != p2.Type())
		{
			if (p1.Type() > p2.Type())
			{
				p2 = p2.UpType(p1.Type());
			}
			else
			{
				p2 = p2.DownType(p1.Type());
			}
		}

		rdb_value v;

		if (p1.Type() == p2.Type())
		{
			if (_op == op_equal)
			{
				p1 = p2;
			}
			else if (_op == op_add_equal)
			{
				p1 += p2;
			}
			else if (_op == op_sub_equal)
			{
				p1 -= p2;
			}
			else if (_op == op_mul_equal)
			{
				p1 *= p2;
			}

			v = p1;
		}

		return v;
	}

	//
	ImplementSingleton(rsql_engine);

	rsql_engine::rsql_engine()
	{
		i_exp = NULL;

		i_token = new Token;
		i_token->AddKey(rsql_operator::op_less, "<");
		i_token->AddKey(rsql_operator::op_greater, ">");
		i_token->AddKey(rsql_operator::op_less_equal, "<=");
		i_token->AddKey(rsql_operator::op_greater_equal, ">=");
		i_token->AddKey(rsql_operator::op_not_equal, "!=");
		i_token->AddKey(rsql_operator::op_equal_equal, "==");
		i_token->AddKey(rsql_operator::op_and_and, "&&");
		i_token->AddKey(rsql_operator::op_or_or, "||");

		i_token->AddKey(rsql_operator::op_equal, "=");
		i_token->AddKey(rsql_operator::op_add_equal, "+=");
		i_token->AddKey(rsql_operator::op_sub_equal, "-=");
		i_token->AddKey(rsql_operator::op_mul_equal, "*=");
		i_token->AddKey(rsql_operator::op_div_equal, "/=");
		//i_token->AddKey(rsql_operator::op_mod_equal, "%=");

		i_token->AddKey(rsql_operator::op_add, "+");
		i_token->AddKey(rsql_operator::op_sub, "-");
		i_token->AddKey(rsql_operator::op_mul, "*");
		i_token->AddKey(rsql_operator::op_div, "/");
		i_token->AddKey(rsql_operator::op_and, "&");
		i_token->AddKey(rsql_operator::op_or, "|");
		//i_token->AddKey(rsql_operator::op_mod, "%");

		i_token->AddKey(rsql_operator::op_left_b, "(");
		i_token->AddKey(rsql_operator::op_right_b, ")");
		i_token->AddKey(rsql_operator::op_comma, ",");

		//
		reg_function("rdb:property", rdb_property);
		reg_function("rdb:typeof", rdb_typeof);
		reg_function("rdb:kindof", rdb_kindof);
		reg_function("rdb:count", rdb_count);
		reg_function("rdb:index", rdb_index);
		reg_function("rdb:offset", rdb_offset);
		reg_function("rdb:orderby", rdb_orderby);
	}

	rsql_engine::~rsql_engine()
	{
		d_assert (i_exp == NULL);
		delete i_token;
	}

	void rsql_engine::set_error(const char * fmt, ...)
	{
		if (!is_error())
		{
			va_list arglist;

			va_start(arglist, fmt);
			vsprintf(i_error, fmt, arglist);
			va_end(arglist);
		}
	}

	const char * rsql_engine::get_error()
	{
		return i_error;
	}

	bool rsql_engine::is_error()
	{
		return i_error[0] != 0;
	}

	void rsql_engine::reg_function(const char * name, rdb_function pfn)
	{
		int crc32 = Math::Crc32(name);

#ifdef M_DEBUG
		int i = i_functions.Find(crc32);
		if (i != -1)
		{
			d_assert(0 && "function exist!");
		}
#endif
		i_functions.Insert(crc32, pfn);
	}

	rdb_function rsql_engine::get_function(const char * name)
	{
		int i = Math::Crc32(name);
		i = i_functions.Find(i);

		return i != -1 ? i_functions[i].value : NULL;
	}

	void rsql_engine::begin()
	{
		i_error[0] = 0;

		i_m = 0;
		i_op = 0;
		i_args = NULL;
		i_table = NULL;
		i_where = NULL;
		i_option = NULL;
		i_record = NULL;
		i_exp = NULL;

		i_first = 0;
		i_last = -1;

		option_count = 0;
		option_offset = 0;
		option_indexed[0] = false;
		option_indexed[1] = false;
		option_indexed[2] = false;
		option_indexed[3] = false;
		option_orderby.Clear();
	}

	void rsql_engine::end()
	{
		i_m = 0;
		i_op = 0;
		i_args = NULL;
		i_table = NULL;
		i_where = NULL;
		i_option = NULL;
		i_record = NULL;
		safe_delete(i_exp);

		i_token->Clear();
	}

	static const char * RSQL_QUERY_STR[rsql_engine::RSQL_QUERY_MAX] = {
		"UNKNOWN",
		
		"SELECT",
		"INSERT",
		"DELETE",
		"UPDATE",

		"CREATE_USER",
		"DELETE_USER",
		"ALTER_USER",

		"CREATE_TABLE",
		"DELETE_TABLE",
		"OPTIMIZE_TABLE",
	};

	int rsql_engine::compile(const char * rsql, int m)
	{
		int len = strlen(rsql);
		i_buffer.Resize(len + 1);
		strcpy(&i_buffer[0], rsql);

		i_m = m;

		char cmd[128];
		char * str = i_buffer.c_ptr();

		str = (char *)str_substring(cmd, 128, str);
		for (int i = 1; i < RSQL_QUERY_MAX; ++i)
		{
			if (strcmp(RSQL_QUERY_STR[i], cmd) == 0)
			{
				i_op = i;
				break;
			}
		}

		if (i_op == 0)
		{
			set_error("Unknown query statement.");
			return -1;
		}

		if (i_op >= rsql_engine::RSQL_CREATE_USER)
		{
			i_args = str;
			return i_args != NULL ? 0 : -1;
		}

		str = (char *)str_substring(cmd, 128, str);
		i_table = (rdb_table *)rdb_getTable(cmd);
		if (i_table == NULL)
		{
			set_error("Table not exist.");
			return -1;
		}
		else if (i_m < i_table->i_mode || (i_op > 1 && i_m < RDB_MODE_WRITE))
		{
			return -255;
		}

		i_first = 0;
		i_last = i_table->i_btree.Size() - 1;
		option_count = i_table->i_btree.Size();
		option_offset = 0;

		i_where = (char *)strstr(str, "WHERE");
		i_option = (char *)strstr(str, "OPTION");

		if (i_where)
		{
			*(i_where + 0) = 0;
			*(i_where + 1) = 0;
			*(i_where + 2) = 0;
			*(i_where + 3) = 0;
			*(i_where + 4) = 0;
			i_where += 5;
		}

		if (i_option)
		{
			*(i_option + 0) = 0;
			*(i_option + 1) = 0;
			*(i_option + 2) = 0;
			*(i_option + 3) = 0;
			*(i_option + 4) = 0;
			*(i_option + 5) = 0;
			i_option += 6;
		}

		str = str_skipwhite(str);
		if (*str != 0)
		{
			set_error("script error.");
			return -1;
		}

		if (i_option != NULL)
		{
			i_token->Parse(i_option);

			i_exp = new rsql_exp;

			i_exp->parse(i_token->Begin(), i_token->End());
			i_exp->build();

			if (!is_error())
			{
				Array<rdb_value> result;
				i_exp->execute(result);
			}

			delete i_exp;
			i_exp = NULL;
		}

		if (!is_error() && i_where)
		{
			i_token->Parse(i_where);

			i_exp = new rsql_exp;
			i_exp->parse(i_token->Begin(), i_token->End());

			i_exp->build();
			
			//r_build_index();
		}

		return !is_error() ? 0 : -1;
	}

	void rsql_engine::r_build_index()
	{
		int i = 0;
		rsql_exp * e = i_exp->_first_child;

		while (i < i_table->i_primaryks.Size())
		{
			if (!TYPE_OF(rsql_property, e) ||
				!TYPE_OF(rsql_operator, e->_next_sibling) ||
				!TYPE_OF(rsql_constant, e->_next_sibling->_next_sibling))
				break;

			rsql_property * pk = (rsql_property *)e;
			if (pk->_prop != i_table->i_primaryks[i])
				break;

			// check type
			int type = rdb_value::TYPE_VOID;
			switch (pk->_prop->type)
			{
			case PT_Int:
				type = (pk->_prop->flag & PF_Unsigned) ? rdb_value::TYPE_UINT : rdb_value::TYPE_INT;
				break;

			case PT_Float:
				type = rdb_value::TYPE_REAL;
				break;

			case PT_FixedString:
				type = rdb_value::TYPE_STRING;
				break;

			case PT_String:
				type = rdb_value::TYPE_STRING;
				break;
			}

			d_assert (type != rdb_value::TYPE_VOID);

			// check operator
			rsql_operator * op = (rsql_operator *)e->_next_sibling;
			if (op->_op != rsql_operator::op_equal_equal &&
				op->_op != rsql_operator::op_less &&
				op->_op != rsql_operator::op_equal_equal &&
				op->_op != rsql_operator::op_greater &&
				op->_op != rsql_operator::op_greater_equal)
				break;

			// set index value (min or max)
			rsql_constant * cn = (rsql_constant *)e->_next_sibling->_next_sibling;

			rdb_value v = cn->_var;
			if (v.Type() != type)
			{
				v = v.Type() > type ? v.DownType(type) : v.UpType(type);
			}

			if (v.Type() == rdb_value::TYPE_VOID)
				break;

			if (op->_op == rsql_operator::op_less || op->_op == rsql_operator::op_less_equal)
				option_vmax[i] = v;
			else if (op->_op == rsql_operator::op_greater || op->_op == rsql_operator::op_greater_equal)
				option_vmin[i] = v;
			else
				option_vmin[i] = option_vmax[i] = v;

			e = cn->_next_sibling;
			if (e == NULL)
			{
				option_indexed[i] = true;
				break;
			}

			if (!TYPE_OF(rsql_operator, e))
				break;

			// only && enable index search
			rsql_operator * lop = (rsql_operator *)e;
			if (lop->_op != rsql_operator::op_and_and)
				break;

			e = lop->_next_sibling;
			if (!TYPE_OF(rsql_property, e))
				break;

			option_indexed[i] = true;

			// if next property != current, jump to next index
			pk = (rsql_property *)e;
			if (pk->_prop != i_table->i_primaryks[i])
				++i;
		}
	}

	int rsql_engine::execute(IObject * obj, rdb_collection * collection)
	{
		d_assert (!is_error());

		int ret = -1;

		if (i_table != NULL)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (!option_indexed[i])
					break;

				i_table->search(i_first, i_last, i, option_vmin[i], option_vmax[i]);
			}
		}

		i_first += option_offset;

		switch (i_op)
		{
		case RSQL_SELECT:
			ret = r_select(collection);
			break;

		case RSQL_INSERT:
			ret = r_insert(obj);
			break;

		case RSQL_DELETE:
			ret = r_delete();
			break;

		case RSQL_UPDATE:
			ret = r_update(obj);
			break;

		case RSQL_CREATE_USER:
			ret = r_createuser();
			break;

		case RSQL_DELETE_USER:
			ret = r_deleteuser();
			break;

		case RSQL_ALTER_USER:
			ret = r_alteruser();
			break;

		case RSQL_CREATE_TABLE:
			ret = r_createtable();
			break;

		case RSQL_DELETE_TABLE:
			ret = r_deletetable();
			break;

		case RSQL_OPTIMIZE_TABLE:
			ret = r_optimizetable();
			break;
		}

		return ret;
	}

	bool rsql_do()
	{
		d_assert (rsql_engine::Instance()->i_exp != NULL);

		Array<rdb_value> result;
		rsql_engine::Instance()->i_exp->execute(result);

		if (!rsql_engine::Instance()->is_error() &&
			result.Size() == 1 && result[0].Type() == rdb_value::TYPE_INT)
		{
			return result[0].AsInt() != 0;
		}

		return false;
	}

	struct rsql_orderby_cmp
	{
		int operator()(IObject * o1, IObject * o2)
		{
			const Property * p = rsql_engine::Instance()->option_orderby[0];

			int cmp = rsql_property::compare(p, o1, o2);
			if (cmp != 0)
				return cmp;

			if (rsql_engine::Instance()->option_orderby.Size() > 1)
			{
				p = rsql_engine::Instance()->option_orderby[1];

				cmp = rsql_property::compare(p, o1, o2);
				if (cmp != 0)
					return cmp;

				if (rsql_engine::Instance()->option_orderby.Size() > 2)
				{
					p = rsql_engine::Instance()->option_orderby[2];

					cmp = rsql_property::compare(p, o1, o2);
					if (cmp != 0)
						return cmp;

					if (rsql_engine::Instance()->option_orderby.Size() > 3)
					{
						p = rsql_engine::Instance()->option_orderby[3];

						cmp = rsql_property::compare(p, o1, o2);
						if (cmp != 0)
							return cmp;
					}
				}
			}

			return true;
		}
	};

	int rsql_engine::r_select(rdb_collection * collection)
	{
		rdb_table * table = (rdb_table *)i_table;

		int count = 0;
		if (i_exp != NULL)
		{
			for (int i = i_first; i <= i_last && count < option_count; ++i)
			{
				i_record = table->record(i)->object;

				if (rsql_do())
				{
					if (collection != NULL)
						collection->PushBack(i_record);

					table->record(i)->i_priority += 1;

					++count;
				}

				if (is_error())
					break;
			}
		}
		else
		{
			for (int i = i_first; i <= i_last && count < option_count; ++i)
			{
				if (collection != NULL)
					collection->PushBack(table->record(i)->object);

				++count;
			}
		}

		if (option_orderby.Size() > 0 && count > 0)
		{
			rsql_orderby_cmp op;
			Sort(collection->c_ptr(), count, op);
		}

		return count;
	}

	int rsql_engine::r_insert(IObject * obj)
	{
		d_assert (obj != NULL);

		if (!obj->IsKindOf(i_table->i_object->GetRTTI()))
		{
			set_error("type error");
			return -1;
		}

		int blockId = i_table->b_query_block();
		if (blockId == -1)
		{
			set_error("query block error.");
			return -1;
		}

		rdb_record record;
		record.i_blockid = blockId;
		record.i_offset = 0;
		record.i_priority = 0;
		record.object = obj;

		if (i_table->i_primaryks.Size() > 0 && i_table->check_unique(record) != -1)
		{
			set_error("primary unique check failed.");
			return -1;
		}

		i_table->insert(record);

		return 1;
	}

	int rsql_engine::r_delete()
	{
		int count = 0;

		if (i_exp != NULL)
		{
			for (int i = i_first; i <= i_last && count < option_count; ++i)
			{
				i_record = i_table->record(i)->object;

				if (rsql_do())
				{
					i_table->remove(i--);
					--i_last;
					++count;
				}

				if (is_error())
					break;
			}
		}
		else
		{
			for (int i = i_first; i <= i_last && count < option_count; ++i)
			{
				i_record = i_table->record(i)->object;

				i_table->remove(i--);
				--i_last;
				++count;
			}
		}

		if (count > 0)
		{
			i_table->save();
		}

		return count;
	}

	int rsql_engine::r_update(IObject * obj)
	{
		d_assert (obj != NULL);

		if (!obj->IsKindOf(i_table->i_object->GetRTTI()))
		{
			set_error("type error");
			return -1;
		}

		rdb_record record;
		record.i_blockid = 0;
		record.i_offset = 0;
		record.i_priority = 0;
		record.object = obj;
		
		if (i_table->i_primaryks.Size() > 0)
		{
			if (i_last > i_first)
			{
				set_error("table has primary key, it can't update more");
				return -1;
			}

			int check = i_table->check_unique(record);
			if (check != -1 && check != i_first)
			{
				set_error("primary unique check failed.");
				return -1;
			}
		}

		int count = 0;

		if (i_exp != NULL)
		{
			for (int i = i_first; i <= i_last && count < option_count; ++i)
			{
				i_record = i_table->record(i)->object;

				if (rsql_do())
				{
					i_table->update(i, record);

					++count;
				}

				if (is_error())
					break;
			}
		}
		else
		{
			for (int i = i_first; i <= i_last && count < option_count; ++i)
			{
				i_record = i_table->record(i)->object;

				i_table->update(i, record);

				++count;
			}
		}

		if (count > 0)
		{
			i_table->save();
		}

		return count;
	}

	int rsql_engine::r_createuser()
	{
		const char * str = i_args;
		FixedString32 username;
		FixedString32 password;
		FixedString32 mode;

		str = str_substring(username.c_str(), 32, str);
		str = str_substring(password.c_str(), 32, str);
		str = str_substring(mode.c_str(), 32, str);

		if (i_m < RDB_MODE_SUPPER || i_m <= mode.ToInt())
			return -255;

		return rdb_createUser(username.c_str(), password.c_str(), mode.ToInt());
	}

	int rsql_engine::r_deleteuser()
	{
		const char * str = i_args;
		FixedString32 username;

		str = str_substring(username.c_str(), 32, str);

		if (i_m < RDB_MODE_SUPPER)
			return -255;

		return rdb_deleteUser(username.c_str());
	}

	int rsql_engine::r_alteruser()
	{
		const char * str = i_args;
		FixedString32 username;
		FixedString32 password;
		FixedString32 mode;

		str = str_substring(username.c_str(), 32, str);

		while (*str)
		{
			FixedString32 cmd;
			str = str_substring(cmd.c_str(), 32, str);

			if (cmd == "password")
			{
				str = str_substring(password.c_str(), 32, str);
			}
			else if (cmd == "mode")
			{
				str = str_substring(mode.c_str(), 32, str);
			}
		}

		if (i_m < RDB_MODE_SUPPER || i_m <= mode.ToInt() || i_m < rdb_getUserMode(username.c_str()))
			return -255;

		return rdb_alterUser(username.c_str(), password.c_str(), mode.ToInt());
	}

	int rsql_engine::r_createtable()
	{
		const char * str = i_args;
		FixedString32 dbname;
		FixedString32 tablename;
		FixedString32 classname;
		FixedString32 mode;

		str = str_substring(dbname.c_str(), 32, str);
		str = str_substring(tablename.c_str(), 32, str);
		str = str_substring(classname.c_str(), 32, str);
		str = str_substring(mode.c_str(), 32, str);

		if (i_m < RDB_MODE_SUPPER || i_m < mode.ToInt())
			return -255;

		return rdb_createTable(dbname.c_str(), tablename.c_str(), classname.c_str(), mode.ToInt());
	}

	int rsql_engine::r_deletetable()
	{
		const char * str = i_args;
		FixedString32 dbname;
		FixedString32 tablename;

		str = str_substring(dbname.c_str(), 32, str);
		str = str_substring(tablename.c_str(), 32, str);

		rdb_table * table = (rdb_table *)rdb_getTable(tablename.c_str());
		if (table == NULL)
		{
			set_error("table not exist");
			return -1;
		}

		if (i_m < RDB_MODE_SUPPER || i_m < table->i_mode)
			return -255;

		return rdb_deleteTable(dbname.c_str(), tablename.c_str());
	}

	int rsql_engine::r_optimizetable()
	{
		const char * str = i_args;
		FixedString32 dbname;
		FixedString32 tablename;

		str = str_substring(dbname.c_str(), 32, str);
		str = str_substring(tablename.c_str(), 32, str);

		rdb_table * table = (rdb_table *)rdb_getTable(tablename.c_str());
		if (table == NULL)
		{
			set_error("table not exist");
			return -1;
		}

		if (i_m < RDB_MODE_SUPPER || i_m < table->i_mode)
			return -255;

		return rdb_optimizeTable(dbname.c_str(), tablename.c_str());
	}

}
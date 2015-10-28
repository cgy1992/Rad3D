#include "RadCCompiler.h"
#include "RadCType.h"
#include "RadCStatement.h"

namespace Rad {
	//
	radc_program::radc_program()
	{
	}

	radc_program::~radc_program()
	{
		for (int i = 0; i < varlist.Size(); ++i)
		{
			delete varlist[i];
		}

		for (int i = 0; i < statements.Size(); ++i)
		{
			delete statements[i];
		}
	}

	//
	void radc_compiler::set_error(const char * fmt, ...)
	{
		if (!is_error())
		{
			char text[256];
			va_list arglist;

			va_start(arglist, fmt);
			vsprintf(text, fmt, arglist);
			va_end(arglist);

			error = text;
		}
	}

	void radc_compiler::clear_error()
	{
		error[0] = 0;
	}

	bool radc_compiler::is_error()
	{
		return error[0] != 0;
	}

	bool radc_compiler::is_break()
	{
		return _break;
	}

	ImplementSingleton(radc_compiler);

	radc_compiler::radc_compiler()
			: inputlist(NULL)
			, outputlist(NULL)
	{
		error_reporter = NULL;
	}

	void radc_compiler::init()
	{
		token = new Token;
		token->AddKey(radc_enum::op_less, "<");
		token->AddKey(radc_enum::op_greater, ">");
		token->AddKey(radc_enum::op_less_equal, "<=");
		token->AddKey(radc_enum::op_greater_equal, ">=");
		token->AddKey(radc_enum::op_not_equal, "!=");
		token->AddKey(radc_enum::op_equal_equal, "==");
		token->AddKey(radc_enum::op_and_and, "&&");
		token->AddKey(radc_enum::op_or_or, "||");

		token->AddKey(radc_enum::op_equal, "=");
		token->AddKey(radc_enum::op_add_equal, "+=");
		token->AddKey(radc_enum::op_sub_equal, "-=");
		token->AddKey(radc_enum::op_mul_equal, "*=");
		token->AddKey(radc_enum::op_div_equal, "/=");
		//token->AddKey(radc_enum::op_mod_equal, "%=");

		token->AddKey(radc_enum::op_add, "+");
		token->AddKey(radc_enum::op_sub, "-");
		token->AddKey(radc_enum::op_mul, "*");
		token->AddKey(radc_enum::op_div, "/");
		//token->AddKey(radc_enum::op_mod, "%");

		token->AddKey(radc_enum::op_left_b, "(");
		token->AddKey(radc_enum::op_right_b, ")");
		token->AddKey(radc_enum::op_comma, ",");

		//
		outputlist = new radc_varlist;

		buffer.Resize(1024);

		for (int i = 0; i < 256; ++i)
		{
			charmap[i] = i;
		}

		charmap['\r'] = ' ';
		charmap['\t'] = ' ';
	}

	void radc_compiler::shutdown()
	{
		delete outputlist;

		for (int i = 0; i < global_varlist.Size(); ++i)
		{
			delete global_varlist[i];
		}
		global_varlist.Clear();

		buffer.Clear();
	
		libs.Clear();
		functions.Clear();

		delete token;
	}

	void radc_compiler::begin(const char * str, const radc_varlist * _varlist)
	{
		clear_error();

		_break = false;
		_options = 0;
		_cache_program = programs.Find(str);
		if (_cache_program == -1)
		{
			int length = strlen(str);

			buffer.Resize(length + 1);

			int i = 0;
			while (*str)
			{
				buffer[i++] = charmap[*str++];
			}

			buffer[i] = 0;
		}

		inputlist = _varlist;

		outputlist->clear();
	}

	void radc_compiler::end(const char * str)
	{
		inputlist = NULL;

		if (_cache_program != -1)
		{
			varlist.Clear();
			statements.Clear();
		}
		else if (!Root::InEditor() && (_options & RADC_OPATION_CACHE))
		{
			radc_program * p = new radc_program;
			p->varlist = varlist;
			p->statements = statements;
			
			programs.Insert(str, p);

			varlist.Clear();
			statements.Clear();
		}
		else
		{
			for (int i = 0; i < varlist.Size(); ++i)
			{
				delete varlist[i];
			}
			varlist.Clear();

			for (int i = 0; i < statements.Size(); ++i)
			{
				delete statements[i];
			}
			statements.Clear();
		}
	}

	radc_var * radc_compiler::decl_variable(const char * name, radc_enum type, bool is_static)
	{
		if (get_variable(name) == NULL)
		{
			radc_var * v = new radc_var(name);

			switch (type)
			{
			case radc_enum::type_real:
				v->setReal(0);
				break;

			case radc_enum::type_string:
				v->setString("");
				break;

			case radc_enum::type_ptr:
				v->setPtr(NULL);
				break;
			}

			if (is_static)
			{
				global_varlist.PushBack(v);
			}
			else
			{
				varlist.PushBack(v);
			}

			return v;
		}

		return 0;
	}

	radc_var * radc_compiler::get_variable(const char * name)
	{
		for (int i = 0; i < varlist.Size(); ++i)
		{
			if (strcmp(varlist[i]->getName(), name) == 0)
			{
				return varlist[i];
			}
		}

		for (int i = 0; i < global_varlist.Size(); ++i)
		{
			if (strcmp(global_varlist[i]->getName(), name) == 0)
			{
				return global_varlist[i];
			}
		}

		return NULL;
	}

	void radc_compiler::reg_function(const char * name, const radc_reg & reg)
	{
		int crc32 = Math::Crc32(name);

#ifdef M_DEBUG
		int i = functions.Find(crc32);
		if (i != -1)
		{
			radc_reg reg = functions[i].value;
			d_assert(0 && "function exist!");
		}
#endif

		functions.Insert(crc32, reg);
	}

	radc_function radc_compiler::get_function(const char * name)
	{
		int i = functions.Find(Math::Crc32(name));

		return i != -1 ? functions[i].value.pfn : NULL;
	}

	void radc_compiler::compile(int options)
	{
		if (_cache_program != -1)
		{
			varlist = programs[_cache_program].value->varlist;
			statements = programs[_cache_program].value->statements;

			return ;
		}

		char * str = buffer.c_ptr();
		int line = 1;
		int depth = 0;

		_options = options;

		while (*str && !is_error())
		{
			str = str_skip(str, ' ');

			if (*str == '#') // comment
			{
				while (*str && *str != '\n')
					++str;

				if (*str == '\n')
					++str;

				continue;
			}

			char * line_str = str;
			int line_length = 0;

			while (*str && *str != '\n')
			{
				++str;
				++line_length;
			}

			if (*str == '\n')
			{
				*str = 0;
				++str;
			}

			str_trim(line_str, line_length);
			if (*line_str)
			{
				const char * matched_str = NULL;
				radc_stat * stat = NULL;
				bool insert_empty = false;

				do
				{
					matched_str = str_match(line_str, "function");
					if (matched_str && (*matched_str == ' ' || *matched_str == '\0'))
					{
						matched_str = str_skip(matched_str, ' ');

						matched_str = str_match(matched_str, "main");
						if (matched_str)
						{
							stat = new rstat_entry;
							stat->str = str_skip(matched_str, ' ');
						}
						else
						{
							set_error("Error: [%d] - main.", line);
						}

						depth += 1;

						break;
					}

					matched_str = str_match(line_str, "if");
					if (matched_str && (*matched_str == ' ' || *matched_str == '\0'))
					{
						stat = new rstat_if;
						stat->str = str_skip(matched_str, ' ');
						insert_empty = true;
						depth += 1;

						break;
					}

					matched_str = str_match(line_str, "else");
					if (matched_str && (*matched_str == ' ' || *matched_str == '\0'))
					{
						matched_str = str_skip(matched_str, ' ');

						const char * matched_str2 = str_match(matched_str, "if");
						if (matched_str2 && (*matched_str2 == ' ' || *matched_str2 == '\0'))
						{
							stat = new rstat_elseif;
							stat->str = str_skip(matched_str2, ' ');
							insert_empty = true;
						}
						else
						{
							stat = new rstat_else;
							stat->str = str_skip(matched_str, ' ');
							insert_empty = true;
						}

						break;
					}

					matched_str = str_match(line_str, "end");
					if (matched_str  && (*matched_str == ' ' || *matched_str == '\0'))
					{
						stat = new rstat_end;
						stat->str = str_skip(matched_str, ' ');
						depth -= 1;

						break;
					}

					matched_str = str_match(line_str, "while");
					if (matched_str  && (*matched_str == ' ' || *matched_str == '\0'))
					{
						stat = new rstat_while;
						stat->str = str_skip(matched_str, ' ');
						insert_empty = true;
						depth += 1;

						break;
					}

					matched_str = str_match(line_str, "return");
					if (matched_str  && (*matched_str == ' ' || *matched_str == '\0'))
					{
						stat = new rstat_return;
						stat->str = str_skip(matched_str, ' ');

						break;
					}

					stat = new rstat_exp;
					stat->str = str_skip(line_str, ' ');

				} while (0);
				
				if (stat != NULL)
				{
					stat->line = line;
					statements.PushBack(stat);

					if (insert_empty)
					{
						statements.PushBack(new rstat_empty);
					}
				}
			}

			++line;
		}

		if (!is_error() && depth != 0)
		{
			set_error("Error: end don't macthed.");
		}

		// if Ìø×ª
		for (int i = 0; i < statements.Size(); ++i)
		{
			if (TYPE_OF(rstat_if, statements[i]) ||
				TYPE_OF(rstat_elseif, statements[i]) ||
				TYPE_OF(rstat_else, statements[i]))
			{
				int in_if = 0, end_if = 0;

				for (int j = i + 1; j < statements.Size(); ++j)
				{
					if (TYPE_OF(rstat_elseif, statements[j]) ||
						TYPE_OF(rstat_else, statements[j]) ||
						TYPE_OF(rstat_end, statements[j]))
					{
						if (in_if == 0)
						{
							end_if = j - 1;
							statements[i]->jump = j;
							break;
						}
					}

					if (TYPE_OF(rstat_if, statements[j]) ||
						TYPE_OF(rstat_while, statements[j]))
					{
						in_if += 1;
					}

					if (TYPE_OF(rstat_end, statements[j]))
					{
						in_if -= 1;
					}
				}

				d_assert (end_if > i);

				in_if = 0;
				for (int j = end_if + 1; j < statements.Size(); ++j)
				{
					if (TYPE_OF(rstat_end, statements[j]))
					{
						if (in_if == 0)
						{
							statements[end_if]->jump = j;
							break;
						}
					}

					if (TYPE_OF(rstat_if, statements[j]) ||
						TYPE_OF(rstat_while, statements[j]))
					{
						in_if += 1;
					}

					if (TYPE_OF(rstat_end, statements[j]))
					{
						in_if -= 1;
					}
				}
			}
		}

		// while Ìø×ª
		for (int i = 0; i < statements.Size(); ++i)
		{
			if (TYPE_OF(rstat_while, statements[i]))
			{
				int in_while = 0, end_while = 0;

				for (int j = i + 1; j < statements.Size(); ++j)
				{
					if (TYPE_OF(rstat_end, statements[j]))
					{
						if (in_while == 0)
						{
							end_while = j - 1;
							statements[i]->jump = j;
							break;
						}
					}

					if (TYPE_OF(rstat_if, statements[j]) ||
						TYPE_OF(rstat_while, statements[j]))
					{
						in_while += 1;
					}

					if (TYPE_OF(rstat_end, statements[j]))
					{
						in_while -= 1;
					}
				}

				d_assert (end_while > i);

				statements[end_while]->jump = i;
			}
		}

		for (int i = 0; i < statements.Size(); ++i)
		{
			statements[i]->build();
		}
	}

	void radc_compiler::execute()
	{
		int line = 0;
		while (line < statements.Size() && !is_break() && !is_error())
		{
			int nextline = statements[line]->execute();

			if (nextline == 0)
			{
				line = line + 1;
			}
			else if (nextline == -1)
			{
				line = statements.Size();
			}
			else
			{
				line = nextline;
			}
		}
	}
	
}
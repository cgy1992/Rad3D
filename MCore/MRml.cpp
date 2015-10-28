#include "MRml.h"

namespace Rad {

#define RML_POOL_NAME 1
#define RML_POOL_VALUE 2
#define RML_POOL_ALL 3

	rml_node::rml_node(char * n, char * v, int p)
		: _name(n)
		, _value(v)
		, _pool(p)
		, _childLinker(NULL)
	{
	}

	rml_node::~rml_node()
	{
		clear();

		if (_pool & RML_POOL_NAME)
		{
			pool_free(_name);
		}

		if (_pool & RML_POOL_VALUE)
		{
			pool_free(_value);
		}
	}

	void rml_node::set_value(const char * value)
	{
		if (_pool & RML_POOL_VALUE)
		{
			pool_free(_value);
		}
		else
		{
			_pool |= RML_POOL_VALUE;
		}

		_value = (char *)pool_alloc(strlen(value) + 1);
		strcpy(_value, value);
	}

	rml_node * rml_node::first_node() const
	{
		rml_node * node = _childLinker;

		while (node != NULL)
		{
			if (LINKER_NEXT(node) == NULL)
				return node;

			node = LINKER_NEXT(node);
		}

		return NULL;
	}

	rml_node * rml_node::first_node(const char * name) const
	{
		rml_node * node = first_node();

		while (node != NULL)
		{
			if (strcmp(node->_name, name) == 0)
				return node;

			node = node->next_sibling();
		}

		return NULL;
	}

	rml_node * rml_node::next_sibling() const
	{
		return (rml_node *)LINKER_PREV(this);
	}

	rml_node * rml_node::next_sibling(const char * name) const
	{
		rml_node * node = next_sibling();

		while (node != NULL)
		{
			if (strcmp(node->_name, name) == 0)
				return node;

			node = node->next_sibling();
		}

		return NULL;
	}

	rml_node * rml_node::append(const char * name, const char * value)
	{
		char * n = (char *)pool_alloc(strlen(name) + 1);
		char * v = (char *)pool_alloc(strlen(value) + 1);

		strcpy(n, name);
		strcpy(v, value);

		rml_node * node = new rml_node(n, v, RML_POOL_ALL);

		LINKER_APPEND(_childLinker, node);

		return node;
	}

	rml_node * rml_node::_append_i(char * name, char * value)
	{
		rml_node * node = new rml_node(name, value, 0);

		LINKER_APPEND(_childLinker, node);

		return node;
	}

	void rml_node::clear()
	{
		while (_childLinker != NULL)
		{
			rml_node * node = _childLinker;

			LINKER_REMOVE(_childLinker, node);

			delete node;
		}
	}

	void rml_node::print(String & o_str, const String & prefix) const
	{
		String line_str;

		if (strchr(_value, '\n') == NULL)
		{
			line_str = prefix + _name + " " + _value + "\n";
		}
		else
		{
			line_str = prefix + _name + " {" + _value + "\n" + prefix + "}\n"; 
		}

		o_str += line_str;

		rml_node * node = first_node();

		if (node != NULL)
		{
			o_str += prefix;
			o_str += "{\n";

			while (node != NULL)
			{
				node->print(o_str, prefix + "\t");

				node = node->next_sibling();
			}

			o_str += prefix;
			o_str += "}\n";
		}

	}

	char * rml_node::_parse_child(char * str)
	{
		rml_node * node = NULL;

		char * name = NULL;
		char * value = NULL;
		int nameLen = 0;
		int valueLen = 0;

		while (*str != 0)
		{
			name = NULL;
			value = NULL;
			nameLen = 0;
			valueLen = 0;

			// trim
			while (*str)
			{
				if (*str != ' ' && *str!= '\t' &&
					*str != '\r' && *str != '\n')
					break;

				++str;
			}

			// commented
			if (*str && *str == '#')
			{
				while (*str)
				{
					if (*str++ == '\n')
						break;
				}
			}

			// trim
			while (*str)
			{
				if (*str != ' ' && *str != '\t' &&
					*str != '\r' && *str != '\n')
					break;

				++str;
			}

			// parse name
			name = str;
			while (*str)
			{
				if (*str == ' ' || *str == '\t' ||
					*str == '\r' || *str == '\n')
					break;

				++nameLen;
				++str;
			}

			// trim
			while (*str)
			{
				if (*str != ' ' && *str != '\t')
					break;

				++str;
			}

			// parse value
			if (*str && *str == '{')
			{
				++str;

				value = str;

				int depth = 0;
				while (*str)
				{
					if (*str == '{')
					{
						++depth;
					}
					else if (*str == '}')
					{
						if (depth-- <= 0)
							break;
					}

					++valueLen;
					++str;
				}
			}
			else
			{
				value = str;
				while (*str)
				{
					if (*str == '\r' || *str == '\n')
						break;

					++valueLen;
					++str;
				}
			}

			// skin next line
			while(*str && *str != '\n')
				++str;

			if (*str == '\n')
				++str;

			// trim value
			while (valueLen > 0)
			{
				if (value[valueLen - 1] != ' ' && value[valueLen - 1] != '\t')
					break;

				--valueLen;
			}

			name[nameLen] = 0;
			value[valueLen] = 0;

			if (strcmp(name, "{") == 0 && node != NULL)
			{
				str = node->_parse_child(str);				
			}
			else if (strcmp(name, "}") == 0)
			{
				break;
			}
			else if (strcmp(name, "") != 0)
			{
				node = _append_i(name, value);
			}
		}

		return str;
	}


	//
	rml_doc::rml_doc()
		: rml_node(NULL, NULL, 0)
	{
	}

	rml_doc::~rml_doc()
	{
		clear();
	}

	bool rml_doc::open_file(const String & filename)
	{
		DataStreamPtr stream = new FileStream(filename);

		return open(stream);
	}

	bool rml_doc::open(DataStreamPtr stream)
	{
		d_assert (stream != NULL);
		
		clear();

		if (stream->IsOpen())
		{
			mStream = stream;

			return _parse((char *)mStream->GetData());
		}

		return false;
	}

	void rml_doc::clear()
	{
		rml_node::clear();

		mStream = NULL;
	}

	bool rml_doc::_parse(char * str)
	{
		_parse_child(str);

		return first_node() != NULL;
	}

	void rml_doc::print(String & str)
	{
		rml_node * n = first_node();
		while (n != NULL)
		{
			n->print(str, "");

			n = n->next_sibling();
		}
	}

	bool rml_doc::save_file(const String & filename)
	{
		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
			return false;

		save_file_ex(fp);

		fclose(fp);

		return true;
	}

	bool rml_doc::save_file_ex(FILE * fp)
	{
		String str;

		print(str);

		fwrite(str.c_str(), 1, str.Length(), fp);

		return true;
	}
}


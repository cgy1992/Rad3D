/*
*	Rml
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MLinker.h"
#include "MDataStream.h"

namespace Rad {

	class M_ENTRY rml_node
	{
		DECLARE_POOL_ALLOC();
		DECLARE_LINKER(rml_node);

	public:
		rml_node(char * n, char * v, int p);
		~rml_node();

		const char * 
			name() const { return _name; }
		const char *
			value() const { return _value; }

		void
			set_value(const char * value);

		rml_node * 
			first_node() const;
		rml_node * 
			first_node(const char * name) const;
		rml_node * 
			next_sibling() const;
		rml_node * 
			next_sibling(const char * name) const;
		
		rml_node * 
			append(const char * name, const char * value);
		void 
			clear();

		void 
			print(String & o_str, const String & prefix) const;

	protected:
		char * 
			_parse_child(char * str);
		rml_node *
			_append_i(char * name, char * value);
		
	protected:
		char * _name;
		char * _value;
		int _pool;

		rml_node * _childLinker;
	};

	class M_ENTRY rml_doc : public rml_node
	{
	public:
		rml_doc();
		~rml_doc();

		bool 
			open(const String & filename);
		bool 
			open(DataStreamPtr stream);
		bool 
			parse(const byte * data, int size);
		void 
			clear();

		void
			print(String & str);
		bool 
			save(const String & filename);
		bool 
			save(FILE * fp);

	protected:
		char * i_data;
	};

}

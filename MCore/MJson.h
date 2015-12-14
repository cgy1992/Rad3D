/*
*	Define
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDataStream.h"

struct cJSON;

namespace Rad {

	class M_ENTRY json_node
	{
	public:
		json_node(cJSON * json);
		~json_node();

		const char * 
			name() const;
		bool
			as_bool();
		int
			as_int();
		double
			as_float();
		const char *
			as_string();

		json_node
			first_node() const;
		json_node 
			first_node(const char * name) const;
		json_node
			next_sibling() const;
		json_node
			next_sibling(const char * name) const;

		json_node
			append(const char * name);
		json_node
			append(const char * name, bool boolean);
		json_node
			append(const char * name, int number);
		json_node
			append(const char * name, double number);
		json_node
			append(const char * name, const char * string);
		
		void 
			clear();

	protected:
		cJSON * i_node;
	};

	class M_ENTRY json_doc : public json_node
	{
	public:
		json_doc();
		~json_doc();

		void
			create();
		bool 
			open(const String & filename);
		bool 
			open(DataStreamPtr stream);
		bool 
			parse(const void * data, int size);
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

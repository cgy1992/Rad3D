/*
*	XML - rapidxml
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#define RAPIDXML_NO_EXCEPTIONS

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "MDataStream.h"

namespace Rad {

	typedef rapidxml::xml_document<char> i_xml_doc;
	typedef rapidxml::xml_node<char> i_xml_node;
	typedef rapidxml::xml_attribute<char> i_xml_attri;

	class M_ENTRY xml_node
	{
	public:
		xml_node(i_xml_node * _i_node);
		xml_node(const xml_node & rk);
		~xml_node();

		xml_node
			first_node();
		xml_node 
			first_node(const char * name);
		xml_node
			next_sibling();
		xml_node 
			next_sibling(const char * name);

		const char *
			name();
		const char *
			value();

		const char *
			first_attribute();
		const char *
			first_attribute(const char * name);

		xml_node
			append_node(const char * name);
		xml_node
			insert_node(xml_node & whr, const char * name);
		void
			remove_node(xml_node & whr);
		void
			remove_all_node();

		void
			append_attribute(const char * name, const char * val);
		bool
			insert_attribute(const char * whr, const char * name, const char * val);
		bool
			remove_attribute(const char * whr);
		void
			remove_all_attribute();

		//
		xml_node &
			operator =(const xml_node & rk);
		bool 
			operator ==(const xml_node & rk) const;
		bool
			operator !=(const xml_node & rk) const;

	protected:
		i_xml_node * i_node;
	};

	//
	class M_ENTRY xml_doc : public xml_node
	{
	public:
		xml_doc();
		~xml_doc();

		bool 
			open(const String & filename);
		bool 
			open(DataStreamPtr stream);
		bool 
			parse(const byte * data, int size);
		void
			print(String & str);
		bool 
			save(const String & filename);
		bool 
			save(FILE * fp);

		void
			clear();

	protected:
		char * i_data;
		i_xml_doc i_xml;
	};

}

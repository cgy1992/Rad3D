#include "MXml.h"
#include "MFile.h"

namespace Rad {

	xml_node::xml_node(i_xml_node * _i_node)
		: i_node(_i_node)
	{
	}

	xml_node::~xml_node()
	{
	}

	xml_node::xml_node(const xml_node & rk)
	{
		i_node = rk.i_node;
	}

	xml_node xml_node::first_node()
	{
		d_assert(i_node != NULL);

		return xml_node(i_node->first_node());
	}

	xml_node xml_node::first_node(const char * name)
	{
		d_assert(i_node != NULL);

		return xml_node(i_node->first_node(name));
	}

	xml_node xml_node::next_sibling()
	{
		d_assert(i_node != NULL);

		return xml_node(i_node->next_sibling());
	}

	xml_node xml_node::next_sibling(const char * name)
	{
		d_assert(i_node != NULL);

		return xml_node(i_node->next_sibling(name));
	}

	const char * xml_node::name()
	{
		d_assert(i_node != NULL);

		return i_node->name();
	}

	const char * xml_node::value()
	{
		d_assert(i_node != NULL);

		return i_node->value();
	}

	const char * xml_node::first_attribute()
	{
		d_assert(i_node != NULL);

		i_xml_attri * attri = i_node->first_attribute();

		return attri != NULL ? attri->value() : "";
	}

	const char * xml_node::first_attribute(const char * name)
	{
		d_assert(i_node != NULL);

		i_xml_attri * attri = i_node->first_attribute(name);

		return attri != NULL ? attri->value() : "";
	}

	xml_node xml_node::append_node(const char * name)
	{
		d_assert(i_node != NULL);

		i_xml_node * node = i_node->document()->allocate_node_ex(rapidxml::node_element, name);

		i_node->append_node(node);

		return xml_node(node);
	}

	xml_node xml_node::insert_node(xml_node & whr, const char * name)
	{
		d_assert(i_node != NULL && whr != NULL);

		i_xml_node * node = i_node->document()->allocate_node_ex(rapidxml::node_element, name);

		i_node->insert_node(whr.i_node, node);

		return xml_node(node);
	}

	void xml_node::remove_node(xml_node & whr)
	{
		d_assert(i_node != NULL && whr != NULL);

		i_node->remove_node(whr.i_node);
	}

	void xml_node::remove_all_node()
	{
		d_assert(i_node != NULL);

		i_node->remove_all_nodes();
	}

	void xml_node::append_attribute(const char * name, const char * val)
	{
		d_assert(i_node != NULL);

		i_xml_attri * attri = i_node->document()->allocate_attribute_ex(name, val);

		i_node->append_attribute(attri);
	}

	bool xml_node::insert_attribute(const char * whr, const char * name, const char * val)
	{
		d_assert(i_node != NULL);

		i_xml_attri * attri = i_node->first_attribute(whr);

		if (attri)
		{
			i_xml_attri * i_attri = i_node->document()->allocate_attribute_ex(name, val);

			i_node->insert_attribute(attri, i_attri);

			return true;
		}

		return false;
	}

	bool xml_node::remove_attribute(const char * whr)
	{
		d_assert(i_node != NULL);

		i_xml_attri * attri = i_node->first_attribute(whr);

		if (attri)
		{
			i_node->remove_attribute(attri);

			return true;
		}

		return false;
	}

	void xml_node::remove_all_attribute()
	{
		d_assert(i_node != NULL);

		i_node->remove_all_attributes();
	}

	xml_node & xml_node::operator =(const xml_node & rk)
	{
		i_node = rk.i_node;
		return *this;
	}

	bool xml_node::operator ==(const xml_node & rk) const
	{
		return i_node == rk.i_node;
	}

	bool xml_node::operator !=(const xml_node & rk) const
	{
		return i_node != rk.i_node;
	}


	//
	xml_doc::xml_doc()
		: xml_node(&i_xml)
		, i_data(NULL)
	{
	}

	xml_doc::~xml_doc()
	{
		clear();
	}

	bool xml_doc::open_file(const String & filename)
	{
		DataStreamPtr stream = new FileStream(filename);

		if (stream->IsOpen())
			return open(stream);

		return false;
	}

	bool xml_doc::open(DataStreamPtr stream)
	{
		d_assert (stream != NULL);

		return parse(stream->GetData(), stream->Size());
	}

	bool xml_doc::parse(const void * data, int size)
	{
		clear();

		i_data = new char[size + 1];

		memcpy(i_data, data, size);
		i_data[size] = 0;

		i_xml.parse<0>(i_data);

		return true;
	}

	void xml_doc::print(std::ostream & stream)
	{
		stream << i_xml;
	}

	void xml_doc::clear()
	{
		i_xml.clear();
		safe_delete_array(i_data);
	}
	
}


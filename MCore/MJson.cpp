#include "MJson.h"
#include "cJSON.c"

namespace Rad {

	json_node::json_node(cJSON * json)
	{
		i_node = json;
	}

	json_node::~json_node()
	{
	}

	const char * json_node::name() const
	{
		d_assert (i_node != NULL);

		return i_node->string;
	}

	bool json_node::as_bool()
	{
		d_assert (i_node != NULL && (i_node->type == cJSON_False || i_node->type == cJSON_True));

		return i_node->type == cJSON_True;
	}

	int json_node::as_int()
	{
		d_assert (i_node != NULL && i_node->type == cJSON_Number);

		return i_node->valueint;
	}

	double json_node::as_float()
	{
		d_assert (i_node != NULL && i_node->type == cJSON_Number);

		return i_node->valuedouble;
	}

	const char * json_node::as_string()
	{
		d_assert (i_node != NULL && i_node->type == cJSON_String);
		
		return i_node->valuestring;
	}

	json_node json_node::first_node() const
	{
		d_assert (i_node != NULL);

		return i_node->child;
	}

	json_node json_node::first_node(const char * name) const
	{
		d_assert (i_node != NULL);

		cJSON * child = i_node->child;
		while (child != NULL)
		{
			if (strcmp(child->string, name) == 0)
				return child;
		}

		return NULL;
	}

	json_node json_node::next_sibling() const
	{
		d_assert (i_node != NULL);

		return i_node->next;
	}

	json_node json_node::next_sibling(const char * name) const
	{
		d_assert (i_node != NULL);

		cJSON * next = i_node->next;
		while (next != NULL)
		{
			if (strcmp(next->string, name) == 0)
				return json_node(next);
		}

		return next;
	}

	json_node json_node::append(const char * name)
	{
		d_assert (i_node != NULL);

		cJSON * json = cJSON_CreateObject();

		cJSON_AddItemToObject(i_node, name, json);

		return json;
	}

	json_node json_node::append(const char * name, bool boolean)
	{
		d_assert (i_node != NULL);

		cJSON * json = cJSON_CreateBool(boolean);

		cJSON_AddItemToObject(i_node, name, json);

		return json;
	}

	json_node json_node::append(const char * name, int number)
	{
		d_assert (i_node != NULL);

		cJSON * json = cJSON_CreateNumber(number);

		cJSON_AddItemToObject(i_node, name, json);

		return json;
	}

	json_node json_node::append(const char * name, double number)
	{
		d_assert (i_node != NULL);

		cJSON * json = cJSON_CreateNumber(number);

		cJSON_AddItemToObject(i_node, name, json);

		return json;
	}

	json_node json_node::append(const char * name, const char * string)
	{
		d_assert (i_node != NULL);

		cJSON * json = cJSON_CreateString(string);

		cJSON_AddItemToObject(i_node, name, json);

		return json;
	}

	void json_node::clear()
	{
		if (i_node != NULL)
		{
			cJSON_Delete(i_node);
			i_node = NULL;
		}
	}

	//
	void * json_alloc(size_t sz)
	{
		return pool_alloc(sz);
	}

	void json_free(void *ptr)
	{
		pool_free(ptr);
	}

	json_doc::json_doc()
		: json_node(NULL)
		, i_data(NULL)
	{
		cJSON_Hooks hooks;
		hooks.malloc_fn = json_alloc;
		hooks.free_fn = json_free;

		cJSON_InitHooks(&hooks);
	}

	json_doc::~json_doc()
	{
		clear();
	}

	void json_doc::create()
	{
		d_assert (i_node == NULL);

		i_node = cJSON_CreateObject();
	}

	bool json_doc::open(const String & filename)
	{
		return open(new FileStream(filename));
	}

	bool json_doc::open(DataStreamPtr stream)
	{
		d_assert (stream != NULL);

		if (stream->IsOpen())
			return parse((byte *)stream->GetData(), stream->Size());

		return false;
	}

	bool json_doc::parse(const void * data, int size)
	{
		clear();

		i_data = new char[size + 1];

		memcpy(i_data, data, size);
		i_data[size] = 0;

		i_node = cJSON_Parse(i_data);

		return i_node != NULL;
	}

	void json_doc::clear()
	{
		json_node::clear();

		safe_delete_array(i_data);
	}

	void json_doc::print(String & str)
	{
		d_assert (i_node != NULL);

		char * p = cJSON_Print(i_node);

		str = p;

		cJSON_free(p);
	}

	bool json_doc::save(const String & filename)
	{
		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
			return false;

		save(fp);

		fclose(fp);

		return true;
	}

	bool json_doc::save(FILE * fp)
	{
		String str;

		print(str);

		fwrite(str.c_str(), 1, str.Length(), fp);

		return true;
	}

}
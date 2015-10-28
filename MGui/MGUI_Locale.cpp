#include "MGUI_Locale.h"

namespace Rad {

	using namespace MGUI;

	ImplementSingleton(Locale);

	Locale::Locale()
	{
	}

	Locale::~Locale()
	{
		Unload();
	}

	void Locale::Load(const String & filename)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);
		if (stream == NULL)
		{
			d_log("Error: file '%s' open failed.", filename.c_str());
			return ;
		}

		rml_doc doc;
		doc.open(stream);

		rml_node * node = doc.first_node();
		while (node != NULL)
		{
			mStringMap.Insert(node->name(), node->value());

			node = node->next_sibling();
		}
	}

	void Locale::Unload()
	{
		mStringMap.Clear();
	}

	const char * Locale::GetString(const String & key)
	{
		int i = mStringMap.Find(key);

		return i != -1 ? mStringMap[i].value.c_str() : NULL;
	}

	void Locale::Insert(const String & key, const String & str)
	{
		mStringMap.Insert(key, str);
	}

	void Locale::Remove(const String & key)
	{
		int i = mStringMap.Find(key);
		if (i != -1)
		{
			mStringMap.Erase(i);
		}
	}

}

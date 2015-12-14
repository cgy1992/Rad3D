#include "MGUI_Locale.h"

namespace Rad {

	using namespace MGUI;

	ImplementSingleton(Locale);

	Locale::Locale()
	{
		mNeedSave = false;
	}

	Locale::~Locale()
	{
		Unload();
	}

	void Locale::Load(const String & filename)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);
		if (stream != NULL)
		{
			rml_doc doc;
			doc.open(stream);

			rml_node * node = doc.first_node();
			while (node != NULL)
			{
				mStringMap.Insert(node->name(), node->value());

				node = node->next_sibling();
			}
		}
	}

	void Locale::Save(const String & filename)
	{
		if (!mNeedSave)
			return ;

		rml_doc doc;
		for (int i = 0; i < mStringMap.Size(); ++i)
		{
			doc.append(mStringMap[i].key.c_str(), mStringMap[i].value.c_str());
		}

		doc.save(filename);

		mNeedSave = false;
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

		if (str == "")
			mNeedSave = true;
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

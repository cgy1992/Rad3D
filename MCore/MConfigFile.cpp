#include "MConfigFile.h"
#include "MFile.h"

namespace Rad {

	ConfigFile::ConfigFile()
	{
	}

	ConfigFile::~ConfigFile()
	{
	}

	bool ConfigFile::Load(const char * filename)
	{
		if (FileHelper::Exist(filename))
		{
			return LoadStream(new FileStream(filename));
		}

		return false;
	}

	bool ConfigFile::LoadStream(DataStreamPtr stream)
	{
		if (stream != NULL && stream->IsOpen())
		{
			return mDocument.open(stream);
		}

		return false;
	}

	void ConfigFile::Save(const char * filename)
	{
		mDocument.save_file(filename);
	}

	void ConfigFile::Clear()
	{
		mDocument.clear();
	}

	void ConfigFile::SetVariable(const char * name, const char * value)
	{
		rml_node * node = mDocument.first_node(name);

		if (node != NULL)
		{
			node->set_value(value);
		}
		else
		{
			mDocument.append(name, value);
		}
	}

	const char * ConfigFile::GetVariable(const char * name)
	{
		rml_node * node = mDocument.first_node(name);
		if (node != NULL)
		{
			return node->value();
		}

		return NULL;
	}

	bool ConfigFile::GetBool(const char * name, bool _default)
	{
		const char * str = GetVariable(name);

		if (str)
		{
			return atoi(str) != 0;
		}

		return _default;
	}

	int ConfigFile::GetInt(const char * name, int _default)
	{
		const char * str = GetVariable(name);

		if (str != NULL)
		{
			return atoi(str);
		}

		return _default;
	}

	float ConfigFile::GetFloat(const char * name, float _default)
	{
		const char * str = GetVariable(name);

		if (str != NULL)
		{
			return (float)atof(str);
		}

		return _default;
	}

	const char * ConfigFile::GetString(const char * name, const char * _default)
	{
		const char * str = GetVariable(name);

		if (str != NULL)
		{
			return str;
		}

		return _default;
	}

}

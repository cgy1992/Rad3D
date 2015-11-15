#include "MShaderFXManager.h"
#include "MResourceManager.h"
#include "MLog.h"
#include "MRml.h"

namespace Rad {

	ImplementSingleton(ShaderFXManager);

    ShaderFXManager::ShaderFXManager()
    {
    }

    ShaderFXManager::~ShaderFXManager()
    {
		for (int i = 0; i < mFXMap.Size(); ++i)
		{
			delete mFXMap[i].value;
		}

		mFXMap.Clear();
    }

	ShaderFX * ShaderFXManager::Load(const String & name, const String & source, const String & macros)
	{
		Hash2 hash(name.c_str());

		ShaderFX * pShaderFX = _find(hash, name);
		if (pShaderFX != NULL)
		{
			return pShaderFX;
		}

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);
		if (stream == NULL)
		{
			d_log("?: ShaderFX '%s' Open Failed.", source.c_str());
			return NULL;
		}

		pShaderFX = new ShaderFX(name, stream->GetSource(), macros);

		_loadImp(pShaderFX, stream);

		mFXMap.Insert(hash, pShaderFX);

		return pShaderFX;
	}

	ShaderFX * ShaderFXManager::Load(const String & name, DataStreamPtr stream, const String & macros)
	{
		Hash2 hash(name.c_str());

		ShaderFX * pShaderFX = _find(hash, name);
		if (pShaderFX != NULL)
		{
			return pShaderFX;
		}

		pShaderFX = new ShaderFX(name, stream->GetSource(), macros);

		_loadImp(pShaderFX, stream);

		mFXMap.Insert(hash, pShaderFX);

		return pShaderFX;

	}

	void ShaderFXManager::Remove(ShaderFX * fx)
	{
		Hash2 hash(fx->GetName().c_str());
		int i = mFXMap.Find(hash);

		d_assert (i != -1);

		delete mFXMap[i].value;
		mFXMap.Erase(i);
	}

	void ShaderFXManager::Reload(ShaderFX * fx)
	{
		if (fx->GetSource() != "")
		{
			fx->Clear();

			DataStreamPtr stream = ResourceManager::Instance()->OpenResource(fx->GetSource());
			if (stream == NULL)
			{
				d_log("?: ShaderFX '%s' Open Failed.", fx->GetSource().c_str());
			}

			_loadImp(fx, stream);
		}
	}

	void ShaderFXManager::ReloadAll()
	{
		for (int i = 0; i < mFXMap.Size(); ++i)
		{
			Reload(mFXMap[i].value);
		}

		E_ReloadAll();
	}

	void ShaderFXManager::LoadLibrary(const String & libname)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(libname);
		if (stream == NULL)
			return ;

		rml_doc doc;
		doc.open(stream);

		String name, filename, macro;

		rml_node * fx = doc.first_node("ShaderFX");
		while (fx != NULL)
		{
			name = fx->value();
			filename[0] = 0;
			macro[0] = 0;

			rml_node * node = fx->first_node("Filename");
			if (node != NULL)
			{
				filename = node->value();

				node = fx->first_node("Macro");
				if (node != NULL)
				{
					macro = node->value();

					Load(name, filename, macro);
				}
			}

			fx = fx->next_sibling("ShaderFX");
		}
	}

	void ShaderFXManager::AddGlobalMacro(const String & macro)
	{
		mGlobalMacroString += macro;
	}
	
	void ShaderFXManager::RemoveGlobalMacro(const String & macro)
	{
		int i = mGlobalMacroString.Find(macro, 0);
		if (i != -1)
		{
			mGlobalMacroString.Erase(i, macro.Length());
		}
	}

	const char * ShaderFXManager::GetGlobalMacroString()
	{
		return mGlobalMacroString.c_str();
	}

	ShaderFX * ShaderFXManager::_find(Hash2 hash, const String & name)
	{
		int i = mFXMap.Find(hash);

		d_assert (i == -1 || name == mFXMap[i].value->GetName());

		return i != -1 ? mFXMap[i].value : NULL;
	}

}
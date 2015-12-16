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

}
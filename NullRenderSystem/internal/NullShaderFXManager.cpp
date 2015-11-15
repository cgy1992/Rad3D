#include "NullShaderFxManager.h"

namespace Rad {

	//
	NullShaderFXManager::NullShaderFXManager()
	{
	}

	NullShaderFXManager::~NullShaderFXManager()
	{
	}

	void NullShaderFXManager::_loadImp(ShaderFX * fx, DataStreamPtr stream)
	{
		fx->AddPass(new NullShaderPass);
	}

}
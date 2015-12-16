#include "NullShaderFxManager.h"

namespace Rad {

	//
	NullShaderFXManager::NullShaderFXManager()
	{
		mShaderFX = new ShaderFX("", "", "");
		mShaderFX->AddPass(new NullShaderPass);
	}

	NullShaderFXManager::~NullShaderFXManager()
	{
		delete mShaderFX;
	}

	ShaderFX * NullShaderFXManager::Load(const String & name, const String & source, const String & macros)
	{
		return mShaderFX;
	}

	ShaderFX * NullShaderFXManager::Load(const String & name, DataStreamPtr stream, const String & macros)
	{
		return mShaderFX;
	}

}
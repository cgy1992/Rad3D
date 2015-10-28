#include "NullShaderFxManager.h"

namespace Rad {

	//
	NullShaderFXManager::NullShaderFXManager()
	{
		mNullShaderFX = new ShaderFX("Null", "Null", "");
		mNullShaderFX->AddPass(new NullShaderPass);
	}

	NullShaderFXManager::~NullShaderFXManager()
	{
		delete mNullShaderFX;
	}

	ShaderFX * NullShaderFXManager::Load(const String & name, const String & source, const String & macros)
	{
		return mNullShaderFX;
	}
}
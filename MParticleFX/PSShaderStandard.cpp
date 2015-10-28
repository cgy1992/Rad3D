#include "PSShaderStandard.h"

namespace Rad {

	ImplementRTTI(PS_ShaderStandard, PS_Shader);

	PS_ShaderStandard::PS_ShaderStandard()
	{
		mFX = ShaderFXManager::Instance()->Load("Shader/MParticle.mfx", "Shader/MParticle.mfx");
	}

	PS_ShaderStandard::~PS_ShaderStandard()
	{
	}

}


#include "MShaderProvider.h"
#include "MRenderObject.h"
#include "MLight.h"
#include "MMesh.h"
#include "MShaderFXManager.h"

namespace Rad {

	ShaderProviderStandard::ShaderProviderStandard()
	{
		mBaseFX = ShaderFXManager::Instance()->Load("Core.Base", "Shader/MBase.mfx");
		mBaseFXSkined = ShaderFXManager::Instance()->Load("Core.BaseSkined", "Shader/MBase.mfx", "#define D_SKINED\n");

		mLightingColorFX = ShaderFXManager::Instance()->Load("Core.LightingColor", "Shader/MLightingColor.mfx");
		mLightingMapFX = ShaderFXManager::Instance()->Load("Core.LightingMap", "Shader/MLightingMap.mfx");

		mLightingFX[eLightType::DIRECTION] = 
			ShaderFXManager::Instance()->Load("Core.DirLighting", "Shader/MLighting.mfx", "#define DIRECTION\n");
		mLightingFXSkined[eLightType::DIRECTION] = 
			ShaderFXManager::Instance()->Load("Core.DirLightingSkined", "Shader/MLighting.mfx", "#define D_SKINED\n#define DIRECTION\n");

		mLightingFX[eLightType::POINT] = 
			ShaderFXManager::Instance()->Load("Core.PointLighting", "Shader/MLighting.mfx", "#define POINT\n");
		mLightingFXSkined[eLightType::POINT] = 
			ShaderFXManager::Instance()->Load("Core.PointLightingSkined", "Shader/MLighting.mfx", "#define D_SKINED\n#define POINT");

		mLightingFX[eLightType::SPOT] = 
			ShaderFXManager::Instance()->Load("Core.SpotLighting", "Shader/MLighting.mfx", "#define SPOT\n");
		mLightingFXSkined[eLightType::SPOT] = 
			ShaderFXManager::Instance()->Load("Core.SpotLightingSkined", "Shader/MLighting.mfx", "#define D_SKINED\n#define SPOT");
	}

	ShaderProviderStandard::~ShaderProviderStandard()
	{
	}

	void ShaderProviderStandard::ApplyShaderFX(RenderObject * able, int flag)
	{
		if (able->GetShaderFX())
		{
			able->SetCurrentShaderFX(able->GetShaderFX());
		}
		else if (able->IsSkined())
		{
			able->SetCurrentShaderFX(mBaseFXSkined);
		}
		else
		{
			Node * node = able->_getNode();
			if (TYPE_OF(Mesh, node))
			{
				Mesh * mesh = (Mesh *)node;

				if (able->GetShaderProvider() == NULL)
				{
					if (mesh->GetSLMode() == eStaticLightingMode::LIGHTING_COLOR && 
						able->GetRenderOp()->vertexBuffers[LIGHTING_COLOR_STREAM] != NULL)
					{
						able->SetCurrentShaderFX(mLightingColorFX);
					}
					else if (mesh->GetSLMode() == eStaticLightingMode::LIGHTING_MAP && 
						able->GetMaterial()->maps[eMapType::LIGHTING_MAP] != NULL)
					{
						able->SetCurrentShaderFX(mLightingMapFX);
					}
					else
					{
						able->SetCurrentShaderFX(mBaseFX);
					}
				}
				else
				{
					able->GetShaderProvider()->ApplyShaderFX(able, flag);
				}
			}
			else
			{
				able->SetCurrentShaderFX(mBaseFX);
			}
		}
	}

	void ShaderProviderStandard::ApplyLightingFX(Light * light, RenderObject * able)
	{
		eLightType type = light->GetType();

		if (able->GetLightShaderFX(type) != NULL)
		{
			able->SetCurrentShaderFX(able->GetLightShaderFX(type));
		}
		else if (able->GetShaderProvider() == NULL)
		{
			able->SetCurrentShaderFX(able->IsSkined() ? mLightingFXSkined[type.value] : mLightingFX[type.value]);
		}
		else
		{
			able->GetShaderProvider()->ApplyLightingFX(light, able);
		}
	}
}


#include "MMeshShader.h"
#include "MMeshSource.h"
#include "MRenderRegister.h"
#include "MShaderFXManager.h"
#include "MRoot.h"
#include "MMesh.h"
#include "MLight.h"

namespace Rad {

	//
	ImplementRTTI(MeshShader, IObject);

	//
	ImplementRTTI(MeshShaderNull, MeshShader);

	//
	ImplementRTTI(MeshShaderStandard, MeshShader);

	DF_PROPERTY_BEGIN(MeshShaderStandard)
		DF_PROPERTY(MeshShaderStandard, mUVScroll, "Shader", "UVScroll", PT_Float2)
	DF_PROPERTY_END();

	MeshShaderStandard::MeshShaderStandard()
	{
		mBaseFX = ShaderFXManager::Instance()->Load("MeshStandard.Base", "Shader/Standard/MBase.mfx");
		mBaseFXSkined = ShaderFXManager::Instance()->Load("MeshStandard.BaseSkined", "Shader/Standard/MBase.mfx", "#define D_SKINED\n");

		mLightingColorFX = ShaderFXManager::Instance()->Load("MeshStandard.LightingColor", "Shader/Standard/MLightingColor.mfx");
		mLightingMapFX = ShaderFXManager::Instance()->Load("MeshStandard.LightingMap", "Shader/Standard/MLightingMap.mfx");

		mLightingFX[eLightType::DIRECTION] = 
			ShaderFXManager::Instance()->Load("MeshStandard.DirLighting", "Shader/Standard/MLighting.mfx", "#define DIRECTION\n");
		mLightingFXSkined[eLightType::DIRECTION] = 
			ShaderFXManager::Instance()->Load("MeshStandard.DirLightingSkined", "Shader/Standard/MLighting.mfx", "#define D_SKINED\n#define DIRECTION\n");

		mLightingFX[eLightType::POINT] = 
			ShaderFXManager::Instance()->Load("MeshStandard.PointLighting", "Shader/Standard/MLighting.mfx", "#define POINT\n");
		mLightingFXSkined[eLightType::POINT] = 
			ShaderFXManager::Instance()->Load("MeshStandard.PointLightingSkined", "Shader/Standard/MLighting.mfx", "#define D_SKINED\n#define POINT");

		mLightingFX[eLightType::SPOT] = 
			ShaderFXManager::Instance()->Load("MeshStandard.SpotLighting", "Shader/Standard/MLighting.mfx", "#define SPOT\n");
		mLightingFXSkined[eLightType::SPOT] = 
			ShaderFXManager::Instance()->Load("MeshStandard.SpotLightingSkined", "Shader/Standard/MLighting.mfx", "#define D_SKINED\n#define SPOT");
	}

	MeshShaderStandard::~MeshShaderStandard()
	{
	}

	void MeshShaderStandard::ApplyShaderFX(RenderObject * able, int flags)
	{
		if (able->IsSkined())
		{
			able->SetCurrentShaderFX(mBaseFXSkined);
		}
		else
		{
			d_assert (KIND_OF(Mesh, able->_getNode()));

			SubMesh * submesh = (SubMesh *)able;
			if (submesh->GetParent()->GetSLMode() == eStaticLightingMode::LIGHTING_COLOR && 
				able->GetRenderOp()->vertexBuffers[LIGHTING_COLOR_STREAM] != NULL)
			{
				able->SetCurrentShaderFX(mLightingColorFX);
			}
			else if (submesh->GetParent()->GetSLMode() == eStaticLightingMode::LIGHTING_MAP && 
				able->GetMaterial()->maps[eMapType::LIGHTING_MAP] != NULL)
			{
				able->SetCurrentShaderFX(mLightingMapFX);
			}
			else
			{
				able->SetCurrentShaderFX(mBaseFX);
			}
		}
	}

	void MeshShaderStandard::ApplyLightingFX(Light * light, RenderObject * able)
	{
		eLightType type = light->GetType();
		able->SetCurrentShaderFX(able->IsSkined() ? mLightingFXSkined[type.value] : mLightingFX[type.value]);
	}

	void MeshShaderStandard::OnCallBack(RenderObject * obj)
	{
		float time = Root::Instance()->GetTime();

		Float4 uvScroll;
		uvScroll.x = time * mUVScroll.x;
		uvScroll.y = time * mUVScroll.y;

		RenderRegister::Instance()->SetUserConst(0, uvScroll);
	}

}
#include "PSShaderMesh.h"

namespace Rad {

	ImplementRTTI(PS_ShaderMesh, PS_Shader);

	PS_ShaderMesh::PS_ShaderMesh()
	{
		mFX = ShaderFXManager::Instance()->Load("Shader/MParticleMesh.mfx", "Shader/MParticleMesh.mfx");
	}

	PS_ShaderMesh::~PS_ShaderMesh()
	{
	}

	void PS_ShaderMesh::OnCallBack(RenderObject * obj)
	{
		Float4 color;
		color.r = obj->GetMaterial()->diffuse.r;
		color.g = obj->GetMaterial()->diffuse.g;
		color.b = obj->GetMaterial()->diffuse.b;
		color.a = obj->_getOpacity();

		mFX->GetPass(0)->SetConst("u_Color", color);
	}

}


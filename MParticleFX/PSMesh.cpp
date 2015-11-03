#include "PSMesh.h"
#include "PSMeshSet.h"

namespace Rad {

	PS_Mesh::PS_Mesh(Mesh * mesh, PS_MeshSet * parent)
		: mParent(parent)
		, mMesh(mesh)
	{
	}

	PS_Mesh::~PS_Mesh()
	{
		delete mMesh;
	}

	void PS_Mesh::_update(Particle * p)
	{
		Float3 Position = p->Position;
		if (mParent->IsLocalSpace())
			Position.TransformA(mParent->GetParent()->GetWorldTM());

		mMesh->SetPosition(Position);
		mMesh->SetOpacity(p->Color.a);
		mMesh->SetRotationEx(p->Rotation);
		mMesh->SetScale(p->Size);
		mMesh->_updateTM();

		int blendMode = mParent->GetBlendMode();

		for (int i = 0; i < mMesh->GetSubMeshCount(); ++i)
		{
			SubMesh * submesh = mMesh->GetSubMesh(i);
			Material * mtl = submesh->GetMaterial();
			
			mtl->diffuse = Float3(p->Color.r, p->Color.g, p->Color.b);
			if (mParent->_getTexture() != NULL)
			{
				mtl->maps[eMapType::DIFFUSE] = mParent->_getTexture();
			}

			if (mParent->GetShaderEnable())
			{
				mtl->depthMode = eDepthMode::N_LESS_EQUAL;

				if (blendMode == PS_BlendMode::ADD)
				{
					mtl->blendMode = eBlendMode::ADD;
				}
				else if (blendMode == PS_BlendMode::ALPHA_BLEND)
				{
					mtl->blendMode = eBlendMode::ALPHA_BLEND;
				}
				else if (blendMode == PS_BlendMode::COLOR_BLEND)
				{
					mtl->blendMode = eBlendMode::COLOR_BLEND;
				}
				else
				{
					mtl->blendMode = eBlendMode::OPACITY;
					mtl->depthMode = eDepthMode::LESS_EQUAL;
				}

				submesh->SetShaderFX(mParent->_getShaderFX());
				submesh->SetRenderCallBack(eRenderCallBack::SHADER, mParent->GetShader().c_ptr());
			}
		}
	}

}
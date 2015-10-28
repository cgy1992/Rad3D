#include "PSMesh.h"
#include "PSMeshSet.h"

namespace Rad {

	PS_Mesh::PS_Mesh(PS_MeshSet * parent)
		: mParent(parent)
	{
		mMesh = new Mesh;
		mMesh->SetSource(mParent->_getSource());
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
		mMesh->SetDirection(p->Direction);

		for (int i = 0; i < mMesh->GetSubMeshCount(); ++i)
		{
			mMesh->GetSubMesh(i)->GetMaterial()->diffuse = Float3(p->Color.r, p->Color.g, p->Color.b);
		}
	}

}
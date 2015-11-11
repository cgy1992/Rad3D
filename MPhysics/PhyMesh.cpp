#include "PhyMesh.h"
#include "PhyWorld.h"
#include "PhyInternal.h"
#include "MMesh.h"

namespace Rad {

	ImplementRTTI(PhyMesh, PhyEntity);

	PhyMesh::PhyMesh(eType type)
		: PhyEntity(type)
	{
		mGeomId = dCreateTriMesh(g_OdeSpace, NULL, NULL, NULL, NULL);

		if (mBodyId != NULL)
			dGeomSetBody(mOdeGeom, mOdeBody);

		dGeomSetData(mOdeGeom, this);
	}

	PhyMesh::~PhyMesh()
	{
	}

	void PhyMesh::SetMeshData(PhyMeshDataPtr data)
	{
		if (mMeshData != data)
		{
			mMeshData = data;

			if (mMeshData != NULL)
			{
				dGeomTriMeshSetData(mOdeGeom, (dTriMeshDataID)(data->_dataId));
			}
			else
			{
				dGeomTriMeshSetData(mOdeGeom, NULL);
			}
		}
	}

	PhyMeshDataPtr PhyMesh::GetMeshData()
	{
		return mMeshData;
	}

	void PhyMesh::SetMass(float mass)
	{
		if (mBodyId != NULL)
		{
			dMass m;
			dMassSetZero(&m);
			dMassSetTrimeshTotal(&m, mass, mOdeGeom);
			dBodySetMass(mOdeBody, &m);
		}
	}

	void PhyMesh::OnAttach(Node * owner)
	{
		PhyEntity::OnAttach(owner);

		OnLoaded();
	}

	void PhyMesh::OnLoaded()
	{
		if (KIND_OF(Mesh, mOwner))
		{
			Mesh * mesh = (Mesh *)mOwner;
			if (mesh->GetSource() != NULL && mesh->GetSource()->GetLoadState() == Resource::LOADED)
			{
				if (!mesh->GetSource()->GetColMesh()->IsValid())
				{
					if (Root::InEditor())
					{
						mesh->GetSource()->GenColMeshFromRenderMesh();
					}
				}

				mesh->GetSource()->GetColMesh();

				ColMesh * pColMesh =  mesh->GetSource()->GetColMesh();

				if (pColMesh->IsValid())
				{
					PhyMeshDataPtr pMeshData = PhyMeshDataPool::Instance()->Load(
						mesh->GetSource()->GetName(),
						pColMesh->GetVertices(), pColMesh->GetNumOfVertices(),
						pColMesh->GetIndices(), pColMesh->GetNumOfIndices(),
						mesh->GetScale());

					SetMeshData(pMeshData);
					SetPosition(mesh->GetPosition());
					SetRotation(mesh->GetRotation());
				}
			}
		}
	}

	void PhyMesh::OnUnload()
	{
		SetMeshData(NULL);
	}

	void PhyMesh::OnChangeTm(int flags)
	{
		PhyEntity::OnChangeTm(flags);

		if ((flags & eTmFlags::SCALE) && KIND_OF(Mesh, mOwner))
		{
			Mesh * mesh = (Mesh *)mOwner;
			if (mesh->GetSource() != NULL && mesh->GetSource()->GetLoadState() == Resource::LOADED)
			{
				if (!mesh->GetSource()->GetColMesh()->IsValid())
				{
					if (Root::InEditor())
					{
						mesh->GetSource()->GenColMeshFromRenderMesh();
					}
				}

				ColMesh * pColMesh =  mesh->GetSource()->GetColMesh();
				if (pColMesh->IsValid())
				{
					PhyMeshDataPtr pMeshData = PhyMeshDataPool::Instance()->Load(
						mesh->GetSource()->GetName(),
						pColMesh->GetVertices(), pColMesh->GetNumOfVertices(),
						pColMesh->GetIndices(), pColMesh->GetNumOfIndices(),
						mesh->GetScale());

					SetMeshData(pMeshData);
				}
			}
		}
	}

}
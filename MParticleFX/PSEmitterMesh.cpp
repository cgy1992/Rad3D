#include "PSEmitterMesh.h"

namespace Rad {

	ImplementRTTI(PS_EmitterMesh, PS_EmitterPoint);

	DF_PROPERTY_BEGIN(PS_EmitterMesh)
		DF_PROPERTY_EX(PS_EmitterMesh, mFilename, "", "Filename", "PT_Filename mesh(*.mesh)|*.mesh|", PT_String)
	DF_PROPERTY_END()

	PS_EmitterMesh::PS_EmitterMesh()
	{
	}

	PS_EmitterMesh::~PS_EmitterMesh()
	{
	}

	void PS_EmitterMesh::SetFilename(const String & filename)
	{
		mFilename = filename;
		mMesh = MeshManager::Instance()->LoadMesh(mFilename);
	}

	void PS_EmitterMesh::InitParticle(Particle * p)
	{
		if (mMesh != NULL && mMesh->GetMeshBufferCount() > 0)
		{
			p->Color = RandomColor();
			p->UVRect = RandomUVRect();
			p->Size = RandomSize();
			p->Rotation = RandomRotation();
			p->Speed = RandomSpeed();
			p->Life = RandomLife();
			p->MaxLife = p->Life;

			_randomOnMeshVertex(p->Position, p->Direction);
		}
	}

	void PS_EmitterMesh::_randomOnMeshVertex(Float3 & position, Float3 & direction)
	{
		RenderOp * rop = mMesh->GetMeshBuffer(0)->GetRenderOp();

		int stride = rop->vertexBuffers[0]->GetStride();
		int index = Math::RandRange(0, rop->vertexBuffers[0]->GetCount() - 1);

		d_assert (
			rop->vertexDeclarations[0].HasElement(eVertexSemantic::POSITION, eVertexType::FLOAT3) &&
			rop->vertexDeclarations[0].HasElement(eVertexSemantic::NORMAL, eVertexType::FLOAT3));

		char * vdata = (char * )rop->vertexBuffers[0]->Lock(eLockFlag::READ) + stride * index;
		{
			Float3 p = *((Float3 *)vdata);
			Float3 n = *((Float3 *)vdata + 1);

			position = p;

			Float3 common(1, 0, 0);
			if (Math::Equal(common.Dot(n), 0.0f))
			{
				common = Float3(0, 1, 0);
			}

			common = Float3::Cross(common, n);

			_randomDirection(direction, n, common);
			
			vdata += index * stride;
		}
		rop->vertexBuffers[0]->Unlock();
	}

	void PS_EmitterMesh::_randomDirection(Float3 & dir, const Float3 & n, const Float3 & common)
	{
		float rads = Math::RandRange(mAngle.x, mAngle.x);
		rads = Math::DegreeToRadian(rads);

		Quat qRotate;
		qRotate.FromAxis(common, rads);

		dir = n * qRotate;

		rads = Math::RandRange(0.0f, 2 * PI);
		qRotate.FromAxis(n, rads);
		dir = dir * qRotate;
	}

	void PS_EmitterMesh::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mFilename")
		{
			SetFilename(mFilename);
		}
		else
		{
			PS_EmitterPoint::OnPropertyChanged(p);
		}
	}

}
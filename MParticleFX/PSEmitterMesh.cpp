#include "PSEmitterMesh.h"

namespace Rad {

	ImplementRTTI(PS_EmitterMesh, PS_EmitterPoint);

	DF_PROPERTY_BEGIN(PS_EmitterMesh)
		DF_PROPERTY_EX(PS_EmitterMesh, mFilename, "Mesh", "Filename", "PT_Filename mesh(*.mesh)|*.mesh|", PT_String)
		DF_PROPERTY(PS_EmitterMesh, mScale, "Mesh", "Scale", PT_Float3)
		DF_PROPERTY(PS_EmitterMesh, mRandom, "Mesh", "Random", PT_Bool)
	DF_PROPERTY_END()

	PS_EmitterMesh::PS_EmitterMesh()
		: mScale(1, 1, 1)
		, mRandom(true)
	{
		mIndex = 0;
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
			PS_Emitter::InitParticle(p);

			_randomOnMeshVertex(p->Position, p->Direction);
		}
	}

	void PS_EmitterMesh::_randomOnMeshVertex(Float3 & position, Float3 & direction)
	{
		RenderOp * rop = mMesh->GetMeshBuffer(0)->GetRenderOp();

		int stride = rop->vertexBuffers[0]->GetStride();
		int vcount = rop->vertexBuffers[0]->GetCount();
		int index = mRandom ? index = Math::RandRange(0, vcount - 1) : mIndex;

		d_assert (
			rop->vertexDeclarations[0].HasElement(eVertexSemantic::POSITION, eVertexType::FLOAT3) &&
			rop->vertexDeclarations[0].HasElement(eVertexSemantic::NORMAL, eVertexType::FLOAT3));

		char * vdata = (char * )rop->vertexBuffers[0]->Lock(eLockFlag::READ) + stride * index;
		{
			Float3 p = *((Float3 *)vdata);
			Float3 n = *((Float3 *)vdata + 1);

			position = mPosition + p * mScale;

			Float3 common(1, 0, 0);
			if (Math::Equal(common.Dot(n), 0.0f))
			{
				common = Float3(0, 1, 0);
			}

			common = Float3::Cross(common, n);

			_randomDirection(direction, n, common);
		}
		rop->vertexBuffers[0]->Unlock();

		mIndex = Min(mIndex + 1, vcount - 1);
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
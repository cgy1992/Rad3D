#include "PSMeshSet.h"

namespace Rad {

	DF_ENUM_BEGIN(PS_MeshType)
		DF_ENUM(FILE_NAME)
		DF_ENUM(SPHERE)
		DF_ENUM(CLINDER)
		DF_ENUM(UP_CONE)
		DF_ENUM(DN_CONE)
	DF_ENUM_END()

	ImplementRTTI(PS_MeshSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_MeshSet)
		DF_PROPERTY_EX(PS_MeshSet, mShaderClass, "Shader", "Class", "PT_Class PS_Shader", PT_FixedString)
		DF_PROPERTY(PS_MeshSet, mShaderEnable, "Shader", "Enable", PT_Bool)
		DF_PROPERTY_ENUM(PS_MeshSet, mBlendMode, "Shader", "BlendType", PS_BlendMode)
		DF_PROPERTY_ENUM(PS_MeshSet, mType, "Mesh", "Type", PS_MeshType)
		DF_PROPERTY(PS_MeshSet, mCenter, "Mesh", "Center", PT_Float3)
		DF_PROPERTY(PS_MeshSet, mRadius, "Mesh", "Radius", PT_Float)
		DF_PROPERTY(PS_MeshSet, mHeight, "Mesh", "Height", PT_Float)
		DF_PROPERTY(PS_MeshSet, mRings, "Mesh", "Rings", PT_Int)
		DF_PROPERTY(PS_MeshSet, mSegments, "Mesh", "Segments", PT_Int)
		DF_PROPERTY_EX(PS_MeshSet, mFilename, "Mesh", "Filename", "PT_Filename mesh(*.mesh)|*.mesh|", PT_String)
		DF_PROPERTY_EX(PS_MeshSet, mTexture, "Mesh", "Texture", "PT_Filename", PT_String)
	DF_PROPERTY_END()

	void PS_MeshSet::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mShaderClass")
		{
			SetShaderClass(mShaderClass);
		}
		else if (p->name == "mFilename")
		{
			SetFilename(mFilename);
		}
		else if (p->name == "mTexture")
		{
			SetTexture(mTexture);
		}
		else
		{
			PS_Set::OnPropertyChanged(p);
		}
	}

	PS_MeshSet::PS_MeshSet()
	{
		mShaderEnable = true;
		mBlendMode = PS_BlendMode::ADD;
		mType = PS_MeshType::FILE_NAME;
		mCenter = Float3::Zero;
		mRadius = UNIT_METRES * 0.1f;
		mHeight = UNIT_METRES * 0.1f;
		mRings = 16;
		mSegments = 16;

		SetShaderClass("PS_ShaderMesh");
	}

	PS_MeshSet::~PS_MeshSet()
	{
		_clear_particle();
	}

	void PS_MeshSet::AllocQuota(int quota)
	{
	}

	void PS_MeshSet::AddRenderQueue(RenderQueue * rq)
	{
		if (mSorting && mParticles.Size())
		{
			Camera * pCamera = World::Instance()->GetCurrentRenderContext()->GetCamera();

			PS_SortOp op;
			op.mRender = this;
			op.mCameraPosition = pCamera->GetWorldPosition();

			Sort(&mParticles[0], mParticles.Size(), op);
		}
	}

	void PS_MeshSet::Update(float elapsedTime)
	{
		PS_Set::Update(elapsedTime);

		if (mParticles.Size() > 0)
		{
			mBound = Aabb::Invalid;

			for (int i = 0; i < mParticles.Size(); ++i)
			{
				Particle * p = mParticles[i];

				if (p->InternalData)
				{
					PS_Mesh * data = (PS_Mesh *)p->InternalData;

					data->_update(p);
				}

				Float3 vMin = p->Position - p->Size * 0.5f;
				Float3 vMax = p->Position + p->Size * 0.5f;

				mBound.minimum = Float3::Minimum(mBound.minimum, vMin);
				mBound.maximum = Float3::Maximum(mBound.maximum, vMax);
			}

			if (IsLocalSpace())
			{
				mBound.Transform(mParent->GetWorldTM());
			}
		}
	}

	void PS_MeshSet::SetShaderClass(const FixedString32 & name)
	{
		mShaderClass = name;

		mShader = NULL;
		if (mShaderClass != "")
		{
			mShader = Root::NEW_OBJECT_T<PS_Shader>(mShaderClass.c_str());
		}
	}

	void PS_MeshSet::SetFilename(const String & name)
	{
		mFilename = name;

		i_mesh = MeshManager::Instance()->LoadMesh(name);
	}

	void PS_MeshSet::SetTexture(const String & name)
	{
		mTexture = name;

		i_texture = HWBufferManager::Instance()->LoadTexture(mTexture);
	}

	Particle * PS_MeshSet::_quest_particle()
	{
		Particle * p = PS_Set::_quest_particle();
		if (p)
		{
			Mesh * mesh = _createMesh();
			if (mesh != NULL)
			{
				if (mTexture != "")
				{
					for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
					{
						mesh->GetSubMesh(i)->GetMaterial()->maps[eMapType::DIFFUSE] = i_texture;
					}
				}

				p->InternalData = new PS_Mesh(mesh, this);
			}
		}

		return p;
	}

	void PS_MeshSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_Mesh * data = (PS_Mesh *)p->InternalData;

			delete data;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_MeshSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_Mesh * data = (PS_Mesh *)p->InternalData;

				delete data;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

	Mesh * PS_MeshSet::_createMesh()
	{
		Mesh * mesh = NULL;
		
		switch (mType)
		{
		case PS_MeshType::FILE_NAME:
			mesh = new Mesh;
			mesh->SetSource(i_mesh);
			break;

		case PS_MeshType::SPHERE:
			mesh = _createSphere();
			break;

		case PS_MeshType::CLINDER:
			mesh = _createClinder();
			break;

		case PS_MeshType::UP_CONE:
			mesh = _createCone(true);
			break;

		case PS_MeshType::DN_CONE:
			mesh = _createCone(false);
			break;
		}

		if (mesh)
			mesh->SetRenderContextId(mParent->GetRenderContextId());

		return mesh;
	}

	Mesh * PS_MeshSet::_createSphere()
	{
		Float3 offset = mCenter;
		int rings = mRings;
		int segments = mSegments;
		float radius = mRadius;

		if (rings < 1 || segments < 1)
			return NULL;

		Mesh * pMesh = new Mesh;
		SubMesh * sm = pMesh->NewSubMesh();

		int iVertexCount = (rings + 1) * (segments + 1);
		int iIndexCount = rings * segments * 6;
		int iPrimCount = iIndexCount / 3;

		d_assert(iIndexCount < 65536);

		sm->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		sm->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		VertexBufferPtr buffer = HWBufferManager::Instance()->NewVertexBuffer(20, iVertexCount);

		float * vert = (float *)buffer->Lock(eLockFlag::WRITE);
		{
			float fTileRingAngle = (PI / rings);
			float fTileSegAngle = (PI * 2 / segments);
			float u_step = 1 / (float)rings;
			float v_step = 1 / (float)segments;
			float r;
			short i, j;
			Float3 pos;

			for (i = 0; i <= rings; ++i)
			{
				r = radius * Math::Sin(i * fTileRingAngle);
				pos.y = radius * Math::Cos(i * fTileRingAngle);

				for (j = 0; j <= segments; ++j)
				{
					pos.x = r * Math::Cos(j * fTileSegAngle);
					pos.z = r * Math::Sin(j * fTileSegAngle);

					*vert++ = pos.x + offset.x;
					*vert++ = pos.y + offset.y;
					*vert++ = pos.z + offset.z;
					*vert++ = i * u_step;
					*vert++ = j * v_step;
				}

			}
		}
		buffer->Unlock();

		IndexBufferPtr ibuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);
		short * indices = (short *)ibuffer->Lock(eLockFlag::WRITE);
		{
			short row = 0, row_n = 0;
			short i, j;

			for (i = 0; i < rings; ++i)
			{
				row_n = row + segments + 1;

				for (j = 0; j < segments; ++j)
				{
					*indices++ = row + j;
					*indices++ = row + j + 1;
					*indices++ = row_n + j;

					*indices++ = row_n + j;
					*indices++ = row + j + 1;
					*indices++ = row_n + j + 1;
				}

				row += segments + 1;
			}
		}
		ibuffer->Unlock();

		sm->GetRenderOp()->vertexBuffers[0] = buffer;
		sm->GetRenderOp()->indexBuffer = ibuffer;
		sm->GetRenderOp()->primCount = iPrimCount;
		sm->GetRenderOp()->primType= ePrimType::TRIANGLE_LIST;

		sm->GetMaterial()->maps[eMapType::DIFFUSE] = RenderHelper::Instance()->GetWhiteTexture();

		pMesh->SetLocalAabb(Aabb(Float3(-radius, -radius, -radius) + offset, Float3(radius, radius, radius) + offset));

		return pMesh;
	}

	Mesh * PS_MeshSet::_createClinder()
	{
		Float3 offset = mCenter;
		int rings = mRings;
		float radius = mRadius;
		float height = mHeight;

		if (rings < 1)
			return NULL;

		Mesh * pMesh = new Mesh;
		SubMesh * sm = pMesh->NewSubMesh();

		int iVertexCount = (rings + 1) * 2;
		int iIndexCount = rings * 6;
		int iPrimCount = iIndexCount / 3;

		d_assert(iIndexCount < 65536);

		sm->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		sm->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		VertexBufferPtr buffer = HWBufferManager::Instance()->NewVertexBuffer(20, iVertexCount);

		float * vert = (float *)buffer->Lock(eLockFlag::WRITE);
		{
			float r_step = (2 * PI / rings);
			float u_step = 1 / (float)rings;
			float x, z, rads;

			for (int i = 0; i <= rings; ++i)
			{
				rads = i * r_step;

				Math::SinCos(rads, z, x);

				x *= radius;
				z *= radius;

				*vert++ = x + offset.x;
				*vert++ = 0 + offset.y;
				*vert++ = z + offset.z;
				*vert++ = i * u_step;
				*vert++ = 1;

				*vert++ = x + offset.x;
				*vert++ = height + offset.y;
				*vert++ = z + offset.z;
				*vert++ = i * u_step;
				*vert++ = 0;
			}
		}
		buffer->Unlock();

		IndexBufferPtr ibuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);
		short * indices = (short *)ibuffer->Lock(eLockFlag::WRITE);
		{
			for (short i = 0; i < rings; ++i)
			{
				int j = i * 2;

				*indices++ = j;
				*indices++ = j + 1;
				*indices++ = j + 2;

				*indices++ = j + 2;
				*indices++ = j + 1;
				*indices++ = j + 3;
			}
		}
		ibuffer->Unlock();

		sm->GetRenderOp()->vertexBuffers[0] = buffer;
		sm->GetRenderOp()->indexBuffer = ibuffer;
		sm->GetRenderOp()->primType = ePrimType::TRIANGLE_LIST;
		sm->GetRenderOp()->primCount = iPrimCount;

		sm->GetMaterial()->cullMode = eCullMode::NONE;
		sm->GetMaterial()->maps[eMapType::DIFFUSE] = RenderHelper::Instance()->GetWhiteTexture();

		pMesh->SetLocalAabb(Aabb(Float3(-radius, 0, -radius) + offset, Float3(radius, height, radius) + offset));

		return pMesh;
	}

	Mesh * PS_MeshSet::_createCone(bool up)
	{
		Float3 offset = mCenter;
		int rings = (mRings + 3) / 4;
		int segments = mSegments;
		float radius = mRadius;
		float height = mHeight;

		if (rings < 1)
			return NULL;

		Mesh * pMesh = new Mesh;
		SubMesh * sm = pMesh->NewSubMesh();

		float h1 = up ? 0 : height;
		float h2 = up ? height : 0;

		int iVertexCount = 1 + (rings * 4 + 1);
		int iIndexCount = rings * 4 * 3;
		int iPrimCount = iIndexCount / 3;

		d_assert(iIndexCount < 65536);

		sm->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		sm->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		VertexBufferPtr buffer = HWBufferManager::Instance()->NewVertexBuffer(20, iVertexCount);

		float * vert = (float *)buffer->Lock(eLockFlag::WRITE);
		{
			float r_step = (PI2 / (rings * 4));
			float d_step = (radius * 2 / rings);
			float u_step = 1 / (float)rings;
			float v_step = u_step;
			float x, z;
			float rads = -PI * 0.25f;

			*vert++ = 0 + offset.x;
			*vert++ = h1 + offset.x;
			*vert++ = 0 + offset.z;
			*vert++ = 0.5f;
			*vert++ = 0.5f;

			// top
			for (int i = 0; i <= rings; ++i)
			{
				Math::SinCos(rads, z, x);

				x *= radius;
				z *= radius;

				*vert++ = x + offset.x;
				*vert++ = h2 + offset.y;
				*vert++ = z + offset.z;
				*vert++ = i * u_step;
				*vert++ = 0;

				rads += r_step;
			}

			// right
			for (int i = 1; i <= rings; ++i)
			{
				Math::SinCos(rads, z, x);

				x *= radius;
				z *= radius;

				*vert++ = x + offset.x;
				*vert++ = h2 + offset.y;
				*vert++ = z + offset.z;
				*vert++ = 1;
				*vert++ = i * v_step;

				rads += r_step;
			}

			// bottom
			for (int i = 1; i <= rings; ++i)
			{
				Math::SinCos(rads, z, x);

				x *= radius;
				z *= radius;

				*vert++ = x + offset.x;
				*vert++ = h2 + offset.y;
				*vert++ = z + offset.z;
				*vert++ = 1 - i * u_step;
				*vert++ = 1;

				rads += r_step;
			}

			// left
			for (int i = 1; i <= rings; ++i)
			{
				Math::SinCos(rads, z, x);

				x *= radius;
				z *= radius;

				*vert++ = x + offset.x;
				*vert++ = h2 + offset.y;
				*vert++ = z + offset.z;
				*vert++ = 0;
				*vert++ = 1 - i * u_step;

				rads += r_step;
			}
		}
		buffer->Unlock();

		IndexBufferPtr ibuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);
		short * indices = (short *)ibuffer->Lock(eLockFlag::WRITE);
		{
			for (short i = 0; i < rings * 4; ++i)
			{
				*indices++ = i + 1;
				*indices++ = 0;
				*indices++ = i + 2;
			}
		}
		ibuffer->Unlock();

		sm->GetRenderOp()->vertexBuffers[0] = buffer;
		sm->GetRenderOp()->indexBuffer = ibuffer;
		sm->GetRenderOp()->primType = ePrimType::TRIANGLE_LIST;
		sm->GetRenderOp()->primCount = iPrimCount;

		sm->GetMaterial()->cullMode = eCullMode::NONE;
		sm->GetMaterial()->maps[eMapType::DIFFUSE] = RenderHelper::Instance()->GetWhiteTexture();

		pMesh->SetLocalAabb(Aabb(Float3(-radius, 0, -radius) + offset, Float3(radius, height, radius) + offset));

		return pMesh;
	}

}
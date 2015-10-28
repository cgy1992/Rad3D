#include "MMeshSerializer.h"
#include "MMeshManager.h"
#include "MHWBufferManager.h"
#include "MResourceManager.h"
#include "MHWBufferManager.h"
#include "MRenderHelper.h"
#include "MAnimationSerializer.h"

namespace Rad {

	void MeshSerializer::Load(MeshSource * mesh, DataStreamPtr stream)
	{
		char magic[MODEL_FILE_MAGIC_LEN];
		int version;

		stream->Read(magic, MODEL_FILE_MAGIC_LEN);

		d_assert (strcmp(magic, MODEL_FILE_MAGIC) == 0 || strcmp(magic, "Myway Model") == 0);

		stream->Read(&version, sizeof(int));

		d_assert (version == K_File_Verion);

		int ckId = -1;
		while (stream->Read(&ckId, sizeof(int)) == sizeof(int))
		{
			switch (ckId)
			{
			case MC_MESH_BUFFER:
				ReadMeshBuffer(mesh, mesh->NewMeshBuffer(), stream);
				break;

			case MC_MESH_SHADER:
				ReadMeshShader(mesh, mesh->GetMeshBuffer(mesh->GetMeshBufferCount() - 1), stream);
				break;

			case MC_BOUND:
				ReadBound(mesh, stream);
				break;

			case MC_SKELETON:
				ReadSkeleton(mesh, stream);
				break;

			case MC_ANIMATION:
				AnimationSerializer::ReadAnimation(mesh, NULL, stream);
				break;

			case MC_ANIMATION_EVENT:
				AnimationSerializer::ReadAnimationEvent(mesh->GetAnimation(mesh->GetAnimationCount() - 1), stream);
				break;
			}
		}
	}

	void MeshSerializer::ReadMeshBuffer(MeshSource * mesh, MeshBuffer * mb, DataStreamPtr stream)
	{
		int version;
		stream->Read(&version, sizeof(int));

		d_assert (
			version == K_MeshBuffer_V0 || 
			version == K_MeshBuffer_V1);

		int vertexCount;
		int indexCount;
		int vertexElems;

		stream->Read(&vertexCount, sizeof(int));
		stream->Read(&indexCount, sizeof(int));
		stream->Read(&vertexElems, sizeof(int));

		d_assert (vertexElems | VE_POSITION);

		RenderOp * rop = mb->GetRenderOp();
		
		rop->primCount = indexCount / 3;
		rop->primType = ePrimType::TRIANGLE_LIST;

		// vertex buffer
		int stride = GenVertexDecl(rop, vertexElems);

		VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(stride , vertexCount);
		void * vdata = vb->Lock(eLockFlag::WRITE);
		{
			stream->Read(vdata, stride * vertexCount);
		}
		vb->Unlock();

		rop->vertexBuffers[0] = vb;

		// index buffer
		IndexBufferPtr ib = HWBufferManager::Instance()->NewIndexBuffer(indexCount);
		void * idata = ib->Lock(eLockFlag::WRITE);
		{
			stream->Read(idata, ib->GetStride() * indexCount);
		}
		ib->Unlock();

		rop->indexBuffer = ib;

		// bone map
		if (version >= K_MeshBuffer_V1)
		{
			int numBoneMap = 0;
			stream->Read(&numBoneMap, sizeof(int));

			if (numBoneMap > 0)
			{
				Array<short> & boneMap = mb->GetBoneIdMap();
				boneMap.Resize(numBoneMap);

				stream->Read(&boneMap[0], sizeof(short) * numBoneMap);
			}
		}

		ReadMeshMaterial(mesh, mb, stream);
	}

	void MeshSerializer::ReadMeshMaterial(MeshSource * mesh, MeshBuffer * mb, DataStreamPtr stream)
	{
		Material * mtl = mb->GetMaterial();

		int mtl_ver = -1;
		stream->Read(&mtl_ver, sizeof(int));

		d_assert (
			mtl_ver == K_Material_V0 ||
			mtl_ver == K_Material_V1 ||
			mtl_ver == K_Material_V2);

		int doubleSide, opacity;
		stream->Read(&doubleSide, sizeof(int));
		stream->Read(&opacity, sizeof(int));

		if (doubleSide == 1)
			mtl->cullMode = eCullMode::NONE;

		if (opacity == 1)
			mtl->blendMode = eBlendMode::ALPHA_TEST;
		else if (opacity == 2)
			mtl->blendMode = eBlendMode::ALPHA_BLEND;

		if (mtl->blendMode == eBlendMode::ALPHA_BLEND)
			mtl->depthMode = eDepthMode::N_LESS_EQUAL;

		if (mtl_ver < K_Material_V2)
		{
			Float4 emissive, ambient, diffuse, specular;
			float specularPower;

			stream->Read(&emissive, sizeof(float) * 4);
			stream->Read(&ambient, sizeof(float) * 4);
			stream->Read(&diffuse, sizeof(float) * 4);
			stream->Read(&specular, sizeof(float) * 4);
			stream->Read(&specularPower, sizeof(float));

			mtl->emissive = Float3(emissive.r, emissive.g, emissive.b);
			mtl->ambient = Float3(ambient.r, ambient.g, ambient.b);
			mtl->diffuse = Float3(diffuse.r, diffuse.g, diffuse.b);
			mtl->specular = Float3(specular.r, specular.g, specular.b);
			mtl->shininess = specularPower;
		}
		else
		{
			stream->Read(&mtl->emissive, sizeof(float) * 3);
			stream->Read(&mtl->ambient, sizeof(float) * 3);
			stream->Read(&mtl->diffuse, sizeof(float) * 3);
			stream->Read(&mtl->specular, sizeof(float) * 3);
			stream->Read(&mtl->shininess, sizeof(float));
		}

		char emissiveMap[128], diffuseMap[128], normalMap[128], specularMap[128];
		if (mtl_ver == K_Material_V0)
		{
			stream->Read(&emissiveMap[0], 128);
			stream->Read(&diffuseMap[0], 128);
			stream->Read(&normalMap[0], 128);
			stream->Read(&specularMap[0], 128);
		}
		else
		{
			stream->ReadString(&emissiveMap[0], 128);
			stream->ReadString(&diffuseMap[0], 128);
			stream->ReadString(&normalMap[0], 128);
			stream->ReadString(&specularMap[0], 128);
		}

		if (emissiveMap != "")
		{
			const String & filename = emissiveMap;

			TexturePtr pTexture = HWBufferManager::Instance()->LoadTexture(filename, 0);
			if (pTexture->GetLoadState() != Resource::LOADED)
			{
				mesh->_addDependence(pTexture.c_ptr());
			}

			mtl->maps[eMapType::EMISSIVE] = pTexture;
		}

		if (diffuseMap != "")
		{
			const String & filename = diffuseMap;

			TexturePtr pTexture = HWBufferManager::Instance()->LoadTexture(filename, 0);
			if (pTexture != NULL && pTexture->GetLoadState() != Resource::LOADED)
			{
				mesh->_addDependence(pTexture.c_ptr());
			}

			mtl->maps[eMapType::DIFFUSE] = pTexture;
		}

		if (normalMap != "")
		{
			const String & filename = normalMap;

			TexturePtr pTexture = HWBufferManager::Instance()->LoadTexture(filename, 0);
			if (pTexture != NULL && pTexture->GetLoadState() != Resource::LOADED)
			{
				mesh->_addDependence(pTexture.c_ptr());
			}

			mtl->maps[eMapType::NORMAL] = pTexture;
		}

		if (specularMap != "")
		{
			const String & filename = specularMap;

			TexturePtr pTexture = HWBufferManager::Instance()->LoadTexture(filename, 0);
			if (pTexture != NULL && pTexture->GetLoadState() != Resource::LOADED)
			{
				mesh->_addDependence(pTexture.c_ptr());
			}

			mtl->maps[eMapType::SPECULAR] = pTexture;
		}

		if (mtl->maps[eMapType::DIFFUSE] == NULL)
			mtl->maps[eMapType::DIFFUSE] = RenderHelper::Instance()->GetWhiteTexture();
	}

	void MeshSerializer::ReadMeshShader(MeshSource * mesh, MeshBuffer * mb, DataStreamPtr stream)
	{
		int nameLen, dataLen;

		stream->Read(&nameLen, sizeof(int));
		stream->Read(&dataLen, sizeof(int));

		d_assert (nameLen > 0);

		char name[32];
		stream->Read(name, nameLen);
		name[nameLen] = 0;

		if (mb->SetShaderClass(name))
		{
			ISerializerD IS(stream);
			mb->GetShader()->Serialize(IS);
		}
		else
		{
			stream->Skip(dataLen);
		}
	}

	void MeshSerializer::ReadSkeleton(MeshSource * mesh, DataStreamPtr stream)
	{
		int version;
		stream->Read(&version, sizeof (int));

		d_assert (
			version == K_Skeleton_V0 ||
			version == K_Skeleton_V1 ||
			version == K_Skeleton_V2);

		int boneCount = 0;
		stream->Read(&boneCount, sizeof (int));

		for (int i = 0; i < boneCount; ++i)
		{
			char Name[128];
			if (version >= K_Skeleton_V2)
			{
				stream->ReadString(&Name[0], 128);
				d_assert(strlen(Name) < 32);
			}
			else
			{
				stream->Read(&Name[0],  128);
			}

			joint * bn = mesh->NewJoint(Name);
			stream->Read(&bn->flag, sizeof(int));
			stream->Read(&bn->position, sizeof(float) * 3);
			stream->Read(&bn->rotation, sizeof(float) * 4);
			stream->Read(&bn->scale, sizeof(float) * 3);
		}

		int linkCount = boneCount;
		if (version >= K_Skeleton_V1)
		{
			stream->Read(&linkCount, sizeof(int));
		}

		for (int i = 0; i < linkCount; ++i)
		{
			short parent, child;

			stream->Read(&parent, sizeof(short));
			stream->Read(&child, sizeof(short));

			d_assert (parent < boneCount && child < boneCount);

			mesh->SetJointLink(parent, child);
		}
	}

	void MeshSerializer::ReadBound(MeshSource * mesh, DataStreamPtr stream)
	{
		int iVersion;
		stream->Read(&iVersion, sizeof(int));

		d_assert (
			iVersion == K_Bound_V0 ||
			iVersion == K_Bound_V1);

		Float3 vMin, vMax;
		stream->Read(&vMin, sizeof(float) * 3);
		stream->Read(&vMax, sizeof(float) * 3);

		mesh->SetAabb(Aabb(vMin, vMax));

		if (iVersion == K_Bound_V0)
		{
			Float3 vCenter;
			float fRadius;
			// ignore.
			stream->Read(&vCenter, sizeof(float) * 3);
			stream->Read(&fRadius, sizeof(float));
		}
	}

	int MeshSerializer::GenVertexDecl(RenderOp * rop, int vertexElems)
	{
		rop->vertexDeclarations[0].Clear();	

		if (vertexElems & VE_POSITION)
		{
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		}

		if (vertexElems & VE_NORMAL)
		{
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::NORMAL, eVertexType::FLOAT3);
		}

		if (vertexElems & VE_COLOR)
		{
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::COLOR, eVertexType::UBYTE4);
		}

		if (vertexElems & VE_TANGENT)
		{
			d_assert (0);
		}

		if (vertexElems & VE_TEXCOORD)
		{
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);
		}

		if (vertexElems & VE_LIGHTMAPUV)
		{
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD1, eVertexType::FLOAT2);
		}

		if (vertexElems & VE_BONEINDEX)
		{
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::BONE_INDEX, eVertexType::UBYTE4);
		}

		if (vertexElems & VE_BONEWEIGHT)
		{
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::BONE_WEIGHT, eVertexType::FLOAT4);
		}

		return rop->vertexDeclarations[0].GetVertexSize();
	}

	bool MeshSerializer::Save(MeshSource * mesh, const String & filename)
	{
		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
			return false;

		OSerializerF stream(fp, true);

		stream.Write(MODEL_FILE_MAGIC, MODEL_FILE_MAGIC_LEN);

		int version = K_File_Verion;
		stream.Write(&version, sizeof(int));

		for (int i = 0; i < mesh->GetMeshBufferCount(); ++i)
		{
			MeshBuffer * mb = mesh->GetMeshBuffer(i);
			if (mb->GetRenderOp()->indexBuffer == NULL)
				continue ;
			
			WriteMeshBuffer(mb, &stream);

			WriteMeshMaterial(mb, &stream);

			WriteMeshShader(mb, &stream);
		}

		WriteBound(mesh, &stream);

		WriteSkeleton(mesh, &stream);

		for (int i = 0; i < mesh->GetAnimationCount(); ++i)
		{
			AnimationSerializer::WriteAnimation(mesh, mesh->GetAnimation(i), &stream);
		}

		return true;
	}

	void MeshSerializer::WriteMeshBuffer(MeshBuffer * mb, OSerializer * stream)
	{
		int iVertexCount = mb->GetRenderOp()->vertexBuffers[0]->GetCount();
		int iIndexCount = mb->GetRenderOp()->indexBuffer->GetCount();
		int iVertexElems = 0;
		int iVertexSize = 0;

		if (mb->GetRenderOp()->vertexDeclarations[0].HasElement(eVertexSemantic::POSITION, eVertexType::FLOAT3))
		{
			iVertexElems |= VE_POSITION;
			iVertexSize += 12;
		}

		if (mb->GetRenderOp()->vertexDeclarations[0].HasElement(eVertexSemantic::NORMAL, eVertexType::FLOAT3))
		{
			iVertexElems |= VE_NORMAL;
			iVertexSize += 12;
		}

		if (mb->GetRenderOp()->vertexDeclarations[0].HasElement(eVertexSemantic::COLOR, eVertexType::BYTE4))
		{
			iVertexElems |= VE_COLOR;
			iVertexSize += 4;
		}

		if (mb->GetRenderOp()->vertexDeclarations[0].HasElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2))
		{
			iVertexElems |= VE_TEXCOORD;
			iVertexSize += 8;
		}

		if (mb->GetRenderOp()->vertexDeclarations[0].HasElement(eVertexSemantic::TEXCOORD1, eVertexType::FLOAT2))
		{
			iVertexElems |= VE_LIGHTMAPUV;
			iVertexSize += 8;
		}

		if (mb->GetRenderOp()->vertexDeclarations[0].HasElement(eVertexSemantic::BONE_INDEX, eVertexType::UBYTE4))
		{
			iVertexElems |= VE_BONEINDEX;
			iVertexSize += 4;
		}

		if (mb->GetRenderOp()->vertexDeclarations[0].HasElement(eVertexSemantic::BONE_WEIGHT, eVertexType::FLOAT4))
		{
			iVertexElems |= VE_BONEWEIGHT;
			iVertexSize += 16;
		}

		d_assert (iVertexSize == mb->GetRenderOp()->vertexBuffers[0]->GetStride());
		if (iVertexSize != mb->GetRenderOp()->vertexBuffers[0]->GetStride())
			return ;

		int ckId = MC_MESH_BUFFER;
		int version = K_MeshBuffer_V1;
		stream->Write(&ckId, sizeof(int));
		stream->Write(&version, sizeof(int));

		stream->Write(&iVertexCount, sizeof(int));
		stream->Write(&iIndexCount, sizeof(int));
		stream->Write(&iVertexElems, sizeof(int));

		void * vertexData = mb->GetRenderOp()->vertexBuffers[0]->Lock(eLockFlag::READ);
		stream->Write(vertexData, iVertexSize * iVertexCount);
		mb->GetRenderOp()->vertexBuffers[0]->Unlock();

		void * indexData = mb->GetRenderOp()->indexBuffer->Lock(eLockFlag::READ);
		stream->Write(indexData, sizeof(short) * iIndexCount);
		mb->GetRenderOp()->indexBuffer->Unlock();

		int numBoneMap = mb->GetBoneIdMap().Size();
		stream->Write(&numBoneMap, sizeof(int));
		if (numBoneMap > 0)
		{
			stream->Write(&mb->GetBoneIdMap()[0], sizeof(short) * numBoneMap);
		}
	}

	void MeshSerializer::WriteMeshMaterial(MeshBuffer * mb, OSerializer * stream)
	{
		int verison = K_Material_V2;
		stream->Write(&verison, sizeof(int));

		Material * mtl = mb->GetMaterial();
		int doubleSide = mtl->cullMode == eCullMode::NONE;
		int opacity = 0;

		if (mtl->blendMode == eBlendMode::ALPHA_TEST)
		{
			opacity = 1;
		}
		else if (mtl->blendMode == eBlendMode::ALPHA_BLEND)
		{
			opacity = 2;
		}

		stream->Write(&doubleSide, sizeof(int));
		stream->Write(&opacity, sizeof(int));

		stream->Write(&mtl->emissive, sizeof(float) * 3);
		stream->Write(&mtl->ambient, sizeof(float) * 3);
		stream->Write(&mtl->diffuse, sizeof(float) * 3);
		stream->Write(&mtl->specular, sizeof(float) * 3);
		stream->Write(&mtl->shininess, sizeof(float));

		String emissiveMap, diffuseMap, normalMap, specularMap;
		if (mtl->maps[eMapType::EMISSIVE] != NULL)
			emissiveMap = mtl->maps[eMapType::EMISSIVE]->GetName();

		if (mtl->maps[eMapType::DIFFUSE] != NULL)
			diffuseMap = mtl->maps[eMapType::DIFFUSE]->GetName();

		if (mtl->maps[eMapType::NORMAL] != NULL)
			normalMap = mtl->maps[eMapType::NORMAL]->GetName();

		if (mtl->maps[eMapType::SPECULAR] != NULL)
			specularMap = mtl->maps[eMapType::SPECULAR]->GetName();

		stream->WriteString(&emissiveMap[0]);
		stream->WriteString(&diffuseMap[0]);
		stream->WriteString(&normalMap[0]);
		stream->WriteString(&specularMap[0]);
	}

	void MeshSerializer::WriteMeshShader(MeshBuffer * mb, OSerializer * stream)
	{
		MeshShaderPtr shader = mb->GetShader();
		if (shader != NULL)
		{
			int ckId = MC_MESH_SHADER;

			stream->Write(&ckId, sizeof(int));

			const char * className = shader->GetRTTI()->Name();
			OSerializerTester OST;
			shader->Serialize(OST);

			int nameLen = strlen(className);
			int dataLen = OST.Size();

			stream->Write(&nameLen, sizeof(int));
			stream->Write(&dataLen, sizeof(int));

			stream->Write(className, nameLen);

			shader->Serialize(*stream);
		}
	}

	void MeshSerializer::WriteSkeleton(MeshSource * mesh, OSerializer * stream)
	{
		if (mesh->GetJointCount() > 0)
		{
			int ckId = MC_SKELETON;
			int verison = K_Skeleton_V2;

			stream->Write(&ckId, sizeof(int));
			stream->Write(&verison, sizeof(int));

			int numJoint = mesh->GetJointCount();
			stream->Write(&numJoint, sizeof(int));

			for (int i = 0; i < numJoint; ++i)
			{
				joint * bn = mesh->GetJoint(i);

				stream->WriteString(&bn->name[0]);
				stream->Write(&bn->flag, sizeof(int));
				stream->Write(&bn->position, sizeof(float) * 3);
				stream->Write(&bn->rotation, sizeof(float) * 4);
				stream->Write(&bn->scale, sizeof(float) * 3);
			}

			int numLink = mesh->GetJointLinkCount();
			stream->Write(&numLink, sizeof(int));

			for (int i = 0; i < numLink; ++i)
			{
				joint_link * bp = mesh->GetJointLink(i);

				stream->Write(&bp->parent, sizeof(short));
				stream->Write(&bp->child, sizeof(short));
			}
		}
	}

	void MeshSerializer::WriteBound(MeshSource * mesh, OSerializer * stream)
	{
		int ckId = MC_BOUND;
		int version = K_Bound_V1;

		stream->Write(&ckId, sizeof(int));
		stream->Write(&version, sizeof(int));
		stream->Write(&mesh->GetAabb().minimum, sizeof(Float3));
		stream->Write(&mesh->GetAabb().maximum, sizeof(Float3));
	}

}
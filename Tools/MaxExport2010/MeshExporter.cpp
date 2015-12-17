#include "stdafx.h"
#include "MeshExporter.h"
#include "Utility.h"
#include "ExportConfig.h"
#include "MMeshSerializer.h"

MeshExporter::MeshExporter(IGameScene * scene)
	: mGameScene(scene)
{
}

MeshExporter::~MeshExporter()
{
}

void MeshExporter::Export()
{
	mRoot = new Root;
	mResourceManager = new ResourceManager;
	mRenderSystem = new NullRenderSystem;
	mWorld = new World;

	try 
	{
		IGameConversionManager* cm = GetConversionManager();
		cm->SetCoordSystem(IGameConversionManager::IGAME_D3D);
		mGameScene->InitialiseIGame(ExportConfig::Instance()->IsExportSelected());
		mGameScene->SetStaticFrame(0);

		int nodeCount = mGameScene->GetTopLevelNodeCount();
		if (nodeCount == 0)
		{
			MessageBox(GetActiveWindow(), "No nodes available!", "Error", MB_OK);
			goto __end;
		}

		mMeshSource = MeshManager::Instance()->NewMesh("MaxExporter");

		// extract skeleton
		for (int i = 0; i < nodeCount; ++i)
		{
			IGameNode* node = mGameScene->GetTopLevelNode(i);

			if (node->IsNodeHidden())
				continue ;

			ExtractSkeleton(node);
		}

		// extract mesh
		mMeshData.Clear();

		for (int i = 0; i < nodeCount; ++i)
		{
			IGameNode* node = mGameScene->GetTopLevelNode(i);

			if (node->IsNodeHidden())
				continue ;

			ExtractMesh(node);
		}

		if (mMMPairs.Size() == 0)
		{
			MessageBox(GetActiveWindow(), "No Objects!", "Error", MB_OK);
			goto __end;
		}

		BuildMesh();

		// save mesh
		MeshSerializer::Save(mMeshSource.c_ptr(), ExportConfig::Instance()->GetExportFilename());

		MessageBox(GetActiveWindow(), "Export OK!", "Info", MB_ICONINFORMATION);
	}
	catch (...) 
	{
		MessageBox(GetActiveWindow(), "Error!", "Error", MB_ICONEXCLAMATION);
	}

__end:
	mMeshSource = NULL;

	delete mWorld;
	delete mRenderSystem;
	delete mResourceManager;
	delete mRoot;
}

void MeshExporter::ExtractSkeleton(IGameNode * node)
{
	if(!node->IsGroupOwner())
		_dumpJoint(node);

	for(int i=0;i<node->GetChildCount();i++)
	{
		IGameNode * child = node->GetNodeChild(i);
		// we deal with targets in the light/camera section
		if(child->IsTarget())
			continue;

		ExtractSkeleton(child);
	}

	node->ReleaseIGameObject();
}

int MeshExporter::_getJointId(const char * name)
{
	for (int i = 0; i < mMeshSource->GetJointCount(); ++i)
	{
		if (mMeshSource->GetJoint(i)->name == name)
			return i;
	}

	return -1;
}

Animation * MeshExporter::_getAnimation()
{
	if (mMeshSource->GetAnimationCount() == 0)
	{
		Animation * anim = mMeshSource->NewAnimation("Default");

		float length = (float)ExportConfig::Instance()->GetFrames();
		length /= (float)GetFrameRate();

		anim->SetLength(length);
	}

	return mMeshSource->GetAnimation(0);
}

void MeshExporter::_dumpAnimation(IGameControl * pGameControl, int boneId)
{
	int start = ExportConfig::Instance()->GetFrameRange().x;
	int end = ExportConfig::Instance()->GetFrameRange().y;
	start *= GetTicksPerFrame();
	end *= GetTicksPerFrame();

	if ((pGameControl->IsAnimated(IGAME_POS)) || pGameControl->IsAnimated(IGAME_ROT) || pGameControl->IsAnimated(IGAME_SCALE))
	{
		IGameKeyTab Key;
		if(pGameControl->GetFullSampledKeys(Key, 1, IGAME_TM, true) )
		{
			SkeletonAnimation * skelAnim = new SkeletonAnimation(boneId);

			int count = Key.Count();
			for(int i=0;i<count;i++)
			{
				if (Key[i].t >= start && Key[i].t <= end)
				{
					float time = (float)Key[i].t / (float)GetTicksPerFrame() / (float)GetFrameRate();
					SkeletonAnimation::KeyFrame * kf = skelAnim->CreateKeyFrame(time);

					kf->position = Utility::ToFloat3(Key[i].sampleKey.gval.Translation());
					kf->rotation = Utility::ToQuat(Key[i].sampleKey.gval.Rotation());
					kf->scale = Utility::ToFloat3(Key[i].sampleKey.gval.Scaling());
				}
			}

			skelAnim->Optimize();
			_getAnimation()->AddSkeletonAnimation(skelAnim);
		}
	}
}

void MeshExporter::_dumpJoint(IGameNode * node)
{
	IGameObject * obj = node->GetIGameObject();
	IGameObject::MaxType T = obj->GetMaxType();
	IGameObject::ObjectTypes type = obj->GetIGameType();
	const char * name = node->GetName();
		
	switch(type)
	{
	case IGameObject::IGAME_BONE:
	case IGameObject::IGAME_HELPER:
		{
			joint * bone = mMeshSource->NewJoint(node->GetName());
			int parent = -1;

			if (node->GetNodeParent())
			{
				parent = _getJointId(node->GetNodeParent()->GetName());

				mMeshSource->SetJointLink(parent, mMeshSource->GetJointCount() - 1);
			}
			else
			{
				INode* pNode = node->GetMaxNode()->GetParentNode();
				if (pNode)
					parent = _getJointId(pNode->GetName());
			}
				
			IGameControl * pGameControl = node->GetIGameControl();
			// base matrix
			{
				GMatrix matWorld = node->GetLocalTM();
				bone->position = Utility::ToFloat3(matWorld.Translation());
				bone->rotation = Utility::ToQuat(matWorld.Rotation());
				bone->scale = Utility::ToFloat3(matWorld.Scaling());

				/*
				if (node->GetNodeParent())
				{
					int parentId = _getBoneId(node->GetNodeParent()->GetName());

					if (parentId != -1)
					{
						xBone * parentBn = GetBone(parentId);

						bone->position = bone->position - parentBn->position;
						bone->orientation = parentBn->orientation.Inverse() * bone->orientation;
						bone->scale = bone->scale / parentBn->scale;
					}
				}
				*/
			}

			_dumpAnimation(pGameControl, mMeshSource->GetJointCount() - 1);
		}

		break;
	}
}

void MeshExporter::ExtractMesh(IGameNode * node)
{
	IGameObject* obj = node->GetIGameObject();

	// export option
	bool expColor = ExportConfig::Instance()->IsExportColor();
	bool expNormal = ExportConfig::Instance()->IsExportNormal();
	bool expTexcoord = ExportConfig::Instance()->IsExportTexcoord();
	bool expLightmapUV = ExportConfig::Instance()->IsExportLightmapUV();

	obj->InitializeData();

	const char * nodeName = node->GetName();

	IGameNode * parent = node->GetNodeParent();
	IGameMesh::ObjectTypes type = obj->GetIGameType();

	if (type == IGameMesh::IGAME_MESH)
	{
		IGameMesh* mesh = (IGameMesh*) obj;
		Tab<int> texMaps = mesh->GetActiveMapChannelNum();

		mMeshData.VertexElems |= MeshSerializer::VE_POSITION;

		// position
		for (int i = 0; i < mesh->GetNumberOfVerts(); ++i)
			mMeshData.P.PushBack(Utility::ToFloat3(mesh->GetVertex(i)));

		// vertex color
		for (int i = 0; expColor && i < mesh->GetNumberOfColorVerts(); ++i)
		{
			Point3 c = mesh->GetColorVertex(i);
			float a = mesh->GetAlphaVertex(i);

			mMeshData.C.PushBack(Float4(c.x, c.y, c.z, a));
			mMeshData.VertexElems |= MeshSerializer::VE_COLOR;
		}

		// normal
		for (int i = 0; expNormal && i < mesh->GetNumberOfNormals(); ++i)
		{
			mMeshData.N.PushBack(Utility::ToFloat3(mesh->GetNormal(i)));
			mMeshData.VertexElems |= MeshSerializer::VE_NORMAL;
		}

		// uv
		for (int i = 0;  expTexcoord && texMaps.Count() > 1 && i < mesh->GetNumberOfMapVerts(texMaps[1]); ++i)
		{
			Point3 tv = mesh->GetMapVertex(texMaps[1], i);

			mMeshData.UV.PushBack(Float2(tv.x, 1 - tv.y));
			mMeshData.VertexElems |= MeshSerializer::VE_TEXCOORD;
		}

		// light map uv
		for (int i = 0;  expLightmapUV && texMaps.Count() > 2 && i < mesh->GetNumberOfMapVerts(texMaps[2]); ++i)
		{
			Point3 tv = mesh->GetMapVertex(texMaps[2], i);

			mMeshData.LUV.PushBack(Float2(tv.x, 1 - tv.y));
			mMeshData.VertexElems |= MeshSerializer::VE_LIGHTMAPUV;
		}

		IGameSkin * skin = obj->GetIGameSkin();
		if (skin != NULL)
			_dumpSkinInfo(skin);
		else if (parent != NULL && parent->GetIGameObject()->GetIGameType() == IGameMesh::IGAME_BONE)
			_genSkinInfo(parent);

		_dumpMeshBuffer(mesh);

		mMeshData.Clear();
	}

	for(int i=0;i<node->GetChildCount();i++)
	{
		IGameNode * child = node->GetNodeChild(i);
		// we deal with targets in the light/camera section
		if(child->IsTarget())
			continue;

		ExtractMesh(child);
	}

	node->ReleaseIGameObject();
}

void MeshExporter::_dumpSkinInfo(IGameSkin * skin)
{
	d_assert (skin->GetNumOfSkinnedVerts() == mMeshData.P.Size());

	for (int i = 0; i < skin->GetNumOfSkinnedVerts(); i++)
	{
		int type = skin->GetVertexType(i);

		Exporter::BlendIndex bi;
		Exporter::BlendWeight bw;

		if (type==IGameSkin::IGAME_RIGID_BLENDED)
		{
			for (int b = 0; b < skin->GetNumberOfBones(i) && b < 4; b++)
			{
				INode * bone = skin->GetBone(i, b);
				float weight = skin->GetWeight(i, b);

				if (bone)
				{
					const char * bname = bone->GetName();
					int boneId = _getJointId(bname);

					bi.i[b] = boneId;
					bw.w[b] = weight;
				}
			}
		}
		else if (type == IGameSkin::IGAME_RIGID)
		{
			INode * bone = skin->GetBone(i, 0);
			if (bone)
			{
				const char * bname = bone->GetName();
				int boneId = _getJointId(bname);

				bi.i[0] = boneId;
				bw.w[0] = 1;
			}
		}

		bw.normalize();

		mMeshData.BI.PushBack(bi);
		mMeshData.BW.PushBack(bw);
	}

	mMeshData.VertexElems |= MeshSerializer::VE_BONEINDEX;
	mMeshData.VertexElems |= MeshSerializer::VE_BONEWEIGHT;
}

void MeshExporter::_genSkinInfo(IGameNode * bone)
{
	int boneId = _getJointId(bone->GetName());
	if (boneId == -1)
		return ;

	for (int i = 0; i < mMeshData.P.Size(); i++)
	{
		Exporter::BlendIndex bi;
		Exporter::BlendWeight bw;

		bi.i[0] = boneId;
		bi.i[1] = boneId;
		bi.i[2] = boneId;
		bi.i[3] = boneId;

		bw.w[0] = 1;
		bw.w[1] = 0;
		bw.w[2] = 0;
		bw.w[3] = 0;

		mMeshData.BI.PushBack(bi);
		mMeshData.BW.PushBack(bw);
	}

	mMeshData.VertexElems |= MeshSerializer::VE_BONEINDEX;
	mMeshData.VertexElems |= MeshSerializer::VE_BONEWEIGHT;
}

void MeshExporter::_dumpMeshBuffer(IGameMesh * mesh)
{
	int iLastIndex = 0;

	for (int i = 0; i < mesh->GetNumberOfFaces(); ++i)
	{
		FaceEx * maxface = mesh->GetFace(i);
		IGameMaterial * mtl = mesh->GetMaterialFromFace(maxface);

		Exporter::Vertex face[3];
		_dumpFace(face, maxface);

		bool added = false;
		if (mMMPairs.Size() > 0 &&
			mMMPairs[iLastIndex].mtl == mtl &&
			mMMPairs[iLastIndex].mesh->AddFace(face, mMeshData.VertexElems))
		{
			added = true;
		}

		if (!added)
		{
			for (int j = 0; j < mMMPairs.Size(); ++j)
			{
				if (mMMPairs[j].mesh->AddFace(face, mMeshData.VertexElems))
				{
					added = true;
					iLastIndex = j;
					break;
				}
			}
		}

		if (!added)
		{
			MMPair mm;
			mm.mtl = mtl;
			mm.mesh = new Exporter::TriMesh(mMeshData.VertexElems);
			mm.mesh->AddFace(face, mMeshData.VertexElems);

			mMMPairs.PushBack(mm);
			iLastIndex = mMMPairs.Size() - 1;
		}
	}
}

void MeshExporter::_dumpFace(Exporter::Vertex * f, FaceEx * face)
{
	for (int j = 0; j < 3; ++j)
	{
		Exporter::Vertex & v = f[2 - j];

		v.mPosition = mMeshData.P[face->vert[j]];

		if (mMeshData.VertexElems & MeshSerializer::VE_NORMAL)
			v.mNormal = mMeshData.N[face->norm[j]];

		if (mMeshData.VertexElems & MeshSerializer::VE_COLOR)
			v.mColor = mMeshData.C[face->color[j]];

		if (mMeshData.VertexElems & MeshSerializer::VE_TEXCOORD)
			v.mTexcoord = mMeshData.UV[face->texCoord[j]];

		if (mMeshData.VertexElems & MeshSerializer::VE_LIGHTMAPUV)
			v.mLightmapUV = mMeshData.LUV[face->texCoord[j]];

		if (mMeshData.VertexElems & MeshSerializer::VE_BONEINDEX)
			v.mBlendIndex = mMeshData.BI[face->vert[j]];

		if (mMeshData.VertexElems & MeshSerializer::VE_BONEWEIGHT)
			v.mBlendWeight = mMeshData.BW[face->vert[j]];
	}
}

#define __vdata(d, s, size) { memcpy(d, s, size); d += size; }

void MeshExporter::BuildMesh()
{
	for (int i = 0; i < mMMPairs.Size(); ++i)
	{
		MeshBuffer * mb = mMeshSource->NewMeshBuffer();
		RenderOp * rop = mb->GetRenderOp();
		int vertexCount = mMMPairs[i].mesh->mVertexList.Size();
		int indexCount = mMMPairs[i].mesh->mIndexList.Size();
		int vertexElems = mMMPairs[i].mesh->mVertexElems;

		if (vertexElems & MeshSerializer::VE_POSITION)
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
			
		if (vertexElems & MeshSerializer::VE_NORMAL)
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::NORMAL, eVertexType::FLOAT3);
			
		if (vertexElems & MeshSerializer::VE_COLOR)
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::COLOR, eVertexType::UBYTE4);
			
		if (vertexElems & MeshSerializer::VE_TANGENT)
			d_assert (0);
			
		if (vertexElems & MeshSerializer::VE_TEXCOORD)
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);
			
		if (vertexElems & MeshSerializer::VE_LIGHTMAPUV)
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD1, eVertexType::FLOAT2);
			
		if (vertexElems & MeshSerializer::VE_BONEINDEX)
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::BONE_INDEX, eVertexType::UBYTE4);
			
		if (vertexElems & MeshSerializer::VE_BONEWEIGHT)
			rop->vertexDeclarations[0].AddElement(eVertexSemantic::BONE_WEIGHT, eVertexType::FLOAT4);

		int stride = rop->vertexDeclarations[0].GetVertexSize();

		VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(stride, vertexCount);
		char * vdata = (char *)vb->Lock(eLockFlag::WRITE);
		for (int k = 0; k < vertexCount; ++k)
		{
			const Exporter::Vertex & v = mMMPairs[i].mesh->mVertexList.GetVertex(k);

			if (vertexElems & MeshSerializer::VE_POSITION)
				__vdata(vdata, &v.mPosition, sizeof(Float3));

			if (vertexElems & MeshSerializer::VE_NORMAL)
				__vdata(vdata, &v.mNormal, sizeof(Float3));

			if (vertexElems & MeshSerializer::VE_COLOR)
				__vdata(vdata, &v.mColor, sizeof(Float4));

			if (vertexElems & MeshSerializer::VE_TANGENT)
				__vdata(vdata, &v.mTangent, sizeof(Float3));

			if (vertexElems & MeshSerializer::VE_TEXCOORD)
				__vdata(vdata, &v.mTexcoord, sizeof(Float2));

			if (vertexElems & MeshSerializer::VE_LIGHTMAPUV)
				__vdata(vdata, &v.mLightmapUV, sizeof(Float2));

			if (vertexElems & MeshSerializer::VE_BONEINDEX)
				__vdata(vdata, &v.mBlendIndex, sizeof(Exporter::BlendIndex));

			if (vertexElems & MeshSerializer::VE_BONEWEIGHT)
				__vdata(vdata, &v.mBlendWeight, sizeof(Exporter::BlendWeight));
		}
		vb->Unlock();

		mb->GetBoneIdMap() = mMMPairs[i].mesh->mBoneIdMap;

		IndexBufferPtr ib = HWBufferManager::Instance()->NewIndexBuffer(indexCount);
		unsigned short * idata = (unsigned short *)ib->Lock(eLockFlag::WRITE);
		for (int k = 0; k < indexCount; ++k)
		{
			idata[k] = mMMPairs[i].mesh->mIndexList[k];
		}
		ib->Unlock();

		mb->GetRenderOp()->vertexBuffers[0] = vb;
		mb->GetRenderOp()->indexBuffer = ib;
		mb->GetRenderOp()->primCount = mMMPairs[i].mesh->mIndexList.Size() / 3;
		mb->GetRenderOp()->primType = ePrimType::TRIANGLE_LIST;

		if (mMMPairs[i].mtl != NULL)
		{
			_dumpMaterial(mb->GetMaterial(), mMMPairs[i].mtl);
		}
	}

	mMeshSource->GenAabbFromRenderMesh();

	for (int i = 0; i < mMMPairs.Size(); ++i)
	{
		delete mMMPairs[i].mesh;
	}

	mMMPairs.Clear();
}

void MeshExporter::_dumpMaterial(Rad::Material * m, IGameMaterial * mtl)
{
	Point4 val4;
	Point3 val3;
	PropType pt;
	IGameProperty* p = NULL;

	p = mtl->GetAmbientData();
	if (p)
	{
		pt = p->GetType();
		if (pt == IGAME_POINT3_PROP && p->GetPropertyValue(val3)) 
		{
			m->ambient = Float3(val3.x, val3.y, val3.z);
		}
	}

	p = mtl->GetDiffuseData();
	if (p)
	{
		pt = p->GetType();
		if (pt == IGAME_POINT3_PROP && p->GetPropertyValue(val3))
		{
			m->diffuse = Float3(val3.x, val3.y, val3.z);
		}
	}

	p = mtl->GetSpecularData();
	if (p)
	{
		pt = p->GetType();
		if (pt == IGAME_POINT3_PROP && p->GetPropertyValue(val3))
		{
			m->specular = Float3(val3.x, val3.y, val3.z);
		}
	}

	p = mtl->GetSpecularLevelData();
	if (p)
	{
		float specularPower;
		if (p->GetPropertyValue(specularPower))
		{
			m->shininess = specularPower;
		}
	}

	p = mtl->GetEmissiveData();
	if (p)
	{
		pt = p->GetType();
		if (pt == IGAME_POINT3_PROP && p->GetPropertyValue(val3))
		{
			m->emissive = Float3(val3.x, val3.y, val3.z);
		}
	}

	Mtl * maxMtl = mtl->GetMaxMaterial();

	ULONG flag = maxMtl->Requirements(0);

	if (flag & MTLREQ_2SIDE)
	{
		m->cullMode = eCullMode::NONE;
	}

	d_assert (!mtl->IsMultiType());

	int numTextures = mtl->GetNumberOfTextureMaps();

	for ( int index = 0; index < numTextures; ++index )
	{
		IGameTextureMap * pMap = mtl->GetIGameTextureMap(index);

		if (!pMap)
			continue;

		const int type = pMap->GetStdMapSlot();
		const char * tex = pMap->GetBitmapFileName();
		const char * ttt = pMap->GetTextureName();
		std::string bmap = tex;
		bmap = bmap.substr(bmap.find_last_of('\\') + 1);

		if (type == ID_DI)
			m->maps[eMapType::DIFFUSE] = HWBufferManager::Instance()->LoadTexture(bmap.c_str());
		else if (type == ID_BU)
			m->maps[eMapType::NORMAL] = HWBufferManager::Instance()->LoadTexture(bmap.c_str());
		else if (type == ID_SP)
			m->maps[eMapType::SPECULAR] = HWBufferManager::Instance()->LoadTexture(bmap.c_str());
		else if (type == ID_SI)
			m->maps[eMapType::EMISSIVE] = HWBufferManager::Instance()->LoadTexture(bmap.c_str());

		if (type == ID_OP)
			m->blendMode = eBlendMode::ALPHA_TEST;

		TextureExporter::Instance()->Push(pMap->GetBitmapFileName());
	}
}

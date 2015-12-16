#include "stdafx.h"
#include "MeshImporter.h"

MeshImporter::MeshImporter()
{
}

MeshImporter::~MeshImporter()
{
}

bool MeshImporter::DoImport(const char * filename, Interface * gi)
{
	bool hr = false;

	Root::Config cfg;
	cfg.bRadC = false;

	mRoot = new Root(&cfg);
	mResourceManager = new ResourceManager;
	mRenderSystem = new NullRenderSystem;
	mWorld = new World;

	mResourceManager->AddArchive(new FilePathArchive(FileHelper::GetFileDir(filename)));

	mMeshSource = MeshManager::Instance()->LoadMesh(FileHelper::GetBaseName(filename));

	do
	{
		if (mMeshSource == NULL)
			break;

		if (mMeshSource->GetMeshBufferCount() == 0)
			break;

		if (!_impMesh(gi))
			break;

		hr = true;

	} while (0);
	
	mMeshSource = NULL;

	delete mWorld;
	delete mRenderSystem;
	delete mResourceManager;
	delete mRoot;

	return hr;
}

bool MeshImporter::_impMesh(Interface * gi)
{
	TriObject * triObj = new TriObject;
	INode * pNode = gi->CreateObjectNode((Object *)triObj);
	gi->GetRootNode()->AttachChild(pNode);
	
	static char buff[256];
	strcpy(buff, mMeshSource->GetSourceName().c_str());
	pNode->SetName(buff);

	::Mesh & mesh = ((TriObject*)pNode->GetObjectRef())->GetMesh();

	// set vertex
	int vertCount = 0;
	for (int i = 0; i < mMeshSource->GetMeshBufferCount(); ++i)
	{
		vertCount += mMeshSource->GetMeshBuffer(i)->GetRenderOp()->vertexBuffers[0]->GetCount();
	}

	mesh.setNumVerts(vertCount);
	mesh.setNumTVerts(vertCount);

	int iVertex = 0;
	for (int i = 0; i < mMeshSource->GetMeshBufferCount(); ++i)
	{
		MeshBuffer * mb = mMeshSource->GetMeshBuffer(i);

		int posoff = mb->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::POSITION);
		int nrmoff = mb->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::NORMAL);
		int uvoff = mb->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::TEXCOORD0);
		if (posoff == -1 || nrmoff == -1 || uvoff == -1)
			return false;

		int stride = mb->GetRenderOp()->vertexBuffers[0]->GetStride();
		byte * data = (byte *)mb->GetRenderOp()->vertexBuffers[0]->Lock(eLockFlag::READ);

		for (int j = 0; j < mb->GetRenderOp()->vertexBuffers[0]->GetCount(); ++j)
		{
			Float3 * p = (Float3 *)(data + posoff);
			Float3 * n = (Float3 *)(data + nrmoff);
			Float2 * t = (Float2 *)(data + uvoff);

			mesh.setVert(iVertex, p->x, p->y, p->z);
			mesh.setTVert(iVertex, t->x, t->y, 0.0f);
			mesh.setNormal(iVertex, Point3(n->x, n->y, n->z));

			iVertex += 1;
			data += stride;
		}

		mb->GetRenderOp()->vertexBuffers[0]->Unlock();
	}

	// set face
	int faceCount = 0;
	for (int i = 0; i < mMeshSource->GetMeshBufferCount(); ++i)
	{
		faceCount += mMeshSource->GetMeshBuffer(i)->GetRenderOp()->indexBuffer->GetCount() / 3;
	}

	mesh.setNumFaces(faceCount);
	mesh.setNumTVFaces(faceCount);

	int iStartVertex = 0;
	int iFace = 0;
	for (int i = 0; i < mMeshSource->GetMeshBufferCount(); ++i)
	{
		MeshBuffer * mb = mMeshSource->GetMeshBuffer(i);

		word * data = (word *)mb->GetRenderOp()->indexBuffer->Lock(eLockFlag::READ);
		for (int j = 0; j < mb->GetRenderOp()->indexBuffer->GetCount(); j += 3)
		{
			int i0 = iStartVertex + data[0];
			int i1 = iStartVertex + data[1];
			int i2 = iStartVertex + data[2];

			mesh.faces[iFace].setVerts(i0,i1,i2);
			mesh.faces[iFace].setSmGroup(1);
			mesh.faces[iFace].setEdgeVisFlags(1,1,0);
			mesh.tvFace[iFace].setTVerts(i0,i1,i2);

			data += 3;
			iFace += 1;
		}
		
		mb->GetRenderOp()->indexBuffer->Unlock();

		iStartVertex += mb->GetRenderOp()->vertexBuffers[0]->GetCount();
	}

	// set texture
	Rad::Material * mmtl = mMeshSource->GetMeshBuffer(0)->GetMaterial();
	if (mmtl->maps[eMapType::DIFFUSE] != NULL)
	{
		String filename = ResourceManager::Instance()->GetAbsoluteSourceName(mmtl->maps[eMapType::DIFFUSE]->GetSourceName());

		char diffMap[256];
		strcpy(diffMap, filename.c_str());

		StdMat2 *mtl = NewDefaultStdMat();

		BitmapTex *bmtex = (BitmapTex*)NewDefaultBitmapTex();  
		bmtex->SetName(diffMap);  
		bmtex->SetMapName(diffMap);  
		mtl->SetSubTexmap(ID_DI,bmtex);  
		mtl->SetSubTexmap( ID_OP, bmtex);  
		mtl->EnableMap( ID_OP, FALSE );  
		mtl->SetActiveTexmap( bmtex );  
		mtl->SetMtlFlag(MTL_TEX_DISPLAY_ENABLED);

		pNode->SetMtl(mtl);
	}

	return true;
}
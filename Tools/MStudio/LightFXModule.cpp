#include "LightFXModule.h"
#include "MUtil.h"
#include "MainFrame.h"
#include "Editor.h"
#include "LightFX_UVUnwrap.h"
#include "MMeshSerializer.h"
#include "MImage.h"

#define STAGE_LUV_CREATE 1
#define STAGE_DO_LIGHTING 2

LightFXModule::LightFXModule()
	: EditorModule("LightFX")
{
	mLayout = MGUI::Layout::Load("LightFX.layout", NULL);
	mLayout->SetVisible(false);

	mButton_Lighting = (MGUI::Button *)mLayout->GetChild("Lighting");
	mButton_Lighting->E_Click += new cListener1<LightFXModule, const MGUI::ClickEvent *>(this, &LightFXModule::OnLighting);

	mButton_Reset = (MGUI::Button *)mLayout->GetChild("Reset");
	mButton_Reset->E_Click += new cListener1<LightFXModule, const MGUI::ClickEvent *>(this, &LightFXModule::OnReset);

	mFXWorld = new FX_World;
	mUVAThread = NULL;
	mDoLighting = false;

	mPropertyGrid = new PropertyGrid(mLayout);
	mPropertyGrid->SetAlign(MGUI::eAlign::H_STRETCH);
	mPropertyGrid->Attach(mFXWorld->GetSetting());

	Editor::Instance()->E_Update += new cListener0<LightFXModule>(this, &LightFXModule::OnUpdate);
}

LightFXModule::~LightFXModule()
{
	safe_delete (mUVAThread);

	delete mPropertyGrid;

	delete mFXWorld;

	delete mLayout;
}

void LightFXModule::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);

	MGUI::Rect clRect = mLayout->GetClient();
	MGUI::Rect tlRect = mButton_Reset->GetRect();
	float y = tlRect.y + tlRect.h + 5;

	mPropertyGrid->SetRect(0, y, clRect.w, clRect.h - y);
}

void LightFXModule::Show()
{
	mLayout->SetVisible(true);

	Editor::Instance()->SetFlags(FLAG_PICK_ABLE | FLAG_DRAG_ABLE);
}

void LightFXModule::Hide()
{
	mLayout->SetVisible(false);
}

void LightFXModule::OnLighting(const MGUI::ClickEvent * e)
{
	if (EnsureLMapUVAtlas())
	{
		InitWorld();

		mDoLighting = true;
		ProgressDialog::Instance()->Show(true);
		Editor::Instance()->MaskUserInput(true);

		mFXWorld->Run();
	}
}

void LightFXModule::OnReset(const MGUI::ClickEvent * e)
{
	Array<Node *> nodeArray;
	World::Instance()->GetSectionNodes(nodeArray);

	for (int i = 0; i < nodeArray.Size(); ++i)
	{
		if (TYPE_OF(Mesh, nodeArray[i]))
		{
			Mesh * pMesh = (Mesh *)nodeArray[i];

			pMesh->ResetLighting();
		}
		else if (TYPE_OF(Light, nodeArray[i]))
		{
			Light * pLight = (Light *)nodeArray[i];
			if (pLight->IsStatic())
			{
				pLight->SetPreview(true);
			}
		}
		else if (TYPE_OF(Prefab, nodeArray[i]))
		{
			Prefab * pPrefab = (Prefab *)nodeArray[i];
			if (!pPrefab->IsInheritLighting())
			{
				pPrefab->ResetLighting();
			}
		}
	}

	Terrain::Instance()->ResetLighting();
}

void _appendPrefabNodes(Array<Node *> & arr, Prefab * prefab)
{
	if (prefab->IsInheritLighting())
		return ;

	for (int i = 0; i > prefab->GetNodeCount(); ++i)
	{
		arr.PushBack(prefab->GetNode(i));
	}
}

bool LightFXModule::EnsureLMapUVAtlas()
{
	int nUVAMesh = 0;

	Array<Node *> nodeArray;
	if (World::Instance()->GetCurrentSection())
	{
		nodeArray = World::Instance()->GetCurrentSection()->GetNodes();
	}

	for (int i = 0; i < nodeArray.Size(); ++i)
	{
		if (TYPE_OF(Prefab, nodeArray[i]))
		{
			_appendPrefabNodes(nodeArray, (Prefab *)nodeArray[i]);
		}
	}

	for (int i = 0; i < nodeArray.Size(); ++i)
	{
		if (TYPE_OF(Mesh, nodeArray[i]))
		{
			Mesh * pMesh = (Mesh *)nodeArray[i];

			if (pMesh->GetSLMode() == eStaticLightingMode::LIGHTING_MAP &&
				_checkMesh(pMesh) && !pMesh->HasLightingMapUV())
			{
				nUVAMesh += 1;
			}
		}
	}

	if (nUVAMesh > 0)
	{
		MessageBoxDialog::Instance()->E_EndDialog += new cListener1<LightFXModule, bool>(this, &LightFXModule::OnUVAtlas);
		MessageBoxDialog::Instance()->DoModal("lack lighting map uv, generate it?", MessageBoxType::Asking);
	}

	return nUVAMesh == 0;
}

void LightFXModule::OnUVAtlas(bool _ok)
{
	if (_ok)
	{
		Editor::Instance()->MaskUserInput(true);

		mUVAtlasGenMeshSet.Clear();
		mUVAMeshes.Clear();

		Array<Node *> nodeArray;
		if (World::Instance()->GetCurrentSection())
		{
			nodeArray = World::Instance()->GetCurrentSection()->GetNodes();
		}

		for (int i = 0; i < nodeArray.Size(); ++i)
		{
			if (TYPE_OF(Prefab, nodeArray[i]))
			{
				_appendPrefabNodes(nodeArray, (Prefab *)nodeArray[i]);
			}
		}

		int i = 0;
		while (i < nodeArray.Size())
		{
			if (TYPE_OF(Mesh, nodeArray[i]))
			{
				Mesh * pMesh = (Mesh *)nodeArray[i];

				if (pMesh->GetSLMode() == eStaticLightingMode::LIGHTING_MAP &&
					_checkMesh(pMesh) && !pMesh->HasLightingMapUV())
				{
					if (mUVAtlasGenMeshSet.Find(pMesh->GetSource().c_ptr()) == -1)
					{
						mUVAtlasGenMeshSet.Insert(pMesh->GetSource().c_ptr());

						FX_Mesh * fxmesh = UVA_CreateMesh(pMesh);
						d_assert (fxmesh != NULL);

						mUVAMeshes.PushBack(fxmesh);
					}
				}
			}

			++i;
		}

		safe_delete (mUVAThread);
		mUVAThread = new FX_UVUnwrapThread(mUVAMeshes);
		mUVAThread->E_Progress += new cListener1<LightFXModule, int>(this, &LightFXModule::UVA_UpdateProgress);

		Editor::Instance()->MaskUserInput(true);
		ProgressDialog::Instance()->Show(true);

		String text;
		text.Format("LUV Unwrap");
		ProgressDialog::Instance()->SetText(text);
	}
}

FX_Mesh * LightFXModule::UVA_CreateMesh(Mesh * pMesh)
{
	int vertexCount = pMesh->GetVertexCount();
	int triangleCount = pMesh->GetPrimCount();
	int materialCount = pMesh->GetSubMeshCount();

	FX_Mesh * fxMesh = new FX_Mesh;
	fxMesh->SetUserData(pMesh);
	fxMesh->SetLightingMapSize(pMesh->GetSLMapWidth(), pMesh->GetSLMapHeight());

	fxMesh->Alloc(vertexCount, triangleCount, materialCount);

	FX_Vertex * d_vtx = NULL;
	FX_Triangle * d_tri = NULL;
	FX_Material * d_mtl = NULL;
	fxMesh->Lock(&d_vtx, &d_tri, &d_mtl);
	{
		int startIndex = 0;
		for (int i = 0; i < pMesh->GetSubMeshCount(); ++i)
		{
			SubMesh * pSubMesh = pMesh->GetSubMesh(i);

			VertexBufferPtr pVertexBuffer = pSubMesh->GetRenderOp()->vertexBuffers[0];
			IndexBufferPtr pIndexBuffer = pSubMesh->GetRenderOp()->indexBuffer;

			int stride = pSubMesh->GetRenderOp()->vertexBuffers[0]->GetStride();
			int pos_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::POSITION);
			int nrm_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::NORMAL);
			int uv_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::TEXCOORD0);

			unsigned char * vdata = (unsigned char *)pVertexBuffer->Lock(eLockFlag::READ);
			unsigned short * idata = (unsigned short *)pIndexBuffer->Lock(eLockFlag::READ);

			for (int n = 0; n < pVertexBuffer->GetCount(); ++n)
			{
				FX_Vertex v;

				v.Position = *(Float3 *)(vdata + pos_off);
				v.Normal = *(Float3 *)(vdata + nrm_off);

				if (uv_off != -1)
				{
					v.UV = *(Float2 *)(vdata + uv_off);
				}
				else
				{
					v.UV = Float2(0, 0);
				}

				v.LUV = Float2(0, 0);

				v.MaterialId = i;

				*d_vtx++ = v;
				vdata += stride;
			}

			for (int n = 0; n < pIndexBuffer->GetCount() / 3; ++n)
			{
				FX_Triangle t;

				t.Index0 = startIndex + *idata++;
				t.Index1 = startIndex + *idata++;
				t.Index2 = startIndex + *idata++;
				t.MaterialId = i;

				*d_tri++ = t;
			}

			pVertexBuffer->Unlock();
			pIndexBuffer->Unlock();

			FX_Material m;
			m.opacity = pSubMesh->GetMaterial()->blendMode == eBlendMode::OPACITY;
			m.color = pSubMesh->GetMaterial()->diffuse;
			m.texture = pSubMesh->GetMaterial()->maps[eMapType::DIFFUSE];

			*d_mtl++ = m;

			startIndex += pVertexBuffer->GetCount();
		}
	}
	fxMesh->Unlock();

	return fxMesh;
}

int UVA_AppendVertex(Array<FX_Vertex> & varray, const FX_Vertex & v)
{
	for (int i = 0; i < varray.Size(); ++i)
	{
		if (varray[i].Position == v.Position &&
			varray[i].Normal == v.Normal && 
			varray[i].UV == v.UV &&
			varray[i].LUV == v.LUV)
		{
			return i;
		}
	}

	varray.PushBack(v);

	return varray.Size() - 1;
}

void LightFXModule::UVA_SaveMesh(FX_Mesh * fxmesh)
{
	String filename;
	filename = static_cast<Mesh *>(fxmesh->GetUserData())->GetFilename();
	filename = ResourceManager::Instance()->GetAbsoluteSourceName(filename);
	d_assert (filename != "");

	Array<FX_Vertex> varray;
	Array<FX_Triangle> tarray;

	FX_Vertex * fx_vtx;
	FX_Triangle * fx_tri;
	FX_Material * fx_mtl;
	
	MeshSourcePtr mesh = MeshManager::Instance()->NewMesh("UVA_Template");

	fxmesh->Lock(&fx_vtx, &fx_tri, &fx_mtl);
	for (int i = 0; i < fxmesh->NumOfMaterial(); ++i)
	{
		tarray.Clear();
		for (int j = 0; j < fxmesh->NumOfTriangles(); ++j)
		{
			FX_Triangle & tri = fx_tri[j];
			if (tri.MaterialId == i)
			{
				tarray.PushBack(tri);
			}
		}

		d_assert (tarray.Size() > 0);

		varray.Clear();
		for (int j = 0; j < tarray.Size(); ++j)
		{
			FX_Triangle & tri = tarray[j];

			tri.Index0 = UVA_AppendVertex(varray, fx_vtx[tri.Index0]);
			tri.Index1 = UVA_AppendVertex(varray, fx_vtx[tri.Index1]);
			tri.Index2 = UVA_AppendVertex(varray, fx_vtx[tri.Index2]);
		}

		d_assert (varray.Size() > 0);

		VertexDeclaration decl;
		decl.AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		decl.AddElement(eVertexSemantic::NORMAL, eVertexType::FLOAT3);
		decl.AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);
		decl.AddElement(eVertexSemantic::TEXCOORD1, eVertexType::FLOAT2);

		VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(decl.GetVertexSize(), varray.Size());
		float * vdata = (float *)vb->Lock(eLockFlag::WRITE);
		for (int j = 0; j < varray.Size(); ++j)
		{
			FX_Vertex & v = varray[j];

			*vdata++ = v.Position.x, *vdata++ = v.Position.y, *vdata++ = v.Position.z;
			*vdata++ = v.Normal.x, *vdata++ = v.Normal.y, *vdata++ = v.Normal.z;
			*vdata++ = v.UV.x, *vdata++ = v.UV.y;
			*vdata++ = v.LUV.x, *vdata++ = v.LUV.y;
		}
		vb->Unlock();

		IndexBufferPtr ib = HWBufferManager::Instance()->NewIndexBuffer(tarray.Size() * 3);
		uint16_t * idata = (uint16_t *)ib->Lock(eLockFlag::WRITE);
		for (int j = 0; j < tarray.Size(); ++j)
		{
			FX_Triangle & t = tarray[j];

			*idata++ = t.Index0;
			*idata++ = t.Index1;
			*idata++ = t.Index2;
		}
		ib->Unlock();

		MeshBuffer * mb = mesh->NewMeshBuffer();

		mb->GetRenderOp()->vertexDeclarations[0] = decl;
		mb->GetRenderOp()->vertexBuffers[0] = vb;
		mb->GetRenderOp()->indexBuffer = ib;
		mb->GetRenderOp()->primCount = ib->GetCount() / 3;
		mb->GetRenderOp()->primType = ePrimType::TRIANGLE_LIST;

		mb->GetMaterial()->diffuse = fx_mtl[i].color;
		mb->GetMaterial()->maps[eMapType::DIFFUSE] = fx_mtl[i].texture;
		mb->GetMaterial()->blendMode = fx_mtl[i].opacity ? eBlendMode::OPACITY : eBlendMode::ALPHA_TEST;
	}
	fxmesh->Unlock();

	mesh->GenAabbFromRenderMesh();

	MeshSerializer::Save(mesh.c_ptr(), filename);
}

void LightFXModule::UVA_UpdateProgress(int progress)
{
	ProgressDialog::Instance()->SetPercentage(progress);

	if (progress == 100)
	{
		for (int i = 0; i < mUVAMeshes.Size(); ++i)
		{
			UVA_SaveMesh(mUVAMeshes[i]);
			delete mUVAMeshes[i];
		}

		mUVAMeshes.Clear();

		ProgressDialog::Instance()->Show(false);
		Editor::Instance()->MaskUserInput(false);
	}
}

void LightFXModule::InitWorld()
{
	Array<Node *> nodeArray;
	if (World::Instance()->GetCurrentSection())
	{
		nodeArray = World::Instance()->GetCurrentSection()->GetNodes();
	}

	for (int i = 0; i < nodeArray.Size(); ++i)
	{
		Node * n = nodeArray[i];

		if (TYPE_OF(Mesh, n))
		{
			Mesh * pMesh = (Mesh *)n;

			if (pMesh->GetSLMode() != eStaticLightingMode::NONE ||
				pMesh->GetSLCastShadow() != false)
			{
				_createMesh(pMesh, false);
			}
			
		}
		else if (TYPE_OF(Light, n))
		{
			Light * pLight = (Light *)n;

			if (pLight->IsStatic())
			{
				_createLight(pLight);

				pLight->SetPreview(false);
			}
		}
		else if (TYPE_OF(Prefab, n))
		{
			_createPrefab((Prefab *)n);
		}
	}

	_createTerrain();

	_createGrassManager();

	Light * pMainLight = World::Instance()->MainLight();

	FX_Light * pLight = FX_World::Instance()->NewLight();
	pLight->Type = pMainLight->GetType();
	pLight->Position = pMainLight->GetPosition();
	pLight->Direction = pMainLight->GetWorldRotation().GetDirVector();
	pLight->Color = pMainLight->GetDiffuse();
	pLight->AttenStart = pMainLight->GetAttenStart();
	pLight->AttenEnd = pMainLight->GetAttenEnd();
	pLight->AttenFallOff = pMainLight->GetAttenFallOff();
	pLight->SpotInner = pMainLight->GetSpotInner();
	pLight->SpotOuter = pMainLight->GetSpotOuter();
	pLight->SpotFallOff = pMainLight->GetSpotFallOff();
}

bool LightFXModule::_checkMesh(Mesh * pMesh)
{
	if (pMesh == NULL)
		return false;

	if (pMesh->GetVertexCount() == 0 ||
		pMesh->GetPrimCount() == 0)
		return false;

	for (int i = 0; i < pMesh->GetSubMeshCount(); ++i)
	{
		SubMesh * pSubMesh = pMesh->GetSubMesh(i);

		if (pSubMesh->GetRenderOp()->primType != ePrimType::TRIANGLE_LIST)
			return false;

		int pos_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::POSITION);
		int nrm_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::NORMAL);
		int uv_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::TEXCOORD0);

		if (pos_off == -1 || nrm_off == -1)
			return false;

		if (pSubMesh->GetRenderOp()->vertexBuffers[0] == NULL ||
			pSubMesh->GetRenderOp()->vertexBuffers[0]->GetUsage() != eUsage::STATIC_MANAGED ||
			pSubMesh->GetRenderOp()->indexBuffer == NULL)
			return false;
	}

	return true;
}

void LightFXModule::_createPrefab(Prefab * pPrefab)
{
	for (int i = 0; i < pPrefab->GetNodeCount(); ++i)
	{
		Node * n = pPrefab->GetNode(i);

		if (TYPE_OF(Mesh, n))
		{
			Mesh * pMesh = (Mesh *)n;

			if (pMesh->GetSLMode() != eStaticLightingMode::NONE ||
				pMesh->GetSLCastShadow() != false)
			{
				_createMesh(pMesh, pPrefab->IsInheritLighting());
			}

		}
		else if (TYPE_OF(Light, n))
		{
			Light * pLight = (Light *)n;

			if (pLight->IsStatic())
			{
				_createLight(pLight);

				pLight->SetPreview(false);
			}
		}
		else if (TYPE_OF(Prefab, n))
		{
			_createPrefab((Prefab *)n);
		}
	}
}

void LightFXModule::_createMesh(Mesh * pMesh, bool onlyCast)
{
	if (!_checkMesh(pMesh))
		return ;

	Mat4 worldTm = pMesh->GetWorldTM();
	int vertexCount = pMesh->GetVertexCount();
	int triangleCount = pMesh->GetPrimCount();
	int materialCount = pMesh->GetSubMeshCount();

	FX_Mesh * fxMesh = FX_World::Instance()->NewMesh();
	if (!onlyCast)
	{
		fxMesh->SetLightingMode(pMesh->GetSLMode());
		fxMesh->SetCastShadow(pMesh->GetSLCastShadow());
		fxMesh->SetRecieveShadow(pMesh->GetSLReceiveShadow());
	}
	
	fxMesh->SetLightingMapSize(pMesh->GetSLMapWidth(), pMesh->GetSLMapHeight());
	fxMesh->SetUserData(pMesh);

	fxMesh->Alloc(vertexCount, triangleCount, materialCount);
	
	FX_Vertex * d_vtx = NULL;
	FX_Triangle * d_tri = NULL;
	FX_Material * d_mtl = NULL;
	fxMesh->Lock(&d_vtx, &d_tri, &d_mtl);
	{
		int startIndex = 0;
		for (int i = 0; i < pMesh->GetSubMeshCount(); ++i)
		{
			SubMesh * pSubMesh = pMesh->GetSubMesh(i);

			VertexBufferPtr pVertexBuffer = pSubMesh->GetRenderOp()->vertexBuffers[0];
			IndexBufferPtr pIndexBuffer = pSubMesh->GetRenderOp()->indexBuffer;

			int stride = pSubMesh->GetRenderOp()->vertexBuffers[0]->GetStride();
			int pos_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::POSITION);
			int nrm_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::NORMAL);
			int uv_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::TEXCOORD0);
			int luv_off = pSubMesh->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::TEXCOORD1);

			unsigned char * vdata = (unsigned char *)pVertexBuffer->Lock(eLockFlag::READ);
			unsigned short * idata = (unsigned short *)pIndexBuffer->Lock(eLockFlag::READ);

			for (int n = 0; n < pVertexBuffer->GetCount(); ++n)
			{
				FX_Vertex v;

				v.Position = *(Float3 *)(vdata + pos_off);
				v.Normal = *(Float3 *)(vdata + nrm_off);

				if (uv_off != -1)
				{
					v.UV = *(Float2 *)(vdata + uv_off);
				}
				else
				{
					v.UV = Float2(0, 0);
				}

				if (luv_off != -1)
				{
					v.LUV = *(Float2 *)(vdata + luv_off);
				}
				else
				{
					v.LUV = Float2(0, 0);
				}

				v.MaterialId = i;

				v.Position.TransformA(worldTm);
				v.Normal.TransformN(worldTm);
				v.Normal.Normalize();

				*d_vtx++ = v;
				vdata += stride;
			}

			for (int n = 0; n < pIndexBuffer->GetCount() / 3; ++n)
			{
				FX_Triangle t;

				t.Index0 = startIndex + *idata++;
				t.Index1 = startIndex + *idata++;
				t.Index2 = startIndex + *idata++;
				t.MaterialId = i;

				*d_tri++ = t;
			}

			pVertexBuffer->Unlock();
			pIndexBuffer->Unlock();

			FX_Material m;
			m.opacity = pSubMesh->GetMaterial()->blendMode == eBlendMode::OPACITY;
			m.color = pSubMesh->GetMaterial()->diffuse;
			m.texture = pSubMesh->GetMaterial()->maps[eMapType::DIFFUSE];

			*d_mtl++ = m;

			startIndex += pVertexBuffer->GetCount();
		}
	}
	fxMesh->Unlock();
}

void LightFXModule::_createLight(Light * pLight)
{
	if (!pLight->IsStatic())
		return ;

	FX_Light * fxLight = FX_World::Instance()->NewLight();
	fxLight->Type = pLight->GetType();
	fxLight->Position = pLight->GetPosition();
	fxLight->Direction = pLight->GetWorldRotation().GetDirVector();
	fxLight->Color = pLight->GetDiffuse();
	fxLight->AttenStart = pLight->GetAttenStart();
	fxLight->AttenEnd = pLight->GetAttenEnd();
	fxLight->AttenFallOff = pLight->GetAttenFallOff();
	fxLight->SpotInner = pLight->GetSpotInner();
	fxLight->SpotOuter = pLight->GetSpotOuter();
	fxLight->SpotFallOff = pLight->GetSpotFallOff();
}

void LightFXModule::_createTerrain()
{
	Terrain * pTerrain = Terrain::Instance();
	if (pTerrain->IsValid())
	{
		pTerrain->ResetLighting();
		FX_World::Instance()->NewTerrain(pTerrain);
	}
}

void LightFXModule::_createGrassManager()
{
	FX_GrassManager * pGrassManager = FX_World::Instance()->NewGrassManager();

	const World::Info * wi = World::Instance()->GetInfo();

	pGrassManager->Build(
		wi->SectionCount.x, wi->SectionCount.y,
		wi->SectionSize.x, wi->SectionSize.z);

	Array<GrassBlock *> blocks;
	GrassManager::Instance()->_getAllBlocks(blocks);

	for (int i = 0; i < blocks.Size(); ++i)
	{
		GrassBlock * block = blocks[i];
		for (int n = 0; n < block->InstanceArray.Size(); ++n)
		{
			GrassInstance * gi = &block->InstanceArray[n];
			const GrassTemplate * gt = block->GetSection()->GetTemplate(gi->Id);

			if (gt->CastAO)
			{
				FX_Grass gs;
				gs.mGI = gi;

				for (int k = 0; k < FX_Grass::K_SEGMENT; ++k)
				{
					float h = (float)k / (float)(FX_Grass::K_SEGMENT - 1) * gi->Size;

					gs.mVertex[k].Position = gi->Position + Float3(0, h, 0);
					gs.mVertex[k].Normal = gi->Normal;
				}

				pGrassManager->AddGrass(gs);
			}
		}
	}
}

String _getLightingMapName(int uid)
{
	if (uid < 9999)
	{
		return i2str(uid, "%04d");
	}
	else
	{
		return i2str(uid);
	}
}

void LightFXModule::ProcessResult()
{
	for (int i = 0; i < FX_World::Instance()->GetMeshCount(); ++i)
	{
		FX_Mesh * pFXMesh = FX_World::Instance()->GetMesh(i);

		Mesh * pMesh = (Mesh *)pFXMesh->GetUserData();
		pMesh->ResetLighting();

		if (pFXMesh->GetLightingMode() == eStaticLightingMode::LIGHTING_COLOR)
		{
			Array<Rgba32> colors;

			pFXMesh->GetLightingColor(colors);

			pMesh->SetLightingColor(&colors[0], colors.Size());
		}
		else if (pFXMesh->GetLightingMode() == eStaticLightingMode::LIGHTING_MAP)
		{
			Array<Rgba32> colors;
			int width = pFXMesh->GetLightingMapWidth();
			int height = pFXMesh->GetLightingMapHeight();

			pFXMesh->GetLightingMap(colors);

			int uid = pMesh->GetUID();
			String floder = Editor::Instance()->GetSceneFilename();
			floder = FileHelper::GetFileDir(floder);

			String filename = floder + "/LM" + _getLightingMapName(uid) + ".png";
			OSerializerF OS(filename.c_str());
			if (OS.GetFile() != NULL)
			{
				PNG_Save(OS, (const byte *)&colors[0], width, height, ePixelFormat::R8G8B8A8);
			}

			filename = ResourceManager::Instance()->GetRelativeSourceName(filename);
			pMesh->SetSLMapFile(filename);
		}
	}

	//
	FX_Terrain * pTerrain = FX_World::Instance()->GetTerrain();
	if (pTerrain != NULL)
	{
		Array<Rgba32> colors;
		pTerrain->GetLightingMap(colors);

		Terrain::Instance()->SetLightingMap(colors);
	}

	//
	FX_GrassManager * pGrassManager = FX_World::Instance()->GetGrassManager();
	if (pGrassManager)
	{
		pGrassManager->ProcessResult();
	}

	if (FX_World::Instance()->GetSetting()->AutoShutdown)
	{
		Editor::Instance()->SaveScene();

		Util_StartEXE("shutdown.exe -s");
	}
}

void LightFXModule::OnUpdate()
{
	if (mDoLighting)
	{
		int stage = mFXWorld->GetStage();
		int progress = mFXWorld->GetProgress();


		switch (stage)
		{
		case FX_World::STAGE_VERTEX_COLOR:
			ProgressDialog::Instance()->SetText("VertexColor");
			break;

		case FX_World::STAGE_DIRECT_LIGHTING_MESH:
			ProgressDialog::Instance()->SetText("DirectLighting - Mesh");
			break;

		case FX_World::STAGE_DIRECT_LIGHTING_TERRAIN:
			ProgressDialog::Instance()->SetText("DirectLighting - Terrain");
			break;

		case FX_World::STAGE_DIRECT_LIGHTING_GRASS:
			ProgressDialog::Instance()->SetText("DirectLighting - Grass");
			break;

		case FX_World::STAGE_INDIRECT_LIGHTING_MESH:
			ProgressDialog::Instance()->SetText("IndirectLighting - Mesh");
			break;

		case FX_World::STAGE_INDIRECT_LIGHTING_TERRAIN:
			ProgressDialog::Instance()->SetText("IndirectLighting - Terrain");
			break;

		case FX_World::STAGE_INDIRECT_LIGHTING_GRASS:
			ProgressDialog::Instance()->SetText("IndirectLighting - Grass");
			break;

		case FX_World::STAGE_AMBIENT_OCCLUSION_MESH:
			ProgressDialog::Instance()->SetText("AO - Mesh");
			break;

		case FX_World::STAGE_AMBIENT_OCCLUSION_TERRAIN:
			ProgressDialog::Instance()->SetText("AO - Terrain");
			break;

		case FX_World::STAGE_AMBIENT_OCCLUSION_GRASS:
			ProgressDialog::Instance()->SetText("AO - GRASS");
			break;

		case FX_World::STAGE_FINISH:
			ProgressDialog::Instance()->SetText("Finish");
			break;
		}

		ProgressDialog::Instance()->SetPercentage(progress);

		if (stage == FX_World::STAGE_END)
		{
			ProcessResult();

			mFXWorld->Clear();

			ProgressDialog::Instance()->Show(false);
			Editor::Instance()->MaskUserInput(false);

			mDoLighting = false;
		}
	}
}

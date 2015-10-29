#include "EntityBound.h"
#include "Editor.h"

EntityBound::EntityBound()
	: mRenderColMesh(NULL)
{
	mRenderAABB = new RenderOp();

	int iVertexCount = 8;
	int iIndexCount = 12 * 2;
	int iPrimCount = 12;

	mRenderAABB->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

	VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(iVertexCount, sizeof(float) * 3);
	float * vert = (float *)vb->Lock(eLockFlag::WRITE);
	{
		const float half_w = 0.5f;
		const float half_h = 0.5f;
		const float half_d = 0.5f;
		Float3 pos;

		//front
		pos = Float3(-half_w, half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w, half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(-half_w, -half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w, -half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		//back
		pos = Float3(-half_w, half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w, half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(-half_w, -half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w, -half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;
	}
	vb->Unlock();
	mRenderAABB->vertexBuffers[0] = vb;

	IndexBufferPtr ibuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount * sizeof(short));
	short * indices = (short *)ibuffer->Lock(eLockFlag::WRITE);
	{
		*indices++ = 0, *indices++ = 1;
		*indices++ = 2, *indices++ = 3;
		*indices++ = 0, *indices++ = 2;
		*indices++ = 1, *indices++ = 3;

		*indices++ = 4, *indices++ = 5;
		*indices++ = 6, *indices++ = 7;
		*indices++ = 4, *indices++ = 6;
		*indices++ = 5, *indices++ = 7;

		*indices++ = 0, *indices++ = 4;
		*indices++ = 1, *indices++ = 5;
		*indices++ = 2, *indices++ = 6;
		*indices++ = 3, *indices++ = 7;
	}
	ibuffer->Unlock();
	mRenderAABB->indexBuffer = ibuffer;

	mRenderAABB->primCount = iPrimCount;
	mRenderAABB->primType = ePrimType::LINE_LIST;

	Editor::Instance()->E_NodeSelect += new cListener0<EntityBound>(this, &EntityBound::OnShapeSelectedChanged);
	World::Instance()->E_RenderSolid += new cListener0<EntityBound>(this, &EntityBound::OnRender);
}

EntityBound::~EntityBound()
{
	safe_delete (mRenderAABB);
	safe_delete (mRenderColMesh);
}

void EntityBound::OnRender()
{
	if (Editor::Instance()->GetSelectNode() == NULL)
		return ;

	if (mRenderColMesh)
		_doRenderColMesh();
	else
		_doRenderAABB();
}

void EntityBound::_doRenderAABB()
{
	Node * node = Editor::Instance()->GetSelectNode();

	Aabb box = node->GetWorldAabb();

	Float3 pos = box.GetCenter();
	Float3 scale = box.GetSize();

	Mat4 worldTm;
	worldTm.MakeTransform(pos, Quat::Identity, scale);

	RenderSystem::Instance()->SetRenderState(eFillMode::SOLID, eCullMode::NONE);
	RenderHelper::Instance()->DebugDraw(mRenderAABB, Float4(1, 1, 0), worldTm);
}

void EntityBound::_doRenderColMesh()
{
	Node * node = Editor::Instance()->GetSelectNode();

	RenderSystem * render = RenderSystem::Instance();

	Mat4 worldTm, scaleTm;
	worldTm = node->GetWorldTM();
	scaleTm.MakeScale(1.001f, 1.001f, 1.001f);

	RenderSystem::Instance()->SetRenderState(eFillMode::FRAME, eCullMode::NONE);
	RenderHelper::Instance()->DebugDraw(mRenderColMesh, Float4(1, 1, 0), scaleTm * worldTm);
}

void EntityBound::OnShapeSelectedChanged()
{
	safe_delete (mRenderColMesh);

	Node * pNode = Editor::Instance()->GetSelectNode();
	if (!KIND_OF(Mesh, pNode))
		return ;

	Mesh * pMesh = (Mesh *)pNode;
	if (pMesh->GetSource() == NULL)
		return ;

	ColMesh * pColMesh = pMesh->GetSource()->GetColMesh();
	if (pColMesh->GetNumOfIndices() == 0 || pColMesh->GetNumOfIndices() > 65535)
		return ;

	int iVertexCount = pColMesh->GetNumOfVertices();
	int iIndexCount = pColMesh->GetNumOfIndices();

	Float3 * pColVert = pColMesh->GetVertices();
	int * pColIndex = pColMesh->GetIndices();

	mRenderColMesh = new RenderOp();

	mRenderColMesh->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

	mRenderColMesh->vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 3, iVertexCount);
	Float3 * vdata = (Float3 *)mRenderColMesh->vertexBuffers[0]->Lock(eLockFlag::WRITE);
	for (int i = 0; i < iVertexCount; ++i)
	{
		vdata[i] = pColVert[i];
	}
	mRenderColMesh->vertexBuffers[0]->Unlock();

	mRenderColMesh->indexBuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);

	unsigned short * idata = (unsigned short *)mRenderColMesh->indexBuffer->Lock(eLockFlag::WRITE);
	for (int i = 0; i < pColMesh->GetNumOfIndices() / 3; ++i)
	{
		unsigned short a = (unsigned short)pColIndex[i * 3 + 0];
		unsigned short b = (unsigned short)pColIndex[i * 3 + 1];
		unsigned short c = (unsigned short)pColIndex[i * 3 + 2];

		*idata++ = a; *idata++ = b; *idata++ = c;
	}
	mRenderColMesh->indexBuffer->Unlock();

	mRenderColMesh->primCount = iIndexCount / 2;
	mRenderColMesh->primType = ePrimType::LINE_LIST;
}


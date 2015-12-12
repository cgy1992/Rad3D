#include "RenderViewer.h"

#define GRID_SIZE 16

ImplementSingleton(RenderViewer);

RenderViewer::RenderViewer()
{
	mLayout = new MGUI::Layout(NULL, NULL);
	mTextBox = new MGUI::TextBox(NULL, mLayout);
	mTextBox->SetAlign(MGUI::eAlign::LEFT | MGUI::eAlign::BOTTOM);

	// create grid mesh
	mGridMesh = new Mesh();
	SubMesh * pMeshBuffer = mGridMesh->NewSubMesh();

	pMeshBuffer->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT2);

	pMeshBuffer->GetRenderOp()->vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(2 * sizeof(float), (GRID_SIZE + 1) * 2 * 2);

	int index = 0;
	float * data = (float *)pMeshBuffer->GetRenderOp()->vertexBuffers[0]->Lock(eLockFlag::WRITE);
	{
		float _s = -GRID_SIZE / 2 * METER_LEN;
		float _e = GRID_SIZE / 2 * METER_LEN;

		float y = _s;
		float dy = METER_LEN;
		for (int i = 0; i < GRID_SIZE + 1; ++i)
		{
			data[index++] = _s; data[index++] = y;
			data[index++] = _e; data[index++] = y;

			y += dy;
		}

		float x = _s;
		float dx = METER_LEN;
		for (int i = 0; i < GRID_SIZE + 1; ++i)
		{
			data[index++] = x; data[index++] = _s;
			data[index++] = x; data[index++] = _e;

			x += dx;
		}
	}
	pMeshBuffer->GetRenderOp()->vertexBuffers[0]->Unlock();

	pMeshBuffer->GetRenderOp()->primCount = (GRID_SIZE + 1) * 2;
	pMeshBuffer->GetRenderOp()->primType = ePrimType::LINE_LIST;

	ShaderFX * pShaderFX = ShaderFXManager::Instance()->Load("Grid", "Grid.mfx");
	d_assert(pShaderFX != NULL);
	pMeshBuffer->SetShaderFX(pShaderFX);

	mGridMesh->SetLocalAabb(Aabb::Infinite);

	Layout();

	World::Instance()->E_RenderEnd += new cListener0<RenderViewer>(this, &RenderViewer::OnUpdate);
}

RenderViewer::~RenderViewer()
{
	delete mGridMesh;

	delete mLayout;
}

void RenderViewer::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();
	float x = 256;
	float y = 0;
	float w = rect.w - x;
	float h = rect.h - y;

	mLayout->SetRect(x, y, w, h);

	Viewport vp;
	vp.x = (int)x;
	vp.y = (int)y;
	vp.w = (int)w;
	vp.h = (int)h;
	World::Instance()->MainRenderContext()->SetViewport(vp);
}

bool RenderViewer::IsMouseFocused()
{
	return (MGUI::InputManager::Instance()->GetMouseFocusedWidget() == mLayout);
}

void RenderViewer::OnUpdate()
{
	int batchCount = RenderSystem::Instance()->_getBatchCount();
	int primCount = RenderSystem::Instance()->_getPrimitiveCount();

	batchCount -= 1;
	primCount -= mGridMesh->GetPrimCount();

	String text;
	text.Format("BC: %d, TC: %d", batchCount, primCount);

	mTextBox->SetCaption(text.c_wstr());
}
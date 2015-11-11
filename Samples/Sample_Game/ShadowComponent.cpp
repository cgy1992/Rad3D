#include "ShadowComponent.h"
#include "Actor.h"

const int ShadowComponent::GRID_COUNT = 4;

struct s_vertex {
	Float3 position;
	Float3 uvn;
};

ShadowComponent::ShadowComponent()
	: mVisible(false)
{
	int iVertexCount = (GRID_COUNT + 1) * (GRID_COUNT + 1);
	int iIndexCount = (GRID_COUNT * GRID_COUNT) * 6;

	mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
	mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT3);

	mRenderOp.vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(s_vertex), iVertexCount);
	mRenderOp.indexBuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);

	short * idx = (short *)mRenderOp.indexBuffer->Lock(eLockFlag::WRITE);
	for (int j = 0; j < GRID_COUNT; ++j)
	{
		int c = j * (GRID_COUNT + 1);
		int n = (j + 1) * (GRID_COUNT + 1);

		for (int i = 0; i < GRID_COUNT; ++i)
		{
			int a = c + i;
			int b = a + 1;
			int c = n + i;
			int d = c + 1;

			d_assert (
				a < iVertexCount &&
				b < iVertexCount &&
				c < iVertexCount &&
				d < iVertexCount);

			*idx++ = a;
			*idx++ = b;
			*idx++ = c;

			*idx++ = c;
			*idx++ = b;
			*idx++ = d;
		}
	}
	mRenderOp.indexBuffer->Unlock();

	mRenderOp.primCount = GRID_COUNT * GRID_COUNT * 2;
	mRenderOp.primType = ePrimType::TRIANGLE_LIST;

	mShaderFX = ShaderFXManager::Instance()->Load("ShadowComponent", "ShadowComponent.mfx");
	d_assert (mShaderFX != NULL);

	World::Instance()->E_RenderSolidEnd += listenerRender(this, &ShadowComponent::OnRender);
}

ShadowComponent::~ShadowComponent()
{
	World::Instance()->E_RenderSolidEnd -= listenerRender(this, &ShadowComponent::OnRender);
}

int ShadowComponent::Update(float time)
{
	mVisible = false;

	Terrain * pTerrain = Terrain::Instance();
	if (!pTerrain->IsValid())
		return 0;

	Actor * pActor = GetActor();
	Camera * pCamera = World::Instance()->MainCamera();
	if (pCamera->GetVisibility(pActor->_getNode()->GetWorldAabb()) == Camera::VB_NONE)
		return 0;

	const Float3 & center = pActor->_getNode()->GetPosition();
	const Aabb & bound = pActor->_getNode()->GetBound();
	int h_grid = GRID_COUNT / 2;

	float size = Max(bound.GetSize().x, bound.GetSize().z) * 0.55f;
	if (size <= 1)
		return 0;

	mVisible = true;
	Float3 currentPos = pActor->GetPosition();
	Float3 targetPos = pActor->GetTargetPosition();
	if (targetPos.DistanceSq(currentPos) < 1)
		return 0;

	float inv_grid = 1.0f / GRID_COUNT;
	float step = size / GRID_COUNT;
	s_vertex * vtx = (s_vertex *)mRenderOp.vertexBuffers[0]->Lock(eLockFlag::WRITE);
	for (int j = 0; j <= GRID_COUNT; ++j)
	{
		float z = center.z + (j - h_grid)* step;
		for (int i = 0; i <= GRID_COUNT; ++i)
		{
			float x = center.x + (i - h_grid) * step;
			float y;
			Float3 n;

			pTerrain->GetHeightAt(y, x, z);
			pTerrain->GetNormalAt(n, x, z);

			vtx->position = Float3(x, y, z) + n * UNIT_METRES * 0.05f;
			vtx->uvn = Float3(i * inv_grid, j * inv_grid, n.y);
			++vtx;
		}
	}
	mRenderOp.vertexBuffers[0]->Unlock();

	mVisible = true;

	return 0;
}

void ShadowComponent::OnRender()
{
	if (mVisible)
	{
		mShaderFX->GetPass(0)->SetConst("u_Opacity", Float4(1, 0, 0));

		RenderSystem::Instance()->SetWorldTM(Mat4::Identity);
		RenderSystem::Instance()->Render(mShaderFX, &mRenderOp);
	}
}


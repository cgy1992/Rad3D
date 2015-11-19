#include "MGrassRenderer.h"
#include "MGrassManager.h"
#include "MGrassSection.h"
#include "MWorld.h"
#include "MFunctional.h"

namespace Rad {

	ImplementRTTI(GrassRenderer, Node);

	GrassRenderer::GrassRenderer(GrassSection * section)
		: Node("GrassRenderer")
		, mSection(section)
	{
		mNode = this;

		int iVertexCount = K_GRASS_RENDER_POOL * K_VERTEX_COUNT_PER_GRASS;
		int iIndexCount = K_GRASS_RENDER_POOL * K_PRIM_COUNT_PER_GRASS * 3;

		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::COLOR, eVertexType::FLOAT3);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(sizeof(GrassVertex), iVertexCount, eUsage::DYNAMIC_MANAGED);
		mRenderOp.vertexBuffers[0] = vb;

		mRenderOp.indexBuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);

		unsigned short * idx = (unsigned short *)mRenderOp.indexBuffer->Lock(eLockFlag::WRITE);
		for (int i = 0; i < K_GRASS_RENDER_POOL; ++i)
		{
			int index = i * K_VERTEX_COUNT_PER_GRASS;

			// Side 1
			*idx++ = index + 0;
			*idx++ = index + 1;
			*idx++ = index + 2;

			*idx++ = index + 2;
			*idx++ = index + 1;
			*idx++ = index + 3;

			index += 4;

			// Side 2
			*idx++ = index + 0;
			*idx++ = index + 1;
			*idx++ = index + 2;

			*idx++ = index + 2;
			*idx++ = index + 1;
			*idx++ = index + 3;
		}
		mRenderOp.indexBuffer->Unlock();

		mRenderOp.primCount = 0;
		mRenderOp.primType = ePrimType::TRIANGLE_LIST;

		mMaterial.depthMode = eDepthMode::N_LESS_EQUAL;
		mMaterial.blendMode = eBlendMode::ALPHA_BLEND;

		mLighting = false;

		mLocalAABB = Aabb::Infinite;

		SetFlags(0);
	}

	GrassRenderer::~GrassRenderer()
	{
	}

	struct GrassSortOp {

		Float3 eye;

		GrassSortOp(Float3 _eye) : eye(_eye) {}

		int operator ()(const GrassInstance * left, const GrassInstance * right) const
		{
			float d1 = left->Position.DistanceSq(eye);
			float d2 = right->Position.DistanceSq(eye);

			return d1 < d2 ? -1 : (d1 > d2 ? 1 : 0);
		}
	};

	void GrassRenderer::Update(float elapsedTime)
	{
		Node::Update(elapsedTime);

		mArray.Clear();

		Float3 camPos = World::Instance()->MainCamera()->GetWorldPosition();
		Float3 camDir = World::Instance()->MainCamera()->GetWorldRotation().GetDirVector();
		float radius = GrassManager::Instance()->GetVisibleRadius();
		Float2 windDir = GrassManager::Instance()->GetWaveDir();
		float windSpeed = GrassManager::Instance()->GetWaveSpeed();
		float windStrength = GrassManager::Instance()->GetWaveStrength();

		windSpeed *= Root::Instance()->GetTime();
		windDir *= windStrength;

		GrassManager::Instance()->_updateWave(elapsedTime);

		mSection->_getInstanceEx(mArray, camPos, camDir, radius);
		if (mArray.Size() > 0)
		{
			GrassSortOp sortOp(camPos);
			Sort(&mArray[0], mArray.Size(), sortOp);

			const int WAVE_VERTEX_INDEX[4] = { 2, 3, 6, 7 };

			int count = mArray.Size() < K_GRASS_RENDER_POOL ? mArray.Size() : K_GRASS_RENDER_POOL;
			GrassVertex * v = (GrassVertex *)mRenderOp.vertexBuffers[0]->Lock(eLockFlag::WRITE);
			for (int j = count - 1; j >= 0; --j)
			{
				const GrassInstance * gi = mArray[j];
				const GrassTemplate * gt = mSection->GetTemplate(gi->Id);

				gt->GetVertex(v, gi->Position, gi->LightingColor, gi->Rotation, gi->Size);

				int pi = gi->Rotation * K_VERTEX_COUNT_PER_GRASS;
				float wave = GrassManager::Instance()->_sampleWave(gi->Position.x, gi->Position.z);

				float theta = windSpeed + gi->Position.x * 0.4f + gi->Position.z * 0.6f;

				int c0 = (int)Math::Abs(theta * TRI_TABLE_RADIAN_MUL);
				int c1 = c0 * 2;
				int c2 = c0 * 4;
				int c3 = c0 * 16;

				c0 &= TRI_TABLE_MASK;
				c1 &= TRI_TABLE_MASK;
				c2 &= TRI_TABLE_MASK;
				c3 &= TRI_TABLE_MASK;

				float w = 0;
				w += Math::COS_TABLE[c0] * 0.50f;
				w += Math::COS_TABLE[c1] * 0.25f;
				w += Math::COS_TABLE[c2] * 0.25f;
				w += Math::COS_TABLE[c3] * wave;
				w *= gi->Size;

				v[WAVE_VERTEX_INDEX[0]].position.x += w * windDir.x;
				v[WAVE_VERTEX_INDEX[0]].position.z += w * windDir.y;

				v[WAVE_VERTEX_INDEX[1]].position.x += w * windDir.x;
				v[WAVE_VERTEX_INDEX[1]].position.z += w * windDir.y;

				v[WAVE_VERTEX_INDEX[2]].position.x += w * windDir.x;
				v[WAVE_VERTEX_INDEX[2]].position.z += w * windDir.y;

				v[WAVE_VERTEX_INDEX[3]].position.x += w * windDir.x;
				v[WAVE_VERTEX_INDEX[3]].position.z += w * windDir.y;

				v += K_VERTEX_COUNT_PER_GRASS;
			}
			mRenderOp.vertexBuffers[0]->Unlock();

			mMaterial.maps[eMapType::DIFFUSE] = mSection->_getTexture();
		}

		mRenderOp.primCount = mArray.Size() * K_PRIM_COUNT_PER_GRASS;

		mShaderFX = GrassManager::Instance()->GetShaderFX();
	}

	void GrassRenderer::AddRenderQueue(RenderQueue * rq)
	{
		rq->AddRenderObject(this);
	}

	void GrassRenderer::_getWorldPosition(Float3 & pos)
	{
		if (World::Instance()->GetCurrentSection() == mSection->GetWorldSection())
		{
			pos = World::Instance()->GetCurrentRenderContext()->GetCamera()->GetWorldPosition();
		}
		else
		{
			pos = mSection->GetWorldSection()->GetBound().GetCenter();
		}
	}
	
}
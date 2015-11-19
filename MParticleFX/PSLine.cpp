#include "PSLine.h"
#include "PSLineSet.h"

namespace Rad {

	PS_Line::PS_Line(PS_LineSet * parent)
		: mParent(parent)
	{
		mSegment = parent->GetSegment();
	}

	PS_Line::~PS_Line()
	{
	}

	void PS_Line::_update(Particle * p)
	{
		mWidth = p->Size.x;
		mLength = p->Size.y;

		const Float3 & waveWeight = mParent->GetWaveWeight();
		const Float3 & waveFreq = mParent->GetWaveFreq();
		Float3 waveDir = mParent->GetWaveDir();
		float waveStep = Math::DegreeToRadian(mParent->GetWaveStep());
		float time = Root::Instance()->GetTime();

		Float3 a = Float3(0, 0, 0); 
		Float3 b = Float3(1, 0, 0) * mLength;

		mPoints.Clear();

		if (!mParent->IsUsePoint())
		{
			if (!mParent->IsLocalSpace())
			{
				a.TransformQ(mParent->GetParent()->GetWorldRotation());
				b.TransformQ(mParent->GetParent()->GetWorldRotation());

				waveDir.TransformQ(mParent->GetParent()->GetWorldRotation());
			}

			a += p->Position;
			b += p->Position;
		}
		else
		{
			a = mParent->GetPoint1();
			b = mParent->GetPoint2();
		}

		Float3 dir = b - a;
		mLength = dir.Normalize();

		if (mParent->IsKeepAspect())
			mWidth = mLength * mParent->_getTexture()->_getAscept();

		mInvLength = 0;
		if (mLength > 0)
			mInvLength = 1 / mLength;

		mSegmentStep = mLength / mSegment;

		float step = 0;
		for (int i = 0; i < mSegment + 1; ++i)
		{
			float theta = waveStep * step * mInvLength;
			Float3 pos = a + dir * step;

			float w = 0;
			w += Math::FastSin2(theta + time * waveFreq.x) * waveWeight.x;
			w += Math::FastSin2(theta + time * waveFreq.y) * waveWeight.y;
			w += Math::FastSin2(theta + time * waveFreq.z) * waveWeight.z;

			pos += w * waveDir;

			mPoints.PushBack(pos);

			step += mSegmentStep;
		}

		mBound.minimum = Float3::Minimum(a, b);
		mBound.maximum = Float3::Maximum(a, b);

		mBound.Merge(a - p->Size * 0.5f);
		mBound.Merge(a + p->Size * 0.5f);
		mBound.Merge(b - p->Size * 0.5f);
		mBound.Merge(b + p->Size * 0.5f);
	}

	void PS_Line::_updateBuffer(Particle * p, void ** vertexBuffer, void ** indexBuffer, int & startVertex, int & primCount)
	{
		Camera * pCamera = World::Instance()->GetCurrentRenderContext()->GetCamera();
		Float3 camPos = pCamera->GetWorldPosition();

		float t = 0, t1 = 0;
		float half_w = mWidth * 0.5f;
		Float3 xAxis, yAxis, zAxis;

		float u0 = p->UVRect.x1, v0 = p->UVRect.y1;
		float u1 = p->UVRect.x2, v1 = p->UVRect.y2;
		float vLength = v1 - v0;

		Float3 lineDir = mPoints.Back() - mPoints.Front();
		lineDir.Normalize();

		Mat4 worldTM;
		worldTM.MakeTransform(mParent->GetParent()->GetWorldPosition(), mParent->GetParent()->GetWorldRotation(), Float3(1, 1, 1));

		bool first = true;
		int segment = 0;
		PS_Vertex * vb = (PS_Vertex  *)(*vertexBuffer);
		for (int i = 0; i < mPoints.Size() - 1; ++i)
		{
			Float3 a = mPoints[i];
			Float3 b = mPoints[i + 1];

			if (mParent->GetTileScale() <= 0)
			{
				t1 += mSegmentStep * mInvLength;
				if (t1 > 1.0f)
					t1 = 1.0f;
			}
			else
			{
				t1 += mSegmentStep * mParent->GetTileScale();
			}
			
			if (mParent->IsLocalSpace())
			{
				a.TransformA(worldTM);
				b.TransformA(worldTM);
			}

			Float3 camDir = a - camPos;
			camDir.Normalize();

			zAxis = lineDir;
			xAxis = Float3::Cross(zAxis, camDir);

			if (!first)
			{
				vb[2].position = b - xAxis * half_w;
				vb[3].position = b + xAxis * half_w;

				vb[2].uv = Float2(u0, v0 + t1 * vLength);
				vb[3].uv = Float2(u1, v0 + t1 * vLength);

				vb[2].color = p->Color;
				vb[3].color = p->Color;
			}
			else
			{
				vb[0].position = a - xAxis * half_w;
				vb[1].position = a + xAxis * half_w;
				vb[2].position = b - xAxis * half_w;
				vb[3].position = b + xAxis * half_w;

				vb[0].uv = Float2(u0, v0 + t * vLength);
				vb[1].uv = Float2(u1, v0 + t * vLength);
				vb[2].uv = Float2(u0, v0 + t1 * vLength);
				vb[3].uv = Float2(u1, v0 + t1 * vLength);

				vb[0].color = p->Color;
				vb[1].color = p->Color;
				vb[2].color = p->Color;
				vb[3].color = p->Color;
			}

			t = t1;
			vb += 2;
			segment += 1;
			first = false;
		}

		if (segment > 0)
		{
			unsigned short * ib = (unsigned short *)(*indexBuffer);
			for (int i = 0; i < segment; ++i)
			{
				int index = startVertex + i * 2;

				*ib++ = index + 0;
				*ib++ = index + 1;
				*ib++ = index + 2;

				*ib++ = index + 2;
				*ib++ = index + 1;
				*ib++ = index + 3;
			}

			*vertexBuffer = vb + 2;
			*indexBuffer = ib;
			startVertex += (segment + 1) * 2;
			primCount += segment * 2;
		}
	}

	//
	PS_Line_RenderObject::PS_Line_RenderObject()
		: mParent(NULL)
	{
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::COLOR, eVertexType::FLOAT4);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		mMaterial.cullMode = eCullMode::NONE;
		mMaterial.depthMode = eDepthMode::N_LESS_EQUAL;
		mMaterial.blendMode = eBlendMode::ADD;
	}

	PS_Line_RenderObject::~PS_Line_RenderObject()
	{
	}

	void PS_Line_RenderObject::Init(PS_LineSet * parent)
	{
		mParent = parent;
		mNode = parent->GetParent();
	}

	void PS_Line_RenderObject::_getWorldPosition(Float3 & pos)
	{
		if (mParent->IsLocalSpace())
			pos = mParent->GetParent()->GetWorldPosition();
		else
			pos = Float3::Zero;
	}

	void PS_Line_RenderObject::_getWorldBound(Aabb & bound)
	{
		bound = mParent->GetBound();
	}

	void PS_Line_RenderObject::_getWorldTM(Mat4 & form)
	{
		form = Mat4::Identity;
	}

	void PS_Line_RenderObject::_allocQuota(int quota)
	{
		d_assert (quota > 0);

		int segemnt = mParent->GetSegment();
		int vertexCount = (segemnt + 1) * 2 * quota;
		int indexCount = segemnt * 6 * quota;

		VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(sizeof(PS_Vertex), vertexCount, eUsage::DYNAMIC_MANAGED);
		IndexBufferPtr ib = HWBufferManager::Instance()->NewIndexBuffer(indexCount, eUsage::DYNAMIC_MANAGED);

		mRenderOp.vertexBuffers[0] = vb;
		mRenderOp.indexBuffer = ib;
		mRenderOp.primCount = 0;
		mRenderOp.primType = ePrimType::TRIANGLE_LIST;
	}

	void PS_Line_RenderObject::_update()
	{
		int blendMode = mParent->GetBlendMode();

		mMaterial.depthMode = eDepthMode::N_LESS_EQUAL;

		if (blendMode == PS_BlendMode::ADD)
		{
			mMaterial.blendMode = eBlendMode::ADD;
		}
		else if (blendMode == PS_BlendMode::ALPHA_BLEND)
		{
			mMaterial.blendMode = eBlendMode::ALPHA_BLEND;
		}
		else if (blendMode == PS_BlendMode::COLOR_BLEND)
		{
			mMaterial.blendMode = eBlendMode::COLOR_BLEND;
		}
		else
		{
			mMaterial.blendMode = eBlendMode::OPACITY;
			mMaterial.depthMode = eDepthMode::LESS_EQUAL;
		}

		mShaderFX = mParent->_getShaderFX();

		mMaterial.fillMode = eFillMode::SOLID;
		mMaterial.maps[eMapType::DIFFUSE] = mParent->_getTexture();

		SetRenderCallBack(eRenderCallBack::SHADER, mParent->GetShader().c_ptr());
	}

	void * PS_Line_RenderObject::_lockVertex()
	{
		return mRenderOp.vertexBuffers[0]->Lock(eLockFlag::WRITE);
	}

	void * PS_Line_RenderObject::_lockIndex()
	{
		return mRenderOp.indexBuffer->Lock(eLockFlag::WRITE);
	}

	void PS_Line_RenderObject::_unlock(int primCount)
	{
		mRenderOp.vertexBuffers[0]->Unlock();
		mRenderOp.indexBuffer->Unlock();

		mRenderOp.primCount = primCount;
	}

}
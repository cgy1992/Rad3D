#include "PSRibbon.h"
#include "PSRibbonSet.h"

namespace Rad {

	PS_Ribbon::PS_Ribbon(PS_RibbonSet * parent)
		: mParent(parent)
	{
		mSegment = parent->GetSegment();
		mSegmentLife = parent->GetSegmentLife();
	}

	PS_Ribbon::~PS_Ribbon()
	{
	}

	void PS_Ribbon::_update(Particle * p, float frameTime)
	{
		mWidth = p->Size.x;
		mLength = p->Size.y;

		if (mParent->IsKeepAspect())
			mWidth = mLength * mParent->_getTexture()->_getAscept();

		mInvLength = 0;
		if (mLength > 0)
			mInvLength = 1 / mLength;

		mSegmentStep = mLength / mSegment;

		Float3 Position = p->Position;

		if (mParent->IsLocalSpace() && mParent->IsLoose())
		{
			Mat4 worldTM;
			worldTM.MakeTransform(mParent->GetParent()->GetWorldPosition(), mParent->GetParent()->GetWorldRotation(), Float3(1, 1, 1));
			Position.TransformA(worldTM);
		}

		if (mSegmentLife > 0)
		{
			for (int i = mElems.Size() - 1; i >= 0; --i)
			{
				mElems[i].life -= frameTime;
				if (mElems[i].life <= 0)
				{
					mElems.Erase(i);
				}
			}
		}
		
		Elem newElem = { mSegmentLife, Position };
		if (mElems.Size() > 1)
		{
			mElems[0] = newElem;

			if (Position.DistanceSq(mElems[1].position) > mSegmentStep * mSegmentStep)
			{
				mElems.Insert(1, newElem);
			}
		}
		else
		{
			mElems.Insert(0, newElem);
		}

		while (mElems.Size() > mSegment + 1)
		{
			mElems.PopBack();
		}
	}

	void PS_Ribbon::_updateBuffer(Particle * p, void ** vertexBuffer, void ** indexBuffer, int & startVertex, int & primCount)
	{
		Camera * pCamera = World::Instance()->GetCurrentRenderContext()->GetCamera();
		Float3 camPos = pCamera->GetWorldPosition();

		float t = 0, t1 = 0;
		float half_w = mWidth * 0.5f;
		RectF uvRect = p->UVRect;
		Float3 xAxis, yAxis, zAxis;

		float u0 = p->UVRect.x1, v0 = p->UVRect.y1;
		float u1 = p->UVRect.x2, v1 = p->UVRect.y2;
		float vLength = v1 - v0;

		Mat4 worldTM;
		worldTM.MakeTransform(mParent->GetParent()->GetWorldPosition(), mParent->GetParent()->GetWorldRotation(), Float3(1, 1, 1));

		bool first = true;
		int segment = 0;
		PS_Vertex * vb = (PS_Vertex  *)(*vertexBuffer);
		for (int i = 0; i < mElems.Size() - 1; ++i)
		{
			Float3 a = mElems[i].position;
			Float3 b = mElems[i + 1].position;

			if (mParent->IsLocalSpace() && !mParent->IsLoose())
			{
				a.TransformA(worldTM);
				b.TransformA(worldTM);
			}

			Float3 dir = a - b;
			float len = dir.Normalize();
			if (len == 0)
				continue;

			t1 += len * mInvLength;
			if (t1 > 1.0f)
				t1 = 1.0f;

			Float3 camDir = a - camPos;
			camDir.Normalize();

			zAxis = dir;
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
	PS_Ribbon_RenderObject::PS_Ribbon_RenderObject()
		: mParent(NULL)
	{
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::COLOR, eVertexType::FLOAT4);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		mMaterial.cullMode = eCullMode::NONE;
		mMaterial.depthMode = eDepthMode::N_LESS_EQUAL;
		mMaterial.blendMode = eBlendMode::ADD;
	}

	PS_Ribbon_RenderObject::~PS_Ribbon_RenderObject()
	{
	}

	void PS_Ribbon_RenderObject::_getWorldPosition(Float3 & pos)
	{
		if (mParent->IsLocalSpace())
			pos = mParent->GetParent()->GetWorldPosition();
		else
			pos = Float3::Zero;
	}

	void PS_Ribbon_RenderObject::_getWorldBound(Aabb & bound)
	{
		bound = mParent->GetBound();
	}

	void PS_Ribbon_RenderObject::_getWorldTM(Mat4 & form)
	{
		form = Mat4::Identity;
	}

	void PS_Ribbon_RenderObject::_allocQuota(int quota)
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

	void PS_Ribbon_RenderObject::_update()
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

		mMaterial.maps[eMapType::DIFFUSE] = mParent->_getTexture();

		mShaderFX = mParent->_getShaderFX();
		SetRenderCallBack(eRenderCallBack::SHADER, mParent->GetShader().c_ptr());
	}

	void * PS_Ribbon_RenderObject::_lockVertex()
	{
		return mRenderOp.vertexBuffers[0]->Lock(eLockFlag::WRITE);
	}

	void * PS_Ribbon_RenderObject::_lockIndex()
	{
		return mRenderOp.indexBuffer->Lock(eLockFlag::WRITE);
	}

	void PS_Ribbon_RenderObject::_unlock(int primCount)
	{
		mRenderOp.vertexBuffers[0]->Unlock();
		mRenderOp.indexBuffer->Unlock();

		mRenderOp.primCount = primCount;
	}

}
#include "PSSlice.h"
#include "PSSliceSet.h"

namespace Rad {

	PS_Slice::PS_Slice(PS_SliceSet * parent)
		: mParent(parent)
	{
		mSegment = parent->GetSegment();
		mSegmentLife = parent->GetSegmentLife();
	}

	PS_Slice::~PS_Slice()
	{
	}

	void PS_Slice::_update(Particle * p, float frameTime)
	{
		mWidth = p->Size.x;
		mLength = p->Size.y;

		if (mParent->IsKeepAspect() && mParent->_getTexture()->GetHeight() > 0)
			mWidth = mLength * mParent->_getTexture()->_getAscept();

		mInvLength = 0;
		if (mLength > 0)
			mInvLength = 1 / mLength;

		mSegmentStep = mLength / mSegment;

		Float3 a, b;
		if (!mParent->IsUsePoint())
		{
			Float3 axis = Float3(1, 0, 0);

			Quat q;
			q.FromEulerAngle(p->Rotation);

			axis.TransformQ(q);

			if (!mParent->IsLocalSpace())
			{
				axis.TransformQ(mParent->GetParent()->GetWorldRotation());
			}

			a = p->Position;
			b = p->Position + axis * mWidth;
		}
		else
		{
			a = mParent->GetPoint1();
			b = mParent->GetPoint2();
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

		Elem newElem;
		newElem.life = mSegmentLife;
		newElem.a = a; 
		newElem.b = b;

		if (mElems.Size() > 2)
		{
			mElems[0] = newElem;

			if (mElems[0].b.DistanceSq(mElems[1].b) > mSegmentStep * mSegmentStep)
			{
				mElems.Insert(1, newElem);
			}
		}
		else
		{
			mElems.PushFront(newElem);
		}

		while (mElems.Size() > mSegment + 1)
		{
			mElems.PopBack();
		}

		if (mElems.Size() > 1)
		{
			mBound = Aabb::Invalid;
			for (int i = 0; i < mElems.Size(); ++i)
			{
				mBound.Merge(mElems[i].a);
				mBound.Merge(mElems[i].b);
			}
		}
		else
		{
			mBound = Aabb::Zero;
		}
	}

	void PS_Slice::_updateBuffer(Particle * p, void ** vertexBuffer, void ** indexBuffer, int & startVertex, int & primCount)
	{
		float t = 0, t1 = 0;

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
			Float3 a = mElems[i].a;
			Float3 b = mElems[i].b;
			Float3 c = mElems[i + 1].a;
			Float3 d = mElems[i + 1].b;

			if (mParent->IsLocalSpace() && !mParent->IsUsePoint())
			{
				a.TransformA(worldTM);
				b.TransformA(worldTM);
				c.TransformA(worldTM);
				d.TransformA(worldTM);
			}

			Float3 dir = d - b;
			float len = dir.Normalize();

			t1 += len * mInvLength;
			if (t1 > 1)
				t1 = 1;

			if (!first)
			{
				vb[2].position = c;
				vb[3].position = d;

				vb[2].uv = Float2(u0, v0 + t1 * vLength);
				vb[3].uv = Float2(u1, v0 + t1 * vLength);

				vb[2].color = p->Color;
				vb[3].color = p->Color;
			}
			else
			{
				vb[0].position = a;
				vb[1].position = b;
				vb[2].position = c;
				vb[3].position = d;

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
	PS_Slice_RenderObject::PS_Slice_RenderObject()
		: mParent(NULL)
	{
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::COLOR, eVertexType::FLOAT4);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		mMaterial.cullMode = eCullMode::NONE;
		mMaterial.depthMode = eDepthMode::N_LESS_EQUAL;
		mMaterial.blendMode = eBlendMode::ADD;
	}

	PS_Slice_RenderObject::~PS_Slice_RenderObject()
	{
	}

	void PS_Slice_RenderObject::_getWorldPosition(Float3 & pos)
	{
		if (mParent->IsLocalSpace())
			pos = mParent->GetParent()->GetWorldPosition();
		else
			pos = Float3::Zero;
	}

	void PS_Slice_RenderObject::_getWorldBound(Aabb & bound)
	{
		bound = mParent->GetBound();
	}

	void PS_Slice_RenderObject::_getWorldTM(Mat4 & form)
	{
		form = Mat4::Identity;
	}

	void PS_Slice_RenderObject::_allocQuota(int quota)
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

	void PS_Slice_RenderObject::_update()
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

	void * PS_Slice_RenderObject::_lockVertex()
	{
		return mRenderOp.vertexBuffers[0]->Lock(eLockFlag::WRITE);
	}

	void * PS_Slice_RenderObject::_lockIndex()
	{
		return mRenderOp.indexBuffer->Lock(eLockFlag::WRITE);
	}

	void PS_Slice_RenderObject::_unlock(int primCount)
	{
		mRenderOp.vertexBuffers[0]->Unlock();
		mRenderOp.indexBuffer->Unlock();

		mRenderOp.primCount = primCount;
	}
}
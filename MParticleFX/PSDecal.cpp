#include "PSDecal.h"
#include "PSDecalSet.h"

namespace Rad {

	PS_Decal::PS_Decal(PS_DecalSet * parent)
		: mParent(parent)
	{
		mXGrids = parent->GetGrids().x;
		mZGrids = parent->GetGrids().y;
	}

	PS_Decal::~PS_Decal()
	{
	}

	void PS_Decal::_update(Particle * p)
	{
		mWidth = p->Size.x;
		mLength = p->Size.z;

		if (mParent->IsKeepAspect())
			mWidth = mLength * mParent->_getTexture()->_getAscept();

		mInvWidth = mInvLength = 0;

		if (mWidth > 0)
			mInvWidth = 1 / mWidth;

		if (mLength > 0)
			mInvLength = 1 / mLength;

		mXStep = mWidth / mXGrids;
		mZStep = mLength / mZGrids;

		Float3 Position = p->Position;
		if (mParent->IsLocalSpace())
		{
			Position.TransformA(mParent->GetParent()->GetWorldTM());
		}

		const Float2 & center = mParent->GetCenter();
		const float y_bias = 1;
		float x_start = -mWidth * center.x;
		float z_start = mLength * center.y;

		Float3 xAxis = Float3(1, 0, 0), zAxis = Float3(0, 0, -1);

		Mat4 tm;
		tm.MakeRotationY(Math::DegreeToRadian(p->Rotation.x));

		xAxis.TransformN(tm);
		zAxis.TransformN(tm);

		if (mParent->IsLocalSpace())
		{
			xAxis.TransformQ(mParent->GetParent()->GetWorldRotation());
			zAxis.TransformQ(mParent->GetParent()->GetWorldRotation());
		}

		xAxis.y = 0;
		zAxis.y = 0;

		xAxis.Normalize();
		zAxis.Normalize();

		mPoints.Clear();

		mBound = Aabb::Invalid;

		float z = z_start;
		for (int j = 0; j <= mZGrids; ++j)
		{
			float x = x_start;
			for (int i = 0; i <= mXGrids; ++i)
			{
				Float3 pos = Position + x * xAxis + z * zAxis;

				pos.y = PS_Manager::d_get_height(pos.x, pos.y + y_bias, pos.z) + y_bias;

				mBound.Merge(pos);
				mPoints.PushBack(pos);

				x += mXStep;
			}

			z -= mZStep;
		}
	}

	void PS_Decal::_updateBuffer(Particle * p, void ** vertexBuffer, void ** indexBuffer, int & startVertex, int & primCount)
	{
		Camera * pCamera = World::Instance()->GetCurrentRenderContext()->GetCamera();
		Float3 camPos = pCamera->GetWorldPosition();

		float uLength = p->UVRect.x2 - p->UVRect.x1;
		float vLength = p->UVRect.y2 - p->UVRect.y1;

		float u_step = mXStep * mInvWidth * uLength;
		float v_step = mZStep * mInvLength * vLength;

		PS_Vertex * vb = (PS_Vertex  *)(*vertexBuffer);

		float v = p->UVRect.y1;
		for (int j = 0; j <= mZGrids; ++j)
		{
			float u = p->UVRect.x1;
			for (int i = 0; i <= mXGrids; ++i)
			{
				vb->position = mPoints[j * (mXGrids + 1) + i];
				vb->uv = Float2(u, v);
				vb->color = p->Color;
				vb += 1;

				u += u_step;
			}

			v += v_step;
		}

		unsigned short * ib = (unsigned short *)(*indexBuffer);
		for (int j = 0; j < mZGrids; ++j)
		{
			int c = j * (mXGrids + 1);
			int n = (j + 1) * (mXGrids + 1);

			for (int i = 0; i < mXGrids; ++i)
			{
				int a = c + i;
				int b = a + 1;
				int c = n + i;
				int d = c + 1;

				*ib++ = a + startVertex;
				*ib++ = b + startVertex;;
				*ib++ = c + startVertex;;

				*ib++ = c + startVertex;;
				*ib++ = b + startVertex;;
				*ib++ = d + startVertex;;
			}
		}

		*vertexBuffer = vb;
		*indexBuffer = ib;
		startVertex += (mXGrids + 1) * (mZGrids + 1);
		primCount += mXGrids * mZGrids * 2;
	}

	//
	PS_Decal_RenderObject::PS_Decal_RenderObject()
		: mParent(NULL)
	{
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::COLOR, eVertexType::FLOAT4);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		mMaterial.cullMode = eCullMode::NONE;
		mMaterial.depthMode = eDepthMode::N_LESS_EQUAL;
		mMaterial.blendMode = eBlendMode::ADD;
	}

	PS_Decal_RenderObject::~PS_Decal_RenderObject()
	{
	}

	void PS_Decal_RenderObject::_getWorldPosition(Float3 & pos)
	{
		if (mParent->IsLocalSpace())
			pos = mParent->GetParent()->GetWorldPosition();
		else
			pos = Float3::Zero;
	}

	void PS_Decal_RenderObject::_getWorldBound(Aabb & bound)
	{
		bound = mParent->GetBound();
	}

	void PS_Decal_RenderObject::_getWorldTM(Mat4 & form)
	{
		form = Mat4::Identity;
	}

	void PS_Decal_RenderObject::_allocQuota(int quota)
	{
		d_assert (quota > 0);

		int vertexCount = (mParent->GetGrids().x + 1) * (mParent->GetGrids().y + 1) * quota;
		int indexCount = mParent->GetGrids().x * mParent->GetGrids().y * 6 * quota;

		VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(sizeof(PS_Vertex), vertexCount, eUsage::DYNAMIC_MANAGED);
		IndexBufferPtr ib = HWBufferManager::Instance()->NewIndexBuffer(indexCount, eUsage::DYNAMIC_MANAGED);

		mRenderOp.vertexBuffers[0] = vb;
		mRenderOp.indexBuffer = ib;
		mRenderOp.primCount = 0;
		mRenderOp.primType = ePrimType::TRIANGLE_LIST;
	}

	void PS_Decal_RenderObject::_update()
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

	void * PS_Decal_RenderObject::_lockVertex()
	{
		return mRenderOp.vertexBuffers[0]->Lock(eLockFlag::WRITE);
	}

	void * PS_Decal_RenderObject::_lockIndex()
	{
		return mRenderOp.indexBuffer->Lock(eLockFlag::WRITE);
	}

	void PS_Decal_RenderObject::_unlock(int primCount)
	{
		mRenderOp.vertexBuffers[0]->Unlock();
		mRenderOp.indexBuffer->Unlock();

		mRenderOp.primCount = primCount;
	}

}
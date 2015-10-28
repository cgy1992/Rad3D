#include "MBillboard.h"
#include "MRenderQueue.h"
#include "MHWBufferManager.h"
#include "MHWBufferManager.h"
#include "MShaderFXManager.h"

namespace Rad {

	ImplementRTTI(Billboard, Node);

	Billboard::Billboard()
		: Node("Billboard")
		, mSize(UNIT_METRES, UNIT_METRES)
		, mTexcoord(0, 0, 1, 1)
		, mColor(1, 1, 1, 1)
		, mNeedUpdate(true)
	{
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT2);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::COLOR, eVertexType::FLOAT4);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		mRenderOp.vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 8, 4);

		mRenderOp.primCount = 2;
		mRenderOp.primType = ePrimType::TRIANGLE_STRIP;

		mShaderFX = ShaderFXManager::Instance()->Load("Billboard", "Shader/MBillboard.mfx");
		
		mMaterial.cullMode = eCullMode::NONE;
		mMaterial.depthMode = eDepthMode::N_LESS_EQUAL;
		mMaterial.blendMode = eBlendMode::ALPHA_BLEND;
	}

	Billboard::~Billboard()
	{
	}

	void Billboard::SetTexture(const String & texName)
	{
		mMaterial.maps[eMapType::DIFFUSE] = HWBufferManager::Instance()->LoadTexture(texName, -1);
	}

	void Billboard::SetTexture(TexturePtr tex)
	{
		mMaterial.maps[eMapType::DIFFUSE] = tex;
	}

	TexturePtr Billboard::GetTexture()
	{
		return mMaterial.maps[eMapType::DIFFUSE];
	}

	void Billboard::SetSize(const Float2 & size)
	{
		mSize = size;
		mNeedUpdate = true;
	}

	const Float2 & Billboard::GetSize()
	{
		return mSize;
	}

	void Billboard::SetTexcoord(const RectF & texcoord)
	{
		mTexcoord = texcoord;
		mNeedUpdate = true;
	}

	const RectF & Billboard::GetTexcoord()
	{
		return mTexcoord;
	}

	void Billboard::SetColor(const Float4 & color)
	{
		mColor = color;
		mNeedUpdate = true;
	}

	const Float4 & Billboard::GetColor()
	{
		return mColor;
	}

	void Billboard::AddRenderQueue(RenderQueue * RQ)
	{
		RQ->AddRenderObject(this);

		if (mNeedUpdate)
		{
			float half_w = mSize.x / 2;
			float half_h = mSize.y / 2;

			float * verts = (float *)mRenderOp.vertexBuffers[0]->Lock(eLockFlag::WRITE);
			{
				*verts++ = -half_w; *verts++ = half_h;
				*verts++ = mColor.r; *verts++ = mColor.g; *verts++ = mColor.b; *verts++ = mColor.a;
				*verts++ = mTexcoord.x1; *verts++ = mTexcoord.y1;

				*verts++ = half_w; *verts++ = half_h;
				*verts++ = mColor.r; *verts++ = mColor.g; *verts++ = mColor.b; *verts++ = mColor.a;
				*verts++ = mTexcoord.x2; *verts++ = mTexcoord.y1;

				*verts++ = -half_w; *verts++ = -half_h;
				*verts++ = mColor.r; *verts++ = mColor.g; *verts++ = mColor.b; *verts++ = mColor.a;
				*verts++ = mTexcoord.x1; *verts++ = mTexcoord.y2;

				*verts++ = half_w; *verts++ = -half_h;
				*verts++ = mColor.r; *verts++ = mColor.g; *verts++ = mColor.b; *verts++ = mColor.a;
				*verts++ = mTexcoord.x2; *verts++ = mTexcoord.y2;
			}
			mRenderOp.vertexBuffers[0]->Unlock();

			mNeedUpdate = false;
		}
	}
}
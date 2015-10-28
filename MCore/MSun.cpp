#include "MSun.h"
#include "MEnvironment.h"
#include "MShaderFXManager.h"
#include "MHWBufferManager.h"
#include "MWorld.h"

namespace Rad {

	Sun::Sun()
	{
		mTech = ShaderFXManager::Instance()->Load("Sun", "Shader/MSun.mfx");

		d_assert (mTech != NULL);

		int iVertexCount = 4;
		int iPrimCount = 2;

		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(20, iVertexCount);

		float * vert = (float *)vb->Lock(eLockFlag::WRITE);
		{
			float x = 0, y = 0, z = 0;

			*vert++ = x; *vert++ = y; *vert++ = z;
			*vert++ = 0; *vert++ = 0;

			*vert++ = x; *vert++ = y; *vert++ = z;
			*vert++ = 1; *vert++ = 0;

			*vert++ = x; *vert++ = y; *vert++ = z;
			*vert++ = 0; *vert++ = 1;

			*vert++ = x; *vert++ = y; *vert++ = z;
			*vert++ = 1; *vert++ = 1;
		}
		vb->Unlock();

		mRenderOp.vertexBuffers[0] = vb;

		mRenderOp.primCount = iPrimCount;
		mRenderOp.primType = ePrimType::TRIANGLE_STRIP;
	}

	Sun::~Sun()
	{
	}

	void Sun::Render()
	{
		Float3 sunPos = Environment::Instance()->GetEvParam()->SunPos;
		Float3 sunDir = Environment::Instance()->GetEvParam()->SunDir;
		float sunSize = Environment::Instance()->GetEvParam()->SunSize * UNIT_METRES;
		const String & sunTexture = Environment::Instance()->GetEvParam()->SunTexture;

		if (mTexture == NULL || mTexture->GetName() != sunTexture)
			mTexture = HWBufferManager::Instance()->LoadTexture(sunTexture, -1);

		mTech->GetPass(0)->SetUniform("u_Transform", Float4(sunPos.x, sunPos.y, sunPos.z, sunSize));

		RenderSystem::Instance()->SetWorldTM(Mat4::Identity);
		RenderSystem::Instance()->SetTexture(0, mTexture.c_ptr());
		RenderSystem::Instance()->Render(mTech, &mRenderOp);
	}

}
#include "MSky.h"
#include "MEnvironment.h"
#include "MShaderFXManager.h"
#include "MHWBufferManager.h"
#include "MWorld.h"

namespace Rad {

	Sky::Sky()
	{
		mTech = ShaderFXManager::Instance()->Load("Sky", "Shader/MSky.mfx");

		d_assert (mTech != NULL);

		int iRings = 30, iSegments = 30;
		int iVertexCount = (iRings + 1) * (iSegments + 1);
		int iIndexCount = iRings * iSegments * 6;
		int iPrimCount = iIndexCount / 3;

		d_assert(iIndexCount < 65535);

		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

		VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 3, iVertexCount);

		float * vert = (float *)vb->Lock(eLockFlag::WRITE);
		{
			float fTileRingAngle = (PI / iRings);
			float fTileSegAngle = (PI * 2 / iSegments);
			float r;
			short i, j;
			float x, y, z;

			for (i = 0; i <= iRings; ++i)
			{
				r = Math::Sin(i * fTileRingAngle);
				y = Math::Cos(i * fTileRingAngle);

				for (j = 0; j <= iSegments; ++j)
				{
					x = r * Math::Cos(j * fTileSegAngle);
					z = r * Math::Sin(j * fTileSegAngle);

					*vert++ = x;
					*vert++ = y;
					*vert++ = z;
				}

			}
		}
		vb->Unlock();

		IndexBufferPtr ib = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);
		short * idx = (short *)ib->Lock(eLockFlag::WRITE);
		{
			short row = 0, row_n = 0;
			short i, j;

			for (i = 0; i < iRings; ++i)
			{
				row_n = row + iSegments + 1;

				for (j = 0; j < iSegments; ++j)
				{
					*idx++ = row + j;
					*idx++ = row + j + 1;
					*idx++ = row_n + j;

					*idx++ = row_n + j;
					*idx++ = row + j + 1;
					*idx++ = row_n + j + 1;

				}

				row += iSegments + 1;
			}
		}
		ib->Unlock();

		mRenderOp.vertexBuffers[0] = vb;
		mRenderOp.indexBuffer = ib;
		mRenderOp.primCount = iPrimCount;
		mRenderOp.primType = ePrimType::TRIANGLE_LIST;
	}

	Sky::~Sky()
	{
	}

	void Sky::Render()
	{
		float skyLum = Environment::Instance()->GetEvParam()->SkyLum;
		float skyU = Environment::Instance()->GetEvParam()->SkyU;
		float skyV = Environment::Instance()->GetEvParam()->SkyV;
		const String & skyTexture = Environment::Instance()->GetEvParam()->SkyTexture;

		if (mTexture == NULL || mTexture->GetName() != skyTexture)
			mTexture = HWBufferManager::Instance()->LoadTexture(skyTexture, -1);

		RenderSystem * render = RenderSystem::Instance();
		Camera * cam = World::Instance()->GetCurrentRenderContext()->GetCamera();

		d_assert (cam != NULL);

		float farclip = cam->GetFarClip() * 0.9f;
		Float3 pos = cam->GetPosition();

		Mat4 worldTM;
		worldTM.MakeTransform(pos, Quat::Identity, Float3(farclip, farclip, farclip));

		mTech->GetPass(0)->SetUniform("u_Param", Float4(skyU, skyV, 1 / (1 + skyV), skyLum));

		RenderSystem::Instance()->SetWorldTM(worldTM);
		RenderSystem::Instance()->SetTexture(0, mTexture.c_ptr());
		render->Render(mTech, &mRenderOp);
	}

}
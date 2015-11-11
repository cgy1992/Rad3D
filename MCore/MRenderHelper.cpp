#include "MRenderHelper.h"
#include "MRenderSystem.h"

namespace Rad {

	ImplementSingleton(RenderHelper);

	RenderHelper::RenderHelper()
	{  
		mWhiteTexture = HWBufferManager::Instance()->NewTexture("Core_WhiteTexture", 4, 4, 0);
		{
			int * color = (int *)mWhiteTexture->Lock(eLockFlag::WRITE);

			for (int i = 0; i < 4 * 4; ++i)
			{
				*color++ = M_RGBA(255, 255, 255, 255);
			}

			mWhiteTexture->Unlock();
		}

		mBlackTexture = HWBufferManager::Instance()->NewTexture("Core_BlackTexture", 4, 4, 0);
		{
			int * color = (int *)mBlackTexture->Lock(eLockFlag::WRITE);

			for (int i = 0; i < 4 * 4; ++i)
			{
				*color++ = M_RGBA(0, 0, 0, 255);
			}

			mBlackTexture->Unlock();
		}

		mEmptyTexture = HWBufferManager::Instance()->NewTexture("Core_EmptyTexture", 4, 4, 0);
		{
			int * color = (int *)mEmptyTexture->Lock(eLockFlag::WRITE);

			for (int i = 0; i < 4 * 4; ++i)
			{
				*color++ = M_RGBA(0, 0, 0, 0);
			}

			mEmptyTexture->Unlock();
		}

		mDefaultNormalMap = HWBufferManager::Instance()->NewTexture("Core_DefaultNormalMap", 4, 4, 0);
		{
			int * color = (int *)mEmptyTexture->Lock(eLockFlag::WRITE);

			for (int i = 0; i < 4 * 4; ++i)
			{
				*color++ = M_RGBA(128, 128, 255, 255);
			}

			mEmptyTexture->Unlock();
		}

		mDefaultLightingMap = HWBufferManager::Instance()->NewTexture("Core_DefaultLightingMap", 4, 4, 0);
		{
			int * color = (int *)mDefaultLightingMap->Lock(eLockFlag::WRITE);

			for (int i = 0; i < 4 * 4; ++i)
			{
				*color++ = M_RGBA(255, 255, 255, 0);
			}

			mDefaultLightingMap->Unlock();
		}

		// 
		mScreenQuad.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT4);
		mScreenQuad.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		mScreenQuad.vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 6, 4);

		mScreenQuad.primCount = 2;
		mScreenQuad.primType = ePrimType::TRIANGLE_STRIP;

		//
		mDebugVertexDecl.AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

		mDebugShaderFX = ShaderFXManager::Instance()->Load("Debug", "Shader/MDebug.mfx");
		d_assert (mDebugShaderFX != NULL && mDebugShaderFX->GetPassCount() > 0);

		//
		mSubmitShaderFX = ShaderFXManager::Instance()->Load("Submit", "Shader/MSubmit.mfx");

		OnResize();
	}

	RenderHelper::~RenderHelper()
	{
	}

	void RenderHelper::OnResize()
	{
		_updateScreenQuad(RenderSystem::Instance()->GetConfig().width, RenderSystem::Instance()->GetConfig().height);
	}

	void RenderHelper::_updateScreenQuad(int w, int h)
	{
		float halfInvWidth = 0.5f / w;
		float halfInvHeight = 0.5f / h;

		float * vert = (float *)mScreenQuad.vertexBuffers[0]->Lock(eLockFlag::WRITE);
		{
			float x = 0, y = 0, z = 0;

			*vert++ = -1; *vert++ = 1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 0 + halfInvWidth; *vert++ = 0 + halfInvHeight;
			*vert++ = 0; *vert++ = 0;

			*vert++ = 1; *vert++ = 1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 1 + halfInvWidth; *vert++ = 0 + halfInvHeight;
			*vert++ = 1; *vert++ = 0;

			*vert++ = -1; *vert++ = -1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 0 + halfInvWidth; *vert++ = 1 + halfInvHeight;
			*vert++ = 0; *vert++ = 1;

			*vert++ = 1; *vert++ = -1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 1 + halfInvWidth; *vert++ = 1 + halfInvHeight;
			*vert++ = 1; *vert++ = 1;
		}
		mScreenQuad.vertexBuffers[0]->Unlock();
	}

	void RenderHelper::DrawScreenQuad(ShaderFX * fx)
	{
		RenderSystem::Instance()->SetWorldTM(Mat4::Identity);

		for (int i = 0; i < fx->GetPassCount(); ++i)
		{
			RenderSystem::Instance()->SetShaderPass(fx->GetPass(i), true);
			RenderSystem::Instance()->Render(&mScreenQuad);
		}
	}

	void RenderHelper::DrawSumit(Texture * texture)
	{
		RenderSystem::Instance()->SetRenderTarget(NULL);
		RenderSystem::Instance()->SetDepthBuffer(NULL);

		RenderSystem::Instance()->SetTexture(0, texture);

		DrawScreenQuad(mSubmitShaderFX);
	}

	void RenderHelper::DebugDrawLine(const Float3 & point1, const Float3 & point2, const Float4 & color, const Mat4 & form)
	{
		Float3 vb[2];
		vb[0] = point1;
		vb[1] = point2;

		RenderSystem::Instance()->SetWorldTM(form);

		mDebugShaderFX->GetPass(0)->SetConst("u_Color", color);
		RenderSystem::Instance()->SetShaderPass(mDebugShaderFX->GetPass(0), false);
		RenderSystem::Instance()->RenderEx(&mDebugVertexDecl, vb, NULL, ePrimType::LINE_LIST, 1);
	}

	void RenderHelper::DebugDrawLines(const Float3 * points, int count, const Float4 & color, const Mat4 & form, bool strip)
	{
		RenderSystem::Instance()->SetWorldTM(form);

		mDebugShaderFX->GetPass(0)->SetConst("u_Color", color);
		RenderSystem::Instance()->SetShaderPass(mDebugShaderFX->GetPass(0), false);
		RenderSystem::Instance()->RenderEx(&mDebugVertexDecl, points, NULL, strip ? ePrimType::LINE_STRIP : ePrimType::LINE_LIST, count);
	}

	void RenderHelper::DebugDrawTriangles(const Float3 * points, int count, const Float4 & color, const Mat4 & form)
	{
		RenderSystem::Instance()->SetWorldTM(form);

		mDebugShaderFX->GetPass(0)->SetConst("u_Color", color);
		RenderSystem::Instance()->SetShaderPass(mDebugShaderFX->GetPass(0), false);
		RenderSystem::Instance()->RenderEx(&mDebugVertexDecl, points, NULL, ePrimType::TRIANGLE_LIST, count);
	}

	void RenderHelper::DebugDrawBox(const Obb & obb, const Float4 & color, const Mat4 & form)
	{
		Float3 vb[8];
		short ib[36];

		obb.GetPoints(vb);

		int i = 0;

		// front
		ib[i++] = 0; ib[i++] = 2; ib[i++] = 1;
		ib[i++] = 1; ib[i++] = 2; ib[i++] = 3;

		// back
		ib[i++] = 5; ib[i++] = 7; ib[i++] = 4;
		ib[i++] = 4; ib[i++] = 7; ib[i++] = 6;

		// left
		ib[i++] = 4; ib[i++] = 6; ib[i++] = 0;
		ib[i++] = 0; ib[i++] = 6; ib[i++] = 1;

		//right
		ib[i++] = 1; ib[i++] = 3; ib[i++] = 5;
		ib[i++] = 5; ib[i++] = 3; ib[i++] = 7;

		// top
		ib[i++] = 2; ib[i++] = 6; ib[i++] = 3;
		ib[i++] = 3; ib[i++] = 6; ib[i++] = 7;

		// bottom
		ib[i++] = 4; ib[i++] = 0; ib[i++] = 5;
		ib[i++] = 5; ib[i++] = 0; ib[i++] = 1;

		RenderSystem::Instance()->SetWorldTM(form);

		mDebugShaderFX->GetPass(0)->SetConst("u_Color", color);
		RenderSystem::Instance()->SetShaderPass(mDebugShaderFX->GetPass(0), false);
		RenderSystem::Instance()->RenderEx(&mDebugVertexDecl, vb, ib, ePrimType::TRIANGLE_LIST, 12);
	}

	void RenderHelper::DebugDrawRect(const RectF & rect, const Float4 & color, const Mat4 & form)
	{
		Float3 vb[4];
		short ib[6];

		vb[0].x = rect.x1; vb[0].y = 0; vb[0].z = rect.y1;
		vb[1].x = rect.x2; vb[1].y = 0; vb[1].z = rect.y1;
		vb[2].x = rect.x1; vb[2].y = 0; vb[2].z = rect.y2;
		vb[3].x = rect.x2; vb[3].y = 0; vb[3].z = rect.y2;

		ib[0] = 0; ib[1] = 1; ib[2] = 2;
		ib[3] = 2; ib[4] = 1; ib[5] = 3;

		RenderSystem::Instance()->SetWorldTM(form);

		mDebugShaderFX->GetPass(0)->SetConst("u_Color", color);
		RenderSystem::Instance()->SetShaderPass(mDebugShaderFX->GetPass(0), false);
		RenderSystem::Instance()->RenderEx(&mDebugVertexDecl, vb, ib, ePrimType::TRIANGLE_LIST, 2);
	}

	void RenderHelper::DebugDrawRectFrames(const RectF & rect, int xTile, int yTile, const Float4 & color, const Mat4 & form)
	{
		d_assert (xTile * yTile > 1);

		Array< Float3, t_alloc_pool<Float3> > vb;
		Array< short, t_alloc_pool<short> > ib;
		Float3 p1, p2;

		int startVertex = 0;

		float y_step = rect.Height() / yTile;
		for (int i = 0; i <= yTile; ++i)
		{
			p1.x = rect.x1; p1.y = 0; p1.z = rect.y1 + i * y_step;
			p2.x = rect.x2; p2.y = 0; p2.z = rect.y1 + i * y_step;

			vb.PushBack(p1);
			vb.PushBack(p2);

			ib.PushBack(startVertex + 0);
			ib.PushBack(startVertex + 1);

			startVertex += 2;
		}

		float x_step = rect.Width() / xTile;
		for (int i = 0; i <= xTile; ++i)
		{
			p1.x = rect.x1 + i * x_step; p1.y = 0; p1.z = rect.y1;
			p2.x = rect.x1 + i * x_step; p2.y = 0; p2.z = rect.y2;

			vb.PushBack(p1);
			vb.PushBack(p2);

			ib.PushBack(startVertex + 0);
			ib.PushBack(startVertex + 1);

			startVertex += 2;
		}

		RenderSystem::Instance()->SetWorldTM(form);

		mDebugShaderFX->GetPass(0)->SetConst("u_Color", color);
		RenderSystem::Instance()->SetShaderPass(mDebugShaderFX->GetPass(0), false);
		RenderSystem::Instance()->RenderEx(&mDebugVertexDecl, vb.c_ptr(), ib.c_ptr(), ePrimType::LINE_LIST, ib.Size() / 2);
	}

	void RenderHelper::DebugDrawSphere(const Sphere & sph, const Float4 & color, const Mat4 & form)
	{
		const int iRings = 10;
		const int iSegments = 10;
		const int iVertexCount = (iRings + 1) * (iSegments + 1) * 2;
		const int iIndexCount = iRings * iSegments * 6;
		const int iPrimCount = iIndexCount / 3;

		Float3 vb[iVertexCount];

		float * verteces = (float *)vb;
		{
			float fTileRingAngle = (PI / iRings);
			float fTileSegAngle = (PI * 2 / iSegments);
			float r;
			short i, j;
			Float3 pos;

			for (i = 0; i <= iRings; ++i)
			{
				r = sph.radius * Math::FastSin(i * fTileRingAngle);
				pos.y = sph.radius * Math::FastCos(i * fTileRingAngle);

				for (j = 0; j <= iSegments; ++j)
				{
					pos.x = r * Math::FastCos(j * fTileSegAngle);
					pos.z = r * Math::FastSin(j * fTileSegAngle);

					*verteces++ = pos.x + sph.center.x;
					*verteces++ = pos.y + sph.center.y;
					*verteces++ = pos.z + sph.center.z;
				}

			}
		}

		short ib[iIndexCount];
		short * indices = (short *)ib;
		{
			short row = 0, row_n = 0;
			short i, j;

			for (i = 0; i < iRings; ++i)
			{
				row_n = row + iSegments + 1;

				for (j = 0; j < iSegments; ++j)
				{
					*indices++ = row + j;
					*indices++ = row + j + 1;
					*indices++ = row_n + j;

					*indices++ = row_n + j;
					*indices++ = row + j + 1;
					*indices++ = row_n + j + 1;

				}

				row += iSegments + 1;
			}
		}

		RenderSystem::Instance()->SetWorldTM(form);

		mDebugShaderFX->GetPass(0)->SetConst("u_Color", color);
		RenderSystem::Instance()->SetShaderPass(mDebugShaderFX->GetPass(0), false);
		RenderSystem::Instance()->RenderEx(&mDebugVertexDecl, vb, ib, ePrimType::TRIANGLE_LIST, iPrimCount);
	}

	void RenderHelper::DebugDraw(RenderOp * rop, const Float4 & color, const Mat4 & form)
	{
		RenderSystem::Instance()->SetWorldTM(form);

		mDebugShaderFX->GetPass(0)->SetConst("u_Color", color);
		RenderSystem::Instance()->SetShaderPass(mDebugShaderFX->GetPass(0), false);
		RenderSystem::Instance()->Render(rop);
	}
}
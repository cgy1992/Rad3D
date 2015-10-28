/*
*	RenderHelper
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSingleton.h"
#include "MRenderObject.h"

namespace Rad {

	class M_ENTRY RenderHelper : public Singleton<RenderHelper>
	{
	public:
		RenderHelper();
		~RenderHelper();

		void 
			OnResize();

		TexturePtr 
			GetWhiteTexture() { return mWhiteTexture; }
		TexturePtr 
			GetBlackTexture() { return mBlackTexture; }
		TexturePtr 
			GetEmptyTexture() { return mEmptyTexture; }
		TexturePtr
			GetDefaultNormalMap() { return mDefaultNormalMap; }
		TexturePtr
			GetDefaultLightingMap() { return mDefaultLightingMap; }
		ShaderFX * 
			GetDebugShader() { return mDebugShaderFX; }

		void 
			DrawScreenQuad(ShaderFX * fx);
		void
			DrawSumit(Texture * texture);
		void 
			DebugDrawLine(const Float3 & point1, const Float3 & point2, const Float4 & color, const Mat4 & form);
		void 
			DebugDrawLines(const Float3 * points, int count, const Float4 & color, const Mat4 & form, bool strip);
		void 
			DebugDrawTriangles(const Float3 * points, int count, const Float4 & color, const Mat4 & form);
		void 
			DebugDrawBox(const Obb & obb, const Float4 & color, const Mat4 & form);
		void
			DebugDrawRect(const RectF & rect, const Float4 & color, const Mat4 & form);
		void
			DebugDrawRectFrames(const RectF & rect, int xTile, int yTile, const Float4 & color, const Mat4 & form);
		void 
			DebugDrawSphere(const Sphere & sph, const Float4 & color, const Mat4 & form);
		void
			DebugDraw(RenderOp * rop, const Float4 & color, const Mat4 & form);
		
	protected:
		void
			_updateScreenQuad(int w, int h);

	protected:
		TexturePtr mWhiteTexture;
		TexturePtr mBlackTexture;
		TexturePtr mEmptyTexture;
		TexturePtr mDefaultNormalMap;
		TexturePtr mDefaultLightingMap;

		RenderOp mScreenQuad;

		VertexDeclaration mDebugVertexDecl;
		ShaderFX * mDebugShaderFX;

		ShaderFX * mSubmitShaderFX;
	};

}
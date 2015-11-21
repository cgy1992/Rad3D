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
#include "MRenderTarget.h"

namespace Rad {

	class M_ENTRY RenderHelper : public Singleton<RenderHelper>
	{
	public:
		RenderHelper();
		~RenderHelper();

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
			GetDebugFX() { return mDebugFX; }

		void
			DrawSumit(const Viewport & vp, Texture * texture, RenderTarget * target = NULL);
		void 
			DebugDrawLine(const Float3 & point1, const Float3 & point2, const Float4 & color, const Mat4 & form);
		void 
			DebugDrawBox(const Obb & obb, const Float4 & color, const Mat4 & form);
		void
			DebugDrawRect(const RectF & rect, const Float4 & color, const Mat4 & form);
		void
			DebugDrawRectFrames(const RectF & rect, int xTile, int yTile, const Float4 & color, const Mat4 & form);
		void 
			DebugDrawSphere(const Sphere & sph, const Float4 & color, const Mat4 & form);
		void 
			DebugDraw(const Float3 * points, ePrimType primType, int primCount, const Float4 & color, const Mat4 & form);
		void
			DebugDraw(RenderOp * rop, const Float4 & color, const Mat4 & form);
		
	protected:
		TexturePtr mWhiteTexture;
		TexturePtr mBlackTexture;
		TexturePtr mEmptyTexture;
		TexturePtr mDefaultNormalMap;
		TexturePtr mDefaultLightingMap;

		VertexDeclaration mDebugVertexDecl;
		ShaderFX * mDebugFX;
		ShaderFX * mSubmitFX;
	};

}
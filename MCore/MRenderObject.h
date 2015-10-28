/*
*	RenderObject
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MField.h"
#include "MVertexDeclaration.h"
#include "MVertexBuffer.h"
#include "MIndexBuffer.h"
#include "MTexture.h"
#include "MShaderFX.h"
#include "MShaderProvider.h"

namespace Rad {

	//
	struct M_ENTRY Material
	{
		eFillMode fillMode;
		eCullMode cullMode;
		eDepthMode depthMode;
		eBlendMode blendMode;

		Float3 emissive;
		Float3 ambient;
		Float3 diffuse;
		Float3 specular;
		float shininess;

		Field<TexturePtr, eMapType::MAX> maps;

		Material();
	};

	//
	struct M_ENTRY RenderOp
	{
		Field<VertexDeclaration, MAX_HW_VERTEX_STREAM> vertexDeclarations;
		Field<VertexBufferPtr, MAX_HW_VERTEX_STREAM> vertexBuffers;
		IndexBufferPtr indexBuffer;
		ePrimType primType;
		int primCount;

		RenderOp();
	};

	//
	class Node;
	class RenderCallBack;

	class M_ENTRY RenderObject
	{
	public:
		RenderObject();
		virtual ~RenderObject();

		Material * 
			GetMaterial() { return &mMaterial; }
		
		RenderOp *
			GetRenderOp() { return &mRenderOp; }

		void 
			SetShaderFX(ShaderFX * fx);
		ShaderFX * 
			GetShaderFX() { return mShaderFX; }

		void 
			SetLightShaderFX(eLightType type, ShaderFX * fx);
		ShaderFX * 
			GetLightShaderFX(eLightType type) { return mLightingShaderFX[type];}

		void 
			SetCurrentShaderFX(ShaderFX * fx);
		ShaderFX * 
			GetCurrentShaderFX() { return mCurrentShaderFX; }

		void 
			SetShaderProvider(ShaderProviderPtr provider);

		ShaderProviderPtr
			GetShaderProvider() { return mShaderProvider; }

		void
			SetRenderCallBack(eRenderCallBack i, RenderCallBack * callback);
		RenderCallBack *
			GetRenderCallBack(int i);

		bool 
			IsSkined() { return _getBoneTM(NULL) > 0; }
		
		virtual void 
			_preRendering() {}
		virtual void
			_postRendering() {}
		virtual void 
			_getWorldPosition(Float3 & pos) = 0;
		virtual void 
			_getWorldBound(Aabb & bound) = 0;
		virtual void 
			_getWorldTM(Mat4 & form) = 0;
		virtual int 
			_getBoneTM(Mat4 * boneTM) { return 0; }
		virtual Node *
			_getNode() { return NULL; }
		float
			_getOpacity();

		bool
			IsSortSpecial();
		virtual int
			SortSpecial(RenderObject * obj2, const Float3 & eye) { return -1; }

	protected:
		
		Material mMaterial;
		RenderOp mRenderOp;

		ShaderFX * mShaderFX;
		Field<ShaderFX *, eLightType::MAX> mLightingShaderFX;
		ShaderProviderPtr mShaderProvider;

		ShaderFX * mCurrentShaderFX;

		Field<RenderCallBack *, eRenderCallBack::MAX> mRenderCallBack;

		bool mSortSpecial;
	};

	//
	class M_ENTRY RenderCallBack
	{
	public:
		RenderCallBack() {}
		virtual ~RenderCallBack() {}

		virtual void
			OnCallBack(RenderObject * obj) = 0;
	};

}



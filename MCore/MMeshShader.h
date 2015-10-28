/*
*	MeshShader
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"

namespace Rad {

	class M_ENTRY MeshShader : public ShaderProvider, public RenderCallBack
	{
		DECLARE_RTTI();

	public:
		MeshShader() {}
		virtual ~MeshShader() {}

		virtual void
			Update(float frameTime) {}
	};

	typedef SmartPtr<MeshShader> MeshShaderPtr;

	//
	class M_ENTRY MeshShaderNull : public MeshShader
	{
		DECLARE_RTTI();

	public:
		MeshShaderNull() {}
		virtual ~MeshShaderNull() {}

		virtual void 
			ApplyShaderFX(RenderObject * able, int flags = 0) {}
		virtual void 
			ApplyLightingFX(Light * light, RenderObject * able) {}

		virtual void
			OnCallBack(RenderObject * obj) { d_assert(0); }
	};

	//
	class M_ENTRY MeshShaderStandard : public MeshShader
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(MeshShader);

	public:
		MeshShaderStandard();
		virtual ~MeshShaderStandard();

		virtual void 
			ApplyShaderFX(RenderObject * able, int flags = 0);
		virtual void 
			ApplyLightingFX(Light * light, RenderObject * able);

		virtual void
			OnCallBack(RenderObject * obj);

	protected:
		Float2 mUVScroll;

		ShaderFX * mBaseFX;
		ShaderFX * mBaseFXSkined;

		ShaderFX * mLightingColorFX;
		ShaderFX * mLightingMapFX;

		Field<ShaderFX *, eLightType::MAX> mLightingFX;
		Field<ShaderFX *, eLightType::MAX> mLightingFXSkined;
	};

}
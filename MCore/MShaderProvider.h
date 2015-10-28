/*
*	ShaderProvider
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MField.h"
#include "MShaderFX.h"

namespace Rad {

	class RenderObject;
	class Light;

	class M_ENTRY ShaderProvider : public IObject
	{
		DECLARE_REF();

	public:
		ShaderProvider() {}
		virtual ~ShaderProvider() {}

		void
			Release() { delete this; }

		virtual void 
			ApplyShaderFX(RenderObject * able, int flags = 0) = 0;
		virtual void 
			ApplyLightingFX(Light * light, RenderObject * able) = 0;
	};

	typedef SmartPtr<ShaderProvider> ShaderProviderPtr;

	//
	class M_ENTRY ShaderProviderStandard : public ShaderProvider
	{
	public:
		ShaderProviderStandard();
		virtual ~ShaderProviderStandard();

		virtual void 
			ApplyShaderFX(RenderObject * able, int flags);
		virtual void 
			ApplyLightingFX(Light * light, RenderObject * able);

	protected:
		ShaderFX * mBaseFX;
		ShaderFX * mBaseFXSkined;

		ShaderFX * mLightingColorFX;
		ShaderFX * mLightingMapFX;

		Field<ShaderFX *, eLightType::MAX> mLightingFX;
		Field<ShaderFX *, eLightType::MAX> mLightingFXSkined;
	};

}
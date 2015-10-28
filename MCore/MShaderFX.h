/*
*	ShaderFX
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"
#include "MTexture.h"

namespace Rad {

	struct M_ENTRY RenderState
	{
		eFillMode FillMode;
		eCullMode CullMode;
		eBlendMode BlendMode;
		eDepthMode DepthMode;
		eColorMode ColorMode;
		
		RenderState()
			: FillMode(eFillMode::SOLID)
			, CullMode(eCullMode::BACK)
			, BlendMode(eBlendMode::OPACITY)
			, DepthMode(eDepthMode::LESS_EQUAL)
			, ColorMode(eColorMode::ALL)
		{
		}
	};

	struct M_ENTRY FX_Uniform
	{
		FixedString32 Name;
		eSPType Type;
		int	Index;
		int	Count;
		void * data;

		FX_Uniform()
			: Type(eSPType::FLOAT4)
			, Index(0)
			, Count(1)
			, data(NULL)
		{
		}
	};

	struct M_ENTRY FX_Sampler
	{
		int	Index;
		FixedString32 Name;
		eSamplerBindType BindType;
		eTexFilter Filter; 
		eTexAddress Address;
		Float4 BorderColor;
		TexturePtr SamplerTexture;

		FX_Sampler()
			: Index(0)
			, BindType(eSamplerBindType::NONE)
			, Filter(eTexFilter::DEFAULT)
			, Address(eTexAddress::WRAP)
			, BorderColor(0, 0, 0, 0)
		{
		}
	};

	class M_ENTRY ShaderPass
	{
	public:
		ShaderPass();
		virtual ~ShaderPass();

		RenderState * 
			GetRenderState();

		int 
			GetUniformIndex(const FixedString32 & name);

		void 
			SetUniform(int index, const Float4 * data, int count);
		bool 
			SetUniform(const FixedString32 & name, const Float4 * data, int count);
		void 
			SetUniform(int index, const Float4 & data);
		bool 
			SetUniform(const FixedString32 & name, const Float4 & data);
		void 
			SetUniform(int index, const Mat4 & data);
		bool 
			SetUniform(const FixedString32 & name, const Mat4 & data);

		int 
			GetUniformCount() const;
		FX_Uniform * 
			GetUniform(int index);

		int 
			GetSamplerCount() const;
		FX_Sampler * 
			GetSampler(int index);

		virtual ShaderPass * 
			Clone() = 0;

	protected:
		RenderState mRenderState;
		FixedArray<FX_Uniform *, MAX_HW_UNIFORM> mUniforms;
		FixedArray<FX_Sampler *, MAX_HW_SAMPLER> mSamplers;
	};

	class M_ENTRY ShaderFX
	{
		DECLARE_ALLOC();

	public:
		ShaderFX(const String & name, const String & source, const String & macros);
		~ShaderFX();

		const String & 
			GetName();
		const String & 
			GetSource();
		const String & 
			GetMacros();

		void 
			AddPass(ShaderPass * pass);
		void 
			InsertPass(int i, ShaderPass * pass);
		void 
			RemovePass(int index);
		void 
			Clear();

		ShaderPass * 
			GetPass(int index) { return mPasses[index]; }
		int 
			GetPassCount() { return mPasses.Size(); }

	protected:
		String mName;
		String mSource;
		String mMacros;
		FixedArray<ShaderPass *, MAX_HW_SHADER_PASS> mPasses;
	};

}

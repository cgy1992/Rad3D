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

		inline void SetConst(float x, float y, float z, float w)
		{
			SetConst(Float4(x, y, z, w));
		}

		inline void SetConst(const Float4 & v)
		{
			d_assert (Type <= eSPType::FLOAT4);

			*(Float4 *)data = v;
		}

		inline void SetConst(const Mat4 & v)
		{
			d_assert (Type == eSPType::MATRIX4);

			*(Mat4 *)data = v;
		}

		inline void SetConst(const Float4 * v, int count)
		{
			d_assert (Type == eSPType::FLOAT4 && Count >= count);

			Float4 * d = (Float4 *)data;
			for (int i = 0; i < count; ++i)
			{
				*d++ = *v++;
			}
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

		virtual ShaderPass * 
			Clone() = 0;

		RenderState * 
			GetRenderState();

		int 
			GetUniformIndex(const FixedString32 & name);
		int 
			GetUniformCount() const;
		FX_Uniform * 
			GetUniform(int index);
		FX_Uniform *
			GetUniform(const FixedString32 & name);

		int 
			GetSamplerCount() const;
		FX_Sampler * 
			GetSampler(int index);

		void
			SetConst(int index, float x, float y, float z, float w);
		void
			SetConst(const FixedString32 & name, float x, float y, float z, float w);
		void 
			SetConst(int index, const Float4 * data, int count);
		bool 
			SetConst(const FixedString32 & name, const Float4 * data, int count);
		void 
			SetConst(int index, const Float4 & data);
		bool 
			SetConst(const FixedString32 & name, const Float4 & data);
		void 
			SetConst(int index, const Mat4 & data);
		bool 
			SetConst(const FixedString32 & name, const Mat4 & data);

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

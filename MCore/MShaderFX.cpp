#include "MShaderFX.h"
#include "MResourceManager.h"

namespace Rad {

	ShaderPass::ShaderPass()
	{
	}

	ShaderPass::~ShaderPass()
	{
	}

	RenderState * ShaderPass::GetRenderState()
	{
		return &mRenderState;
	}

	int ShaderPass::GetUniformIndex(const FixedString32 & name)
	{
		for (int i = 0; i < mUniforms.Size(); ++i)
		{
			FX_Uniform * uniform = mUniforms[i];

			if (uniform->Name == name)
				return i;
		}

		return -1;
	}

	void ShaderPass::SetUniform(int index, const Float4 * data, int count)
	{
		FX_Uniform * uniform = mUniforms[index];

		if (uniform->data)
		{
			memcpy(uniform->data, data, count * 16);
		}
	}

	bool ShaderPass::SetUniform(const FixedString32 & name, const Float4 * data, int count)
	{
		for (int i = 0; i < mUniforms.Size(); ++i)
		{
			FX_Uniform * uniform = mUniforms[i];

			if (uniform->Name == name)
			{
				SetUniform(i, data, count);
				return true;
			}
		}

		return false;
	}

	void ShaderPass::SetUniform(int index, const Float4 & data)
	{
		SetUniform(index, &data, 1);
	}

	bool ShaderPass::SetUniform(const FixedString32 & name, const Float4 & data)
	{
		return SetUniform(name, &data, 1);
	}

	void ShaderPass::SetUniform(int index, const Mat4 & data)
	{
		SetUniform(index, (const Float4 *)&data, 4);
	}

	bool ShaderPass::SetUniform(const FixedString32 & name, const Mat4 & data)
	{
		return SetUniform(name, (const Float4 *)&data, 4);
	}

	int ShaderPass::GetUniformCount() const
	{
		return mUniforms.Size();
	}

	FX_Uniform * ShaderPass::GetUniform(int index)
	{
		return mUniforms[index];
	}

	int ShaderPass::GetSamplerCount() const
	{
		return mSamplers.Size();
	}

	FX_Sampler * ShaderPass::GetSampler(int index)
	{
		return mSamplers[index];
	}

	//
	ShaderFX::ShaderFX(const String & name, const String & source, const String & macros)
		: mName (name)
		, mSource(source)
		, mMacros(macros)
	{
	}

	ShaderFX::~ShaderFX()
	{
		Clear();
	}

	const String & ShaderFX::GetName()
	{
		return mName;
	}

	const String & ShaderFX::GetSource()
	{
		return mSource;
	}

	const String & ShaderFX::GetMacros()
	{
		return mMacros;
	}

	void ShaderFX::AddPass(ShaderPass * pass)
	{
		mPasses.PushBack(pass);
	}

	void ShaderFX::InsertPass(int i, ShaderPass * pass)
	{
		mPasses.Insert(i, pass);
	}

	void ShaderFX::RemovePass(int index)
	{
		mPasses.Erase(index);

		delete mPasses[index];
	}

	void ShaderFX::Clear()
	{
		for (int i = 0; i < mPasses.Size(); ++i)
		{
			delete mPasses[i];
		}

		mPasses.Clear();
	}

}

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

	FX_Uniform * ShaderPass::GetUniform(const FixedString32 & name)
	{
		for (int i = 0; i < mUniforms.Size(); ++i)
		{
			FX_Uniform * uniform = mUniforms[i];

			if (uniform->Name == name)
				return uniform;
		}

		return false;
	}

	void ShaderPass::SetConst(int index, float x, float y, float z, float w)
	{
		SetConst(index, Float4(x, y, z, w));
	}

	bool ShaderPass::SetConst(const FixedString32 & name, float x, float y, float z, float w)
	{
		return SetConst(name, Float4(x, y, z, w));
	}

	void ShaderPass::SetConst(int index, const Float4 * data, int count)
	{
		mUniforms[index]->SetConst(data, count);
	}

	bool ShaderPass::SetConst(const FixedString32 & name, const Float4 * data, int count)
	{
		FX_Uniform * u = GetUniform(name);
		if (u != NULL)
		{
			u->SetConst(data, count);
			return true;
		}

		return false;
	}

	void ShaderPass::SetConst(int index, const Float4 & data)
	{
		mUniforms[index]->SetConst(data);
	}

	bool ShaderPass::SetConst(const FixedString32 & name, const Float4 & data)
	{
		FX_Uniform * u = GetUniform(name);
		if (u != NULL)
		{
			u->SetConst(data);
			return true;
		}

		return false;
	}

	void ShaderPass::SetConst(int index, const Mat4 & data)
	{
		mUniforms[index]->SetConst(data);
	}

	bool ShaderPass::SetConst(const FixedString32 & name, const Mat4 & data)
	{
		FX_Uniform * u = GetUniform(name);
		if (u != NULL)
		{
			u->SetConst(data);
			return true;
		}

		return false;
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

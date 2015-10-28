#include "MRenderObject.h"
#include "MRenderHelper.h"
#include "MNode.h"

namespace Rad {

	//
	Material::Material()
	{
		fillMode = eFillMode::SOLID;
		cullMode = eCullMode::BACK;
		depthMode = eDepthMode::LESS_EQUAL;
		blendMode = eBlendMode::OPACITY;

		emissive = Float3(0, 0, 0);
		ambient = Float3(1, 1, 1);
		diffuse = Float3(1, 1, 1);
		specular = Float3(1, 1, 1);
		shininess = 40;

		maps[eMapType::DIFFUSE] = RenderHelper::Instance()->GetWhiteTexture();
	}

	//
	RenderOp::RenderOp()
		: primType(ePrimType::TRIANGLE_LIST)
		, primCount(0)
	{
	}

	//
	RenderObject::RenderObject()
		: mShaderFX(NULL)
		, mCurrentShaderFX(NULL)
		, mSortSpecial(false)
	{
		for (int i = 0; i < eLightType::MAX; ++i)
			mLightingShaderFX[i] = NULL;

		for (int i = 0; i < eRenderCallBack::MAX; ++i)
			mRenderCallBack[i] = NULL;
	}

	RenderObject::~RenderObject()
	{
	}

	void RenderObject::SetShaderFX(ShaderFX * fx)
	{
		mShaderFX = fx;
	}

	void RenderObject::SetLightShaderFX(eLightType type, ShaderFX * fx)
	{
		mLightingShaderFX[type] = fx;
	}

	void RenderObject::SetCurrentShaderFX(ShaderFX * fx)
	{
		mCurrentShaderFX = fx;
	}

	void RenderObject::SetShaderProvider(ShaderProviderPtr provider)
	{
		mShaderProvider = provider;
	}

	void RenderObject::SetRenderCallBack(eRenderCallBack i, RenderCallBack * callback)
	{
		mRenderCallBack[i] = callback;
	}

	RenderCallBack * RenderObject::GetRenderCallBack(int i)
	{
		return mRenderCallBack[i];
	}

	float RenderObject::_getOpacity()
	{
		Node * n = _getNode();

		return n != NULL ? n->GetWorldOpacity() : 1.0f;
	}

	bool RenderObject::IsSortSpecial()
	{
		return mSortSpecial;
	}
}
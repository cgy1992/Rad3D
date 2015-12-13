#include "MRenderRegister.h"

namespace Rad {

	ImplementSingleton(RenderRegister);

	RenderRegister::RenderRegister()
		: mNeedUpdateTransfrom(false)
	{
		mWorldTM = Mat4::Identity;

		mViewTM = Mat4::Identity;
		mProjTM = Mat4::Identity;
		mWorldViewTM = Mat4::Identity;
		mViewProjTM = Mat4::Identity;
		mWorldViewProjTM = Mat4::Identity;

		mBoneCount = 0;

		SetFogParam(5000, 20000);
		SetFogColor(Float3(1, 1, 1));

		mTime = Float4::Zero;

		mClipPlane = Float4(1, 10000, 1, 0.0001f);

		memset(mUserConst, 0, MAX_HW_USER_CONST * sizeof(Float4));
	}

	RenderRegister::~RenderRegister()
	{
	}

	void RenderRegister::Begin()
	{
		if (mNeedUpdateTransfrom)
		{
			mWorldViewTM = mWorldTM * mViewTM;
			mViewProjTM = mViewTM * mProjTM;
			mWorldViewProjTM = mWorldViewTM * mProjTM;

			mNeedUpdateTransfrom = FALSE;
		}

		mAmbient = mMaterialAmbient * mLightAmbient;
		mDiffuse = mMaterialDiffuse * mLightDiffuse;
		mSpecular = mMaterialSpecular * mLightSpecular;
		mSpecular.a = mMaterialSpecular.a;
	}

	void RenderRegister::End()
	{
	}

}

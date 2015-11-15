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

		mFogColor = Float4(1, 1, 1, 1);
		mFogParam = Float4(5000, 10000, 0.001f, 1);

		mTime = Float4::Zero;

		mClipPlane = Float4(1, 10000, 1, 0.0001f);

		memset(mUserConst, 0, MAX_HW_USER_CONST * sizeof(Float4));
	}

	RenderRegister::~RenderRegister()
	{
	}

	void RenderRegister::Reset()
	{
		// transform
		mWorldTM = Mat4::Identity;

		mNeedUpdateTransfrom = TRUE;
	}

	void RenderRegister::Begin()
	{
		if (mNeedUpdateTransfrom)
		{
			mWorldViewTM = mWorldTM * mViewTM;
			mViewProjTM = mViewTM * mProjTM;
			mWorldViewProjTM = mWorldViewTM * mProjTM;
		}

		mAmbient = mMaterialAmbient * mLightAmbient;
		mDiffuse = mMaterialDiffuse * mLightDiffuse;
		mSpecular = mMaterialSpecular * mLightSpecular;
		mSpecular.a = mMaterialSpecular.a;

		mNeedUpdateTransfrom = FALSE;
	}

	void RenderRegister::End()
	{
	}

	void RenderRegister::SetWorldTM(const Mat4 & m)
	{
		mWorldTM = m;
		mNeedUpdateTransfrom = TRUE;
	}

	void RenderRegister::SetViewTM(const Mat4 & m)
	{
		mViewTM = m;
		mNeedUpdateTransfrom = TRUE;
	}

	void RenderRegister::SetProjTM(const Mat4 & m)
	{
		mProjTM = m;
		mNeedUpdateTransfrom = TRUE;
	}

	const Mat4 & RenderRegister::GetWorldTM()
	{
		return mWorldTM;
	}

	const Mat4 & RenderRegister::GetViewTM()
	{
		return mViewTM;
	}

	const Mat4 & RenderRegister::GetProjTM()
	{
		return mProjTM;
	}

	const Mat4 & RenderRegister::GetWorldViewTM()
	{
		return mWorldViewTM;
	}

	const Mat4 & RenderRegister::GetViewProjTM()
	{
		return mViewProjTM;
	}

	const Mat4 & RenderRegister::GetWorldViewProjTM()
	{
		return mWorldViewProjTM;
	}

	void RenderRegister::SetBoneTM(const Mat4 * pBoneTM, int count)
	{
		d_assert (count <= MAX_HW_BONE);

		for (int i = 0; i < count; ++i)
		{
			const Mat4 & m = pBoneTM[i];
			int index = i * 3;

			mBoneTM[index + 0][0] = m[0][0];
			mBoneTM[index + 0][1] = m[1][0];
			mBoneTM[index + 0][2] = m[2][0];
			mBoneTM[index + 0][3] = m[3][0];

			mBoneTM[index + 1][0] = m[0][1];
			mBoneTM[index + 1][1] = m[1][1];
			mBoneTM[index + 1][2] = m[2][1];
			mBoneTM[index + 1][3] = m[3][1];

			mBoneTM[index + 2][0] = m[0][2];
			mBoneTM[index + 2][1] = m[1][2];
			mBoneTM[index + 2][2] = m[2][2];
			mBoneTM[index + 2][3] = m[3][2];
		}

		mBoneCount = count;
	}

	int	RenderRegister::GetBoneCount()
	{
		return mBoneCount;
	}

	const Float4 * RenderRegister::GetBoneTM()
	{
		return mBoneTM;
	}

	//
	void RenderRegister::SetCameraPosition(const Float3 & position)
	{
		mCameraPosition.x = position.x;
		mCameraPosition.y = position.y;
		mCameraPosition.z = position.z;
		mCameraPosition.w = 1;
	}

	void RenderRegister::SetCameraRightVector(const Float3 & right)
	{
		mCameraRightVector.x = right.x;
		mCameraRightVector.y = right.y;
		mCameraRightVector.z = right.z;
		mCameraRightVector.w = 1;
	}

	void RenderRegister::SetCameraUpVector(const Float3 & up)
	{
		mCameraUpVector.x = up.x;
		mCameraUpVector.y = up.y;
		mCameraUpVector.z = up.z;
		mCameraUpVector.w = 1;
	}

	void RenderRegister::SetCameraDirVector(const Float3 & dir)
	{
		mCameraDirVector.x = dir.x;
		mCameraDirVector.y = dir.y;
		mCameraDirVector.z = dir.z;
		mCameraDirVector.w = 1;
	}

	const Float4 & RenderRegister::GetCameraPosition()
	{
		return mCameraPosition;
	}

	const Float4 & RenderRegister::GetCameraRightVector()
	{
		return mCameraRightVector;
	}

	const Float4 & RenderRegister::GetCameraUpVector()
	{
		return mCameraUpVector;
	}

	const Float4 & RenderRegister::GetCameraDirVector()
	{
		return mCameraDirVector;
	}

	//
	void RenderRegister::SetMaterial(
		const Float3 & emissive, 
		const Float3 & ambient,
		const Float3 & diffuse,
		const Float3 & specular,
		float shininess,
		float opacity)
	{
		mMaterialEmissive = Float4(emissive.r, emissive.g, emissive.b, 1);
		mMaterialAmbient = Float4(ambient.r, ambient.g, ambient.b, 1);
		mMaterialDiffuse = Float4(diffuse.r, diffuse.g, diffuse.b, 1);
		mMaterialSpecular = Float4(specular.r, specular.g, specular.b, shininess);
		mMaterialOpacity = Float4(opacity, opacity, opacity, opacity);
	}

	// Light
	void RenderRegister::SetLightPosition(const Float3 & position)
	{
		mLightPosition.x = position.x;
		mLightPosition.y = position.y;
		mLightPosition.z = position.z;
		mLightPosition.w = 1;
	}

	void RenderRegister::SetLightDirection(const Float3 & direction)
	{
		mLightDirection.x = -direction.x;
		mLightDirection.y = -direction.y;
		mLightDirection.z = -direction.z;
		mLightDirection.w = 1;
	}

	void RenderRegister::SetLightColor(const Float3 & ambeint, const Float3 & diffuse, const Float3 & specular)
	{
		mLightAmbient = Float4(ambeint.r, ambeint.g, ambeint.b, 1);
		mLightDiffuse = Float4(diffuse.r, diffuse.g, diffuse.b, 1);
		mLightSpecular = Float4(specular.r, specular.g, specular.b, 1);
	}
	
	void RenderRegister::SetLightAttenParam(const Float4 & param)
	{
		mLightAttenParam = param;
	}

	void RenderRegister::SetLightSpotParam(const Float4 & param)
	{
		mLightSpotParam = param;
	}

	const Float4 & RenderRegister::GetLightPosition()
	{
		return mLightPosition;
	}

	const Float4 & RenderRegister::GetLightDirection()
	{
		return mLightDirection;
	}

	const Float4 & RenderRegister::GetLightAttenParam()
	{
		return mLightAttenParam;
	}

	const Float4 & RenderRegister::GetLightSpotParam()
	{
		return mLightSpotParam;
	}

	const Float4 & RenderRegister::GetEmissive()
	{
		return mMaterialEmissive;
	}

	const Float4 & RenderRegister::GetAmbient()
	{
		return mAmbient;
	}

	const Float4 & RenderRegister::GetDiffuse()
	{
		return mDiffuse;
	}

	const Float4 & RenderRegister::GetSpecular()
	{
		return mSpecular;
	}

	const Float4 & RenderRegister::GetOpacity()
	{
		return mMaterialOpacity;
	}

	// Fog
	void RenderRegister::SetFogColor(const Float3 & color)
	{
		mFogColor = Float4(color.r, color.g, color.b, 1);
	}

	void RenderRegister::SetFogParam(float start, float end)
	{
		mFogParam.x = start;
		mFogParam.y = end;
		mFogParam.z = 1 / (end - start);
		mFogParam.w = 1;
	}

	const Float4 & RenderRegister::GetFogColor()
	{
		return mFogColor;
	}
	
	const Float4 & RenderRegister::GetFogParam()
	{
		return mFogParam;
	}

	// ShaderState
	void RenderRegister::SetShaderState(float alphaTest, float reserved1, float reserved2, float reserved3)
	{
		mShaderState.x = alphaTest;
		mShaderState.y = 0;
		mShaderState.z = 0;
		mShaderState.w = 0;
	}

	const Float4 & RenderRegister::GetShaderState()
	{
		return mShaderState;
	}

	//Time
	void RenderRegister::SetTime(float time)
	{
		mTime.x = time;
		Math::SinCos(time, mTime.y, mTime.z);
		mTime.w = 1;
	}

	const Float4 & RenderRegister::GetTime()
	{
		return mTime;
	}

	void RenderRegister::SetClipPlane(float nearClip, float farClip)
	{
		mClipPlane.x = nearClip; mClipPlane.y = farClip;
		mClipPlane.z = 1 / (farClip - nearClip); mClipPlane.w = 1 / farClip;
	}

	const Float4 & RenderRegister::GetClipPlane()
	{
		return mClipPlane;
	}

	void RenderRegister::SetUserConst(int index, const Float4 & v)
	{
		mUserConst[index] = v;
	}

	void RenderRegister::SetUserConst(int index, const Float4 * v, int count)
	{
		d_assert (index + count < MAX_HW_USER_CONST);

		memcpy(&mUserConst[index], v, count * sizeof(Float4));
	}

	const Float4 * RenderRegister::GetUserConst(int index)
	{
		return &mUserConst[index];
	}

}

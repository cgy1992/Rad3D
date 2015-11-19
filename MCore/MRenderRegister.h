/*
*	RenderRegister
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"
#include "MSingleton.h"

namespace Rad {

	class M_ENTRY RenderRegister : public Singleton<RenderRegister>
	{
	public:
		RenderRegister();
		~RenderRegister();

		void            
			Begin();
		void            
			End();

		// Transform
		void            
			SetWorldTM(const Mat4 & m);
		void            
			SetViewTM(const Mat4 & m);
		void            
			SetProjTM(const Mat4 & m);

		const Mat4 &    
			GetWorldTM();
		const Mat4 &    
			GetViewTM();
		const Mat4 &    
			GetProjTM();
		const Mat4 &    
			GetWorldViewTM();
		const Mat4 &    
			GetViewProjTM();
		const Mat4 &    
			GetWorldViewProjTM();

		// Bone
		void			
			SetBoneTM(const Mat4 * pBoneTM, int count);
		int				
			GetBoneCount();
		const Float4 *	
			GetBoneTM();

		// Camera
		void			
			SetCameraPosition(const Float3 & position);
		void
			SetCameraRightVector(const Float3 & right);
		void
			SetCameraUpVector(const Float3 & up);
		void			
			SetCameraDirVector(const Float3 & dir);

		const Float4 &  
			GetCameraPosition();
		const Float4 &
			GetCameraRightVector();
		const Float4 &
			GetCameraUpVector();
		const Float4 &	
			GetCameraDirVector();


		// Material
		void			
			SetMaterial(const Float3 & emissive, 
						const Float3 & ambient,
						const Float3 & diffuse,
						const Float3 & specular,
						float shininess, float opacity);

		// Light
		void			
			SetLightPosition(const Float3 & position);
		void			
			SetLightDirection(const Float3 & direction);
		void			
			SetLightColor(const Float3 & ambeint, const Float3 & diffuse, const Float3 & specular);
		void			
			SetLightAttenParam(const Float4 & param);
		void			
			SetLightSpotParam(const Float4 & param);

		const Float4 &  
			GetLightPosition();
		const Float4 &  
			GetLightDirection();
		const Float4 &  
			GetLightAttenParam();
		const Float4 &  
			GetLightSpotParam();
		const Float4 &
			GetLightAmbient();
		const Float4 &
			GetLightDiffuse();
		const Float4 &
			GetLightSpecular();

		const Float4 &  
			GetMaterialEmissive();
		const Float4 &  
			GetMaterialAmbient();
		const Float4 &  
			GetMaterialDiffuse();
		const Float4 &  
			GetMaterialSpecular();

		const Float4 &
			GetEmissive();
		const Float4 &	
			GetAmbient();
		const Float4 &	
			GetDiffuse();
		const Float4 &	
			GetSpecular();
		const Float4 &  
			GetOpacity();

		// Fog
		void			
			SetFogColor(const Float3 & color);
		void			
			SetFogParam(float start, float end);
		const Float4 &	
			GetFogColor();
		const Float4 &  
			GetFogParam();

		// ShaderState
		void			
			SetShaderState(float alphaTest, float reserved1, float reserved2, float reserved3);
		const Float4 &	
			GetShaderState();

		//Time
		void            
			SetTime(float time);
		const Float4 &  
			GetTime();

		// clip plane
		void            
			SetClipPlane(float nearClip, float farClip);
		const Float4 &  
			GetClipPlane();

		// User Const
		void			
			SetUserConst(int index, const Float4 & v);
		void			
			SetUserConst(int index, const Float4 * v, int count);
		const Float4 *	
			GetUserConst(int index);

	protected:
		Mat4            mWorldTM;
		Mat4            mViewTM;
		Mat4            mProjTM;
		Mat4            mWorldViewTM;
		Mat4            mViewProjTM;
		Mat4            mWorldViewProjTM;

		bool            mNeedUpdateTransfrom;

		int             mBoneCount;
		Float4			mBoneTM[MAX_HW_BONE * 3];

		Float4			mCameraPosition;
		Float4			mCameraRightVector;
		Float4			mCameraUpVector;
		Float4			mCameraDirVector;

		Float4			mMaterialEmissive;
		Float4			mMaterialAmbient;
		Float4			mMaterialDiffuse;
		Float4			mMaterialSpecular;
		Float4			mMaterialOpacity;

		Float4			mLightPosition;
		Float4			mLightDirection;
		Float4			mLightAmbient;
		Float4			mLightDiffuse;
		Float4			mLightSpecular;
		Float4			mLightAttenParam;
		Float4			mLightSpotParam;

		Float4			mAmbient;
		Float4			mDiffuse;
		Float4			mSpecular;

		Float4			mFogColor;
		Float4			mFogParam;

		Float4			mShaderState;

		Float4          mTime;
		Float4          mClipPlane;

		Float4			mUserConst[MAX_HW_USER_CONST];
	};

	//
	inline void RenderRegister::SetWorldTM(const Mat4 & m)
	{
		mWorldTM = m;
		mNeedUpdateTransfrom = TRUE;
	}

	inline void RenderRegister::SetViewTM(const Mat4 & m)
	{
		mViewTM = m;
		mNeedUpdateTransfrom = TRUE;
	}

	inline void RenderRegister::SetProjTM(const Mat4 & m)
	{
		mProjTM = m;
		mNeedUpdateTransfrom = TRUE;
	}

	inline const Mat4 & RenderRegister::GetWorldTM()
	{
		return mWorldTM;
	}

	inline const Mat4 & RenderRegister::GetViewTM()
	{
		return mViewTM;
	}

	inline const Mat4 & RenderRegister::GetProjTM()
	{
		return mProjTM;
	}

	inline const Mat4 & RenderRegister::GetWorldViewTM()
	{
		return mWorldViewTM;
	}

	inline const Mat4 & RenderRegister::GetViewProjTM()
	{
		return mViewProjTM;
	}

	inline const Mat4 & RenderRegister::GetWorldViewProjTM()
	{
		return mWorldViewProjTM;
	}

	inline void RenderRegister::SetBoneTM(const Mat4 * pBoneTM, int count)
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

	inline int	RenderRegister::GetBoneCount()
	{
		return mBoneCount;
	}

	inline const Float4 * RenderRegister::GetBoneTM()
	{
		return mBoneTM;
	}

	//
	inline void RenderRegister::SetCameraPosition(const Float3 & position)
	{
		mCameraPosition.x = position.x;
		mCameraPosition.y = position.y;
		mCameraPosition.z = position.z;
		mCameraPosition.w = 1;
	}

	inline void RenderRegister::SetCameraRightVector(const Float3 & right)
	{
		mCameraRightVector.x = right.x;
		mCameraRightVector.y = right.y;
		mCameraRightVector.z = right.z;
		mCameraRightVector.w = 1;
	}

	inline void RenderRegister::SetCameraUpVector(const Float3 & up)
	{
		mCameraUpVector.x = up.x;
		mCameraUpVector.y = up.y;
		mCameraUpVector.z = up.z;
		mCameraUpVector.w = 1;
	}

	inline void RenderRegister::SetCameraDirVector(const Float3 & dir)
	{
		mCameraDirVector.x = dir.x;
		mCameraDirVector.y = dir.y;
		mCameraDirVector.z = dir.z;
		mCameraDirVector.w = 1;
	}

	inline const Float4 & RenderRegister::GetCameraPosition()
	{
		return mCameraPosition;
	}

	inline const Float4 & RenderRegister::GetCameraRightVector()
	{
		return mCameraRightVector;
	}

	inline const Float4 & RenderRegister::GetCameraUpVector()
	{
		return mCameraUpVector;
	}

	inline const Float4 & RenderRegister::GetCameraDirVector()
	{
		return mCameraDirVector;
	}

	//
	inline void RenderRegister::SetMaterial(
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
	inline void RenderRegister::SetLightPosition(const Float3 & position)
	{
		mLightPosition.x = position.x;
		mLightPosition.y = position.y;
		mLightPosition.z = position.z;
		mLightPosition.w = 1;
	}

	inline void RenderRegister::SetLightDirection(const Float3 & direction)
	{
		mLightDirection.x = -direction.x;
		mLightDirection.y = -direction.y;
		mLightDirection.z = -direction.z;
		mLightDirection.w = 1;
	}

	inline void RenderRegister::SetLightColor(const Float3 & ambeint, const Float3 & diffuse, const Float3 & specular)
	{
		mLightAmbient = Float4(ambeint.r, ambeint.g, ambeint.b, 1);
		mLightDiffuse = Float4(diffuse.r, diffuse.g, diffuse.b, 1);
		mLightSpecular = Float4(specular.r, specular.g, specular.b, 1);
	}

	inline void RenderRegister::SetLightAttenParam(const Float4 & param)
	{
		mLightAttenParam = param;
	}

	inline void RenderRegister::SetLightSpotParam(const Float4 & param)
	{
		mLightSpotParam = param;
	}

	inline const Float4 & RenderRegister::GetLightPosition()
	{
		return mLightPosition;
	}

	inline const Float4 & RenderRegister::GetLightDirection()
	{
		return mLightDirection;
	}

	inline const Float4 & RenderRegister::GetLightAttenParam()
	{
		return mLightAttenParam;
	}

	inline const Float4 & RenderRegister::GetLightSpotParam()
	{
		return mLightSpotParam;
	}

	inline const Float4 & RenderRegister::GetLightAmbient()
	{
		return mLightAmbient;
	}

	inline const Float4 & RenderRegister::GetLightDiffuse()
	{
		return mLightDiffuse;
	}

	inline const Float4 & RenderRegister::GetLightSpecular()
	{
		return mLightSpecular;
	}

	inline const Float4 & RenderRegister::GetMaterialEmissive()
	{
		return mMaterialEmissive;
	}

	inline const Float4 & RenderRegister::GetMaterialAmbient()
	{
		return mMaterialAmbient;
	}

	inline const Float4 & RenderRegister::GetMaterialDiffuse()
	{
		return mMaterialDiffuse;
	}

	inline const Float4 & RenderRegister::GetMaterialSpecular()
	{
		return mMaterialSpecular;
	}

	inline const Float4 & RenderRegister::GetEmissive()
	{
		return mMaterialEmissive;
	}

	inline const Float4 & RenderRegister::GetAmbient()
	{
		return mAmbient;
	}

	inline const Float4 & RenderRegister::GetDiffuse()
	{
		return mDiffuse;
	}

	inline const Float4 & RenderRegister::GetSpecular()
	{
		return mSpecular;
	}

	inline const Float4 & RenderRegister::GetOpacity()
	{
		return mMaterialOpacity;
	}

	// Fog
	inline void RenderRegister::SetFogColor(const Float3 & color)
	{
		mFogColor = Float4(color.r, color.g, color.b, 1);
	}

	inline void RenderRegister::SetFogParam(float start, float end)
	{
		mFogParam.x = start;
		mFogParam.y = end;
		mFogParam.z = 1 / (end - start);
		mFogParam.w = 1;
	}

	inline const Float4 & RenderRegister::GetFogColor()
	{
		return mFogColor;
	}

	inline const Float4 & RenderRegister::GetFogParam()
	{
		return mFogParam;
	}

	// ShaderState
	inline void RenderRegister::SetShaderState(float alphaTest, float reserved1, float reserved2, float reserved3)
	{
		mShaderState.x = alphaTest;
		mShaderState.y = 0;
		mShaderState.z = 0;
		mShaderState.w = 0;
	}

	inline const Float4 & RenderRegister::GetShaderState()
	{
		return mShaderState;
	}

	//Time
	inline void RenderRegister::SetTime(float time)
	{
		mTime.x = time;
		Math::SinCos(time, mTime.y, mTime.z);
		mTime.w = 1;
	}

	inline const Float4 & RenderRegister::GetTime()
	{
		return mTime;
	}

	inline void RenderRegister::SetClipPlane(float nearClip, float farClip)
	{
		mClipPlane.x = nearClip; mClipPlane.y = farClip;
		mClipPlane.z = 1 / (farClip - nearClip); mClipPlane.w = 1 / farClip;
	}

	inline const Float4 & RenderRegister::GetClipPlane()
	{
		return mClipPlane;
	}

	inline void RenderRegister::SetUserConst(int index, const Float4 & v)
	{
		mUserConst[index] = v;
	}

	inline void RenderRegister::SetUserConst(int index, const Float4 * v, int count)
	{
		d_assert (index + count < MAX_HW_USER_CONST);

		memcpy(&mUserConst[index], v, count * sizeof(Float4));
	}

	inline const Float4 * RenderRegister::GetUserConst(int index)
	{
		return &mUserConst[index];
	}

}

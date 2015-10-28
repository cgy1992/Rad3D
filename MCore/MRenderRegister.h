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
			Reset();

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

}

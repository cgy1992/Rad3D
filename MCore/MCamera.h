/*
*	Camera
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMat4.h"
#include "MSphere.h"
#include "MAabb.h"
#include "MRay.h"
#include "MPlane.h"
#include "MNode.h"

namespace Rad {

	struct Frustum
	{
		Plane Near;
		Plane Far;
		Plane Left;
		Plane Right;
		Plane Top;
		Plane Bottom;

		void FromMatrix(const Mat4 & matViewProj)
		{
			Frustum & frustum = *this;
			const Mat4 & mat = matViewProj;

			frustum.Left.normal.x = mat._14 + mat._11;
			frustum.Left.normal.y = mat._24 + mat._21;
			frustum.Left.normal.z = mat._34 + mat._31;
			frustum.Left.d = mat._44 + mat._41;

			frustum.Right.normal.x = mat._14 - mat._11;
			frustum.Right.normal.y = mat._24 - mat._21;
			frustum.Right.normal.z = mat._34 - mat._31;
			frustum.Right.d = mat._44 - mat._41;

			frustum.Top.normal.x = mat._14 - mat._12;
			frustum.Top.normal.y = mat._24 - mat._22;
			frustum.Top.normal.z = mat._34 - mat._32;
			frustum.Top.d = mat._44 - mat._42;

			frustum.Bottom.normal.x = mat._14 + mat._12;
			frustum.Bottom.normal.y = mat._24 + mat._22;
			frustum.Bottom.normal.z = mat._34 + mat._32;
			frustum.Bottom.d = mat._44 + mat._42;

			frustum.Near.normal.x = mat._13;
			frustum.Near.normal.y = mat._23;
			frustum.Near.normal.z = mat._33;
			frustum.Near.d = mat._43;

			frustum.Far.normal.x = mat._14 - mat._13;
			frustum.Far.normal.y = mat._24 - mat._23;
			frustum.Far.normal.z = mat._34 - mat._33;
			frustum.Far.d = mat._44 - mat._43;

			frustum.Left.Normalize();
			frustum.Right.Normalize();
			frustum.Top.Normalize();
			frustum.Bottom.Normalize();
			frustum.Near.Normalize();
		}
	};

	//
	class M_ENTRY Camera : public Node
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(Node);

	public:
		enum Visibility {
			VB_NONE,
			VB_FULL,
			VB_PARTIAL,
		};

	public:
		Camera();
		virtual ~Camera();

		void
			LookAt(const Float3 & _at);

		void            
			SetFov(float _fovy);
		float           
			GetFov() const { return mFovy; }

		void            
			SetClipPlane(float _near, float _far);
		float           
			GetNearClip() const { return mNear; }
		float           
			GetFarClip() const { return mFar; }

		void            
			SetAspect(float _asecpt);
		float           
			GetAspect() const { return mAspect; }
		
		void            
			SetOrthoParam(float width, float height, bool enable);
		bool
			GetOrthoEnable() const { return mOrthoEnable; }
		float           
			GetOrthoWidth() const { return mOrthoWidth; }
		float           
			GetOrthoHeight() const { return mOrthoHeight; }
		
		void
			SetMirrorPlane(const Plane & plane);
		const Plane &
			GetMirrorPlane() const { return mMirrorPlane; }
		bool
			IsMirrorEnable() const { return mMirrorEnable; }

		const Mat4 &    
			GetViewMatrix();
		const Mat4 &    
			GetProjMatrix();
		const Mat4 &    
			GetViewProjMatrix();
		
		const Frustum &
			GetFrustum();
		const Float3 *
			GetViewCorner();
		const Float3 *
			GetWorldCorner();
		void 
			GetWorldCorner(Float3 * corner, float nearClip, float farClip);

		Visibility     
			GetVisibility(const Float3 & point);
		Visibility      
			GetVisibility(const Aabb & box);
		Visibility		
			GetVisibility(const Sphere & sph);

		bool            
			InVisible(const Aabb & box);
		bool            
			InVisible(const Sphere & sph);

		Ray             
			GetViewportRay(float x, float y);

	protected:
		void            
			_updateTM();
		void 
			_makeClipProjMatrix();

	protected:
		float mFovy;
		float mNear;
		float mFar;
		float mAspect;

		bool mOrthoEnable;
		float mOrthoWidth;
		float mOrthoHeight;

		Plane mMirrorPlane;
		bool mMirrorEnable;

		Mat4 mMatView;
		Mat4 mMatProj;
		Mat4 mMatViewProj;

		Frustum mFrustum;
		Float3 mViewCorner[8];
		Float3 mWorldCorner[8];
	};

}
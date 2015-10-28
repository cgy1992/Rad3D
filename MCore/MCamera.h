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
		Plane		Near;
		Plane       Far;
		Plane       Left;
		Plane       Right;
		Plane       Top;
		Plane       Bottom;
	};

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

		const Mat4 &    
			GetViewMatrix();
		const Mat4 &    
			GetProjMatrix();
		const Mat4 &    
			GetViewProjMatrix();
		
		const Frustum &
			GetFrustum();
		const Float3 *
			GetCorner();
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
			_updateFrustum();

	protected:
		float mFovy;
		float mNear;
		float mFar;
		float mAspect;

		bool mOrthoEnable;
		float mOrthoWidth;
		float mOrthoHeight;

		Mat4 mMatView;
		Mat4 mMatProj;
		Mat4 mMatViewProj;

		Frustum mFrustum;
		Float3 mCorner[8];
		Float3 mWorldCorner[8];
	};

}
#include "MCamera.h"
#include "MNode.h"

namespace Rad {

	DF_PROPERTY_BEGIN(Camera)
		DF_PROPERTY(Camera, mFovy, "", "Fovy", PT_Float)
		DF_PROPERTY(Camera, mNear, "", "Near", PT_Float)
		DF_PROPERTY(Camera, mFar, "", "Far", PT_Float)
	DF_PROPERTY_END()

	//
	ImplementRTTI(Camera, Node);

	Camera::Camera()
		: Node("Camera")
		, mFovy(PI / 3)
		, mNear(1.0f)
		, mFar(1000.0f * METER_LEN)
		, mAspect(1.3333333f)
		, mOrthoEnable(false)
		, mOrthoWidth(300.0f)
		, mOrthoHeight(200.0f)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::LookAt(const Float3 & _at)
	{
		SetDirection(_at - GetPosition());
	}

	void Camera::SetFov(float _fovy)
	{
		if (mFovy != _fovy)
		{
			mFovy = _fovy;

			ChangeTm(eTmFlags::UNKNOWN);
		}
	}

	void Camera::SetClipPlane(float _near, float _far)
	{
		d_assert (_near < _far);

		if (mNear != _near || mFar != _far)
		{
			mNear = _near;
			mFar = _far;

			ChangeTm(eTmFlags::UNKNOWN);
		}
	}

	void Camera::SetAspect(float _asecpt)
	{
		if (mAspect != _asecpt)
		{
			mAspect = _asecpt;

			ChangeTm(eTmFlags::UNKNOWN);
		}
	}

	void Camera::SetOrthoParam(float width, float height, bool enable)
	{
		mOrthoWidth = width;
		mOrthoHeight = height;
		mOrthoEnable = enable;

		ChangeTm(eTmFlags::UNKNOWN);
	}

	const Mat4 & Camera::GetViewTM()
	{
		_updateTM();
		return mMatView;
	}

	const Mat4 & Camera::GetProjTM()
	{
		_updateTM();
		return mMatProj;
	}

	const Mat4 & Camera::GetViewProjTM()
	{
		_updateTM();
		return mMatViewProj;
	}

	const Frustum & Camera::GetFrustum()
	{
		_updateTM();
		return mFrustum;
	}

	const Float3 * Camera::GetViewCorner()
	{
		_updateTM();
		return mViewCorner;
	}

	const Float3 * Camera::GetWorldCorner()
	{
		_updateTM();
		return mWorldCorner;
	}

	void Camera::GetWorldCorner(Float3 * corner, float nearClip, float farClip)
	{
		Float3 nearPos = Float3(0, 0, nearClip);
		Float3 farPos = Float3(0, 0, farClip);

		nearPos *= GetProjTM();
		farPos *= GetProjTM();

		Float3 t_corner[8] = { 
			Float3(-1, 1, 0), Float3(1, 1, 0), Float3(-1, -1, 0), Float3(1, -1, 0), 
			Float3(-1, 1, 1), Float3(1, 1, 1), Float3(-1, -1, 1), Float3(1, -1, 1)
		};

		for (int i = 0; i < 4; ++i)
		{
			t_corner[i].z = nearPos.z;
			t_corner[i + 4].z = farPos.z;
		}

		Mat4 matInverseVP = GetViewProjTM();
		matInverseVP.Inverse();

		for (int i = 0; i < 8; ++i)
		{
			corner[i] = t_corner[i] * matInverseVP;
		}
	}

	void Camera::_updateTM()
	{
		if (mTmChangeFlags == 0)
			return ;

		Node::_updateTM();

		const Float3 & pos = GetWorldPosition();
		const Quat & ort = GetWorldRotation();

		mMatView.MakeViewLH(pos, ort);

		if (GetOrthoEnable())
		{
			mMatProj.MakeOrthoLH(mOrthoWidth, mOrthoHeight, mNear, mFar);
		}
		else
		{
			mMatProj.MakePerspectiveLH(mFovy, mAspect, mNear, mFar);
		}

		mMatViewProj = mMatView * mMatProj;

		mFrustum.FromMatrix(mMatViewProj);

		// update corner
		Float3 corner[8] = { 
			Float3(-1, 1, 0), Float3(1, 1, 0), Float3(-1, -1, 0), Float3(1, -1, 0), 
			Float3(-1, 1, 1), Float3(1, 1, 1), Float3(-1, -1, 1), Float3(1, -1, 1)
		};

		Mat4 matInvProj = mMatProj, matInvView = mMatView;

		matInvProj.Inverse();
		matInvView.Inverse();

		for (int i = 0; i < 8; ++i)
			mViewCorner[i] = corner[i] * matInvProj;

		for (int i = 0; i < 8; ++i)
			mWorldCorner[i] = mViewCorner[i] * matInvView;
	}

	Camera::Visibility Camera::GetVisibility(const Float3 & point)
	{
		_updateTM();

		const Plane * planes = (const Plane *)&mFrustum;
		for (int i = 0; i < 6; ++i)
		{
			Plane::Side side = planes[i].AtSide(point);

			if (side == Plane::NEGATIVE)
				return VB_NONE;
		}

		return VB_FULL;
	}

	Camera::Visibility Camera::GetVisibility(const Aabb & box)
	{
		_updateTM();

		const Plane * planes = (const Plane *)&mFrustum;
		Float3 center = box.GetCenter();
		Float3 half = box.GetHalfSize();
		bool full = true;

		for (int i = 0; i < 6; ++i)
		{
			Plane::Side side = planes[i].AtSide(center, half);

			if (side == Plane::NEGATIVE)
				return VB_NONE;
			else if (side == Plane::BOTH)
				full = FALSE;
		}

		return full ? VB_FULL : VB_PARTIAL;
	}

	Camera::Visibility Camera::GetVisibility(const Sphere & sph)
	{
		_updateTM();

		const Plane * planes = (const Plane *)&mFrustum;
		bool full = true;

		for (int i = 0; i < 6; ++i)
		{
			Plane::Side side = planes[i].AtSide(sph);

			if (side == Plane::NEGATIVE)
				return VB_NONE;
			else if (side == Plane::BOTH)
				full = FALSE;
		}

		return full ? VB_FULL : VB_PARTIAL;
	}

	bool Camera::InVisible(const Aabb & box)
	{
		Visibility v = GetVisibility(box);

		return v == VB_FULL || v == VB_PARTIAL;
	}

	bool Camera::InVisible(const Sphere & sph)
	{
		Visibility v = GetVisibility(sph);

		return v == VB_FULL || v == VB_PARTIAL;
	}

	Ray Camera::GetViewportRay(float x, float y)
	{
		Ray result;

		x = Math::Clamp(x, 0.0f, 1.0f);
		y = Math::Clamp(y, 0.0f, 1.0f);

		Mat4 matInvViewProj = GetViewProjTM();
		matInvViewProj.Inverse();

		x = x * 2.0f - 1.0f;
		y = y * -2.0f + 1.0f;

		Float3 p1(x, y, 0.0f);
		Float3 p2(x, y, 0.5f);

		p1.Transform(matInvViewProj);
		p2.Transform(matInvViewProj);

		result.orig = p1;
		result.dir = p2 - p1;

		result.dir.Normalize();

		return result;
	}

}

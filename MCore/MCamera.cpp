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
		, mFar(1000.0f * UNIT_METRES)
		, mAspect(1.3333333f)
		, mOrthoEnable(false)
		, mOrthoWidth(300.0f)
		, mOrthoHeight(200.0f)
		, mMirrorPlane(0, 1, 0, 0)
		, mMirrorEnable(false)
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

	void Camera::SetMirrorPlane(const Plane & plane)
	{
		mMirrorPlane = plane;
		mMirrorEnable = true;
	}

	const Mat4 & Camera::GetViewMatrix()
	{
		_updateTM();
		return mMatView;
	}

	const Mat4 & Camera::GetProjMatrix()
	{
		_updateTM();
		return mMatProj;
	}

	const Mat4 & Camera::GetViewProjMatrix()
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

		nearPos *= GetProjMatrix();
		farPos *= GetProjMatrix();

		Float3 t_corner[8] = { 
			Float3(-1, 1, 0), Float3(1, 1, 0), Float3(-1, -1, 0), Float3(1, -1, 0), 
			Float3(-1, 1, 1), Float3(1, 1, 1), Float3(-1, -1, 1), Float3(1, -1, 1)
		};

		for (int i = 0; i < 4; ++i)
		{
			t_corner[i].z = nearPos.z;
			t_corner[i + 4].z = farPos.z;
		}

		Mat4 matInverseVP = GetViewProjMatrix();
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

		if (mMirrorEnable)
			_makeClipProjMatrix();
	}

	Camera::Visibility Camera::GetVisibility(const Float3 & point)
	{
		_updateTM();

		Float3 pt = point;

		pt.Transform(mMatViewProj);

		if (pt.x > -1 && pt.x < 1 &&
			pt.y > -1 && pt.y < 1 &&
			pt.z >  0 && pt.z < 1)
			return VB_FULL;

		return VB_NONE;
	}

	Camera::Visibility Camera::GetVisibility(const Aabb & box)
	{
		_updateTM();

		if (!mMirrorEnable)
		{
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
		else
		{
			Float3 points[8];
			box.GetPoints(points);

			for (int i = 0; i < 8;  ++i)
			{
				points[i].Transform(mMatViewProj);
			}

			int vb_count = 0;
			for (int i = 0; i < 8; ++i)
			{
				const Float3 & pt = points[i];

				if (pt.x > -1 && pt.x < 1 &&
					pt.y > -1 && pt.y < 1 &&
					pt.z > 0 && pt.z < 1)
					++vb_count;
			}

			if (vb_count == 0)
				return VB_NONE;
			else if (vb_count == 8)
				return VB_FULL;
			else
				return VB_PARTIAL;
		}
	}

	Camera::Visibility Camera::GetVisibility(const Sphere & sph)
	{
		_updateTM();

		if (!mMirrorEnable)
		{
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
		else
		{
			Aabb abb;

			abb.minimum = sph.center - Float3(sph.radius, sph.radius, sph.radius);
			abb.maximum = sph.center + Float3(sph.radius, sph.radius, sph.radius);

			return GetVisibility(abb);
		}
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

		Mat4 matInvViewProj = GetViewProjMatrix();
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

	void Camera::_makeClipProjMatrix()
	{
		Plane clip_plane = mMirrorPlane;
		Mat4 matClipProj;
		Mat4 WorldToProjection;
		WorldToProjection = mMatView * mMatProj;

		WorldToProjection.Inverse();
		WorldToProjection.Transpose();

		Float4 clipPlane(clip_plane.normal.x, clip_plane.normal.y, clip_plane.normal.z, clip_plane.d);
		Float4 projClipPlane;
		// transform clip plane into projection space
		projClipPlane = clipPlane * WorldToProjection;

		matClipProj = Mat4::Identity;

		if (projClipPlane.w == 0)  // or less than a really small value
		{
			// plane is perpendicular to the near plane
			return;
		}

		float k_len = projClipPlane.x * projClipPlane.x + projClipPlane.y * projClipPlane.y +
			projClipPlane.z * projClipPlane.z + projClipPlane.w * projClipPlane.w;

		k_len = Math::Sqrt(k_len);
		projClipPlane.x /= k_len; projClipPlane.y /= k_len;
		projClipPlane.z /= k_len; projClipPlane.w /= k_len;

		if (projClipPlane.w > 0)
		{
			projClipPlane = -projClipPlane;
			projClipPlane.w += 1;
		}
		// put projection space clip plane in Z column
		matClipProj[0][2] = projClipPlane.x;
		matClipProj[1][2] = projClipPlane.y;
		matClipProj[2][2] = projClipPlane.z;
		matClipProj[3][2] = projClipPlane.w;

		// multiply into projection matrix
		mMatProj = mMatProj * matClipProj;
	}

}

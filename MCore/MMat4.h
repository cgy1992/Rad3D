/*
*	Mat4
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"
#include "MMat3.h"

namespace Rad {

	class Float3;
	class Float4;
	class Quat;
	class Plane;

	class M_ENTRY Mat4
	{
		DECLARE_ALLOC();

	public:
		static const Mat4 Identity;
		static const Mat4 Zero;

	public:
		union 
		{
			struct 
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
			float m16[16];
		};

	public:
		Mat4();
		~Mat4();

		Mat4(const float * p);
		Mat4(const Mat3 & m);
		Mat4(const Mat4 & m);
		Mat4(float m11, float m12, float m13, float m14, 
			 float m21, float m22, float m23, float m24, 
			 float m31, float m32, float m33, float m34,  
			 float m41, float m42, float m43, float m44);

		Mat4& 
			operator =(const Mat4 & mat);

		Mat4 
			operator -() const;
		Mat4 
			operator +(const Mat4 & mat) const; 
		Mat4 
			operator -(const Mat4 & mat) const; 
		Mat4 
			operator *(const Mat4 & mat) const; 

		Mat4&
			operator +=(const Mat4 & mat);
		Mat4& 
			operator -=(const Mat4 & mat);
		Mat4& 
			operator *=(const Mat4 & mat);

		Mat4 
			operator *(float v) const;
		Mat4 
			operator /(float v) const;

		Mat4& 
			operator *=(float v);
		Mat4& 
			operator /=(float v);

		bool 
			operator ==(const Mat4 & mat) const;
		bool 
			operator !=(const Mat4 & mat) const;

		float* 
			operator [](int index);
		const float* 
			operator [](int index) const;

	public:
		const float * 
			ToFloatPtr() const;

		bool 
			IsAffine() const;

		float 
			Minor(int r0, int r1, int r2, int c0, int c1, int c2) const;
		float 
			Det() const;

		void 
			Transpose();
		void 
			Inverse();
		void 
			InverseAffine();
		void 
			InverseOrtho();
		void 
			EnsureOrtho();
		
		void 
			MakeTransform(const Float3 & trans, const Quat & rotate, const Float3 & scale);
		void 
			MakeTranslate(float x, float y, float z);
		void 
			MakeRotationX(float rads);
		void 
			MakeRotationY(float rads);
		void 
			MakeRotationZ(float rads);
		void 
			MakeRotationAxis(const Float3 & vAxis, float rads);
		void 
			MakeRotationAxis(const Float3 & xAxis, const Float3 & yAxis, const Float3 & zAxis);
		void 
			MakeRotationYawPitchRoll(float yaw, float pitch, float roll);
		void 
			MakeRotationQuaternion(float x, float y, float z, float w);
		void 
			MakeScale(float x, float y, float z);

		void 
			MakeViewLH(const Float3 & vEye, const Float3 & vAt, const Float3 & vUp);
		void 
			MakeViewLH(const Float3 & vEye, const Quat & qOrient);
		void 
			MakeOrthoLH(float w, float h, float zn, float zf);
		void 
			MakePerspectiveLH(float fovy, float aspect,float zn, float zf);

		void
			MakeShadow(const Float4 & l, const Plane & p);
		void
			MakeReflection(const Plane & p);
		bool
			MakeClipProjection(const Plane & clipPlane, const Mat4 & matViewProj);

		void 
			Decompose(Float3 & pos, Quat & orient, Float3 & scale) const;
	};

	inline const float * Mat4::ToFloatPtr() const
	{
		return (const float *)this;
	}

}

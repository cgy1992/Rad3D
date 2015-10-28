/*
*	Quaternion
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat3.h"

namespace Rad {

	class M_ENTRY Quat
	{
		DECLARE_ALLOC();

	public:
		static const Quat Zero;
		static const Quat Identity;

	public:
		union
		{
			struct {
				float x, y, z, w;
			};
			float m[4];
		};

	public:
		Quat(float x, float y, float z, float w);
		Quat(const Float3 & axis, float rads);
		Quat(const Quat & q);
		Quat();
		~Quat();

		Quat & 
			operator =(const Quat & q);
		Quat 
			operator -() const;

		Quat 
			operator +(const Quat & q) const;
		Quat 
			operator -(const Quat & q) const;
		Quat 
			operator *(const Quat & q) const;
		Quat 
			operator *(float v) const;

		Quat & 
			operator +=(const Quat & q);
		Quat & 
			operator -=(const Quat & q);
		Quat & 
			operator *=(const Quat & q);

		bool 
			operator ==(const Quat & q) const;
		bool 
			operator !=(const Quat & q) const;

	public:
		float 
			Dot(const Quat & rk) const;
		float 
			Length() const;
		void 
			Inverse();
		void 
			Normalize();
		void 
			Conjugate();

		void 
			FromAxis(const Float3 & vAxis, float rads);
		void 
			FromAxisFast(const Float3 & vAxis, float rads);
		void 
			FromAxis(const Float3 & xAxis, const Float3 & yAxis, const Float3 & zAxis);
		void 
			FromDir(const Float3 & dir1, const Float3 & dir2, const Float3 & fallbackAxis = Float3::Zero);
		void 
			FromMatrix(const Mat4 & rot);

		void 
			ToAxis(Float3 & axis, float & rads) const;
		Mat4 
			ToMatrix() const;

		Float3 
			GetRightVector() const;
		Float3 
			GetUpVector() const;
		Float3 
			GetDirVector() const;
		void 
			ToAxis(Float3 & xAxis, Float3 & yAxis, Float3 & zAxis) const;

		void 
			FromEulerAngle(const Float3 & angle);
		Float3 
			ToEulerAngle() const;

		static Quat 
			Slerp(const Quat & a, const Quat & b, float t);
		static Quat
			Lerp(const Quat & a, const Quat & b, float t);
	};

}

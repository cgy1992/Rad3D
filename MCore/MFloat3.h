/*
*	Float3
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"

namespace Rad {

	class Mat4;
	class Quat;

	class M_ENTRY Float3
	{
		DECLARE_ALLOC();

	public:
		static const Float3 Zero;

	public:
		union {

			struct {
				float x, y, z;
			};

			struct {
				float r, g, b;
			};

			struct {
				float m[3];
			};
		};
		

	public:
		Float3(float _x, float _y, float _z);
		Float3(const Float3 & rk);
		Float3();
		~Float3();

		Float3 &
			operator =(const Float3 & rk);

		bool 
			operator ==(const Float3 & rk) const;
		bool 
			operator !=(const Float3 & rk) const;
		bool 
			operator > (const Float3 & rk) const;
		bool 
			operator >=(const Float3 & rk) const;
		bool 
			operator < (const Float3 & rk) const;
		bool 
			operator <=(const Float3 & rk) const;

		Float3 
			operator -() const;

		Float3 
			operator + (const Float3 & rk) const;
		Float3 
			operator - (const Float3 & rk) const;
		Float3 
			operator * (const Float3 & rk) const;
		Float3 
			operator / (const Float3 & rk) const;
		Float3 
			operator * (float f) const;
		Float3 
			operator / (float f) const;

		Float3 & 
			operator +=(const Float3 & rk);
		Float3 & 
			operator -=(const Float3 & rk);
		Float3 & 
			operator *=(float rk);
		Float3 & 
			operator /=(float rk);

		Float3 & 
			operator *=(const Float3 & rk);
		Float3 & 
			operator /=(const Float3 & rk);

		Float3 
			operator * (const Mat4 & rk) const;
		Float3 & 
			operator *=(const Mat4 & rk);

		Float3
			operator * (const Quat & rk) const;
		Float3 &
			operator *=(const Quat & rk);

		float & 
			operator[](int index);
		float 
			operator[](int index) const;

	public:
		const float * 
			ToFloatPtr() const;

		float 
			Length() const;
		float 
			LengthSq() const;

		float 
			Normalize();

		float 
			Distance(const Float3 & rk) const;
		float 
			DistanceSq(const Float3 & rk) const;

		float 
			Dot(const Float3 & rk) const;
		
		void 
			Reflect(const Float3 & n);
		void 
			Refract(const Float3 & n, float eta);

		void 
			Transform(const Mat4 & m);
		void 
			TransformA(const Mat4 & m);
		void 
			TransformN(const Mat4 & m);
		void 
			TransformQ(const Quat & q);

		void 
			Saturate();

		Float3
			Cross(const Float3 & rk) const { return Cross(*this, rk); }
		Float3
			Lerp(const Float3 & rk, float t) const { return Lerp(*this, rk, t); }

		static Float3 
			Cross(const Float3 & lk, const Float3 & rk);
		static Float3 
			CrossN(const Float3 & lk, const Float3 & rk);
		static Float3 
			Lerp(const Float3 & lk, const Float3 & rk, float t);

		static Float3 
			Minimum(const Float3 & lk, const Float3 & rk);
		static Float3 
			Maximum(const Float3 & lk, const Float3 & rk);

		static Float3 
			GetNormal(const Float3 & a, const Float3 & b, const Float3 & c);
	};

	inline const float * Float3::ToFloatPtr() const
	{
		return (const float *)this;
	}

	inline Float3 Float3::CrossN(const Float3 & lk, const Float3 & rk)
	{
		Float3 n = Cross(lk, rk);
		
		n.Normalize();

		return n;
	}

	inline Float3 Float3::Lerp(const Float3 & lk, const Float3 & rk, float t)
	{
		return lk + (rk - lk) * t;
	}

	inline Float3 Float3::Minimum(const Float3 & lk, const Float3 & rk)
	{
		Float3 v;

		v.x = Min(lk.x, rk.x);
		v.y = Min(lk.y, rk.y);
		v.z = Min(lk.z, rk.z);

		return v;
	}

	inline Float3 Float3::Maximum(const Float3 & lk, const Float3 & rk)
	{
		Float3 v;

		v.x = Max(lk.x, rk.x);
		v.y = Max(lk.y, rk.y);
		v.z = Max(lk.z, rk.z);

		return v;
	}

	inline Float3 Float3::GetNormal(const Float3 & a, const Float3 & b, const Float3 & c)
	{
		Float3 ab = b - a;
		Float3 ac = c - a;

		Float3 n = Float3::Cross(ab, ac);
		n.Normalize();

		return n;
	}

	inline Float3 operator * (float lk, const Float3 & rk)
	{
		return rk * lk;
	}
}

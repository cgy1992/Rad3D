/*
*	Float4
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"
#include "MFloat3.h"

namespace Rad {

	class Mat4;

	class M_ENTRY Float4
	{
		DECLARE_ALLOC();

	public:
		static const Float4 Zero;

	public:
		union {

			struct {
				float x, y, z, w;
			};

			struct {
				float r, g, b, a;
			};

			struct {
				float m[4];
			};
		};
		

	public:
		Float4(float _x, float _y, float _z, float _w = 1);
		Float4(const Float3 & rk, float _w = 1);
		Float4(const Float4 & rk);
		Float4();
		~Float4();

		Float4 &
			operator =(const Float4 & rk);

		bool 
			operator ==(const Float4 & rk) const;
		bool 
			operator !=(const Float4 & rk) const;
		bool 
			operator > (const Float4 & rk) const;
		bool 
			operator >=(const Float4 & rk) const;
		bool 
			operator < (const Float4 & rk) const;
		bool 
			operator <=(const Float4 & rk) const;

		Float4 
			operator -() const;

		Float4 
			operator + (const Float4 & rk) const;
		Float4 
			operator - (const Float4 & rk) const;
		Float4 
			operator * (const Float4 & rk) const;
		Float4 
			operator / (const Float4 & rk) const;
		Float4 
			operator * (float f) const;
		Float4 
			operator / (float f) const;

		Float4& 
			operator += (const Float4 & rk);
		Float4& 
			operator -= (const Float4 & rk);
		Float4& 
			operator *= (float rk);
		Float4& 
			operator /= (float rk);

		Float4& 
			operator *= (const Float4 & rk);
		Float4& 
			operator /= (const Float4 & rk);

		Float4 
			operator * (const Mat4 & rk) const;
		Float4& 
			operator *= (const Mat4 & rk);

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
			Distance(const Float4 & rk) const;
		float 
			DistanceSq(const Float4 & rk) const;

		float 
			Dot(const Float4 & rk) const;
		
		void 
			Reflect(const Float4 & n);
		void 
			Refract(const Float4 & n, float eta);

		void 
			Transform(const Mat4 & m);
		void 
			TransformA(const Mat4 & m);
		void 
			TransformN(const Mat4 & m);

		void 
			Saturate();

		Float4
			Cross(const Float4 & rk) const { return Cross(*this, rk); }
		Float4
			Lerp(const Float4 & rk, float t) const { return Lerp(*this, rk, t); }

		static Float4 
			Cross(const Float4 & lk, const Float4 & rk);
		static Float4 
			Lerp(const Float4 & lk, const Float4 & rk, float t);

		static Float4 
			Minimum(const Float4 & lk, const Float4 & rk);
		static Float4 
			Maximum(const Float4 & lk, const Float4 & rk);
	};

	inline const float * Float4::ToFloatPtr() const
	{
		return (const float *)this;
	}

	inline Float4 Float4::Minimum(const Float4 & lk, const Float4 & rk)
	{
		Float4 v;

		v.x = Min(lk.x, rk.x);
		v.y = Min(lk.y, rk.y);
		v.z = Min(lk.z, rk.z);
		v.w = Min(lk.w, rk.w);

		return v;
	}

	inline Float4 Float4::Maximum(const Float4 & lk, const Float4 & rk)
	{
		Float4 v;

		v.x = Max(lk.x, rk.x);
		v.y = Max(lk.y, rk.y);
		v.z = Max(lk.z, rk.z);
		v.w = Max(lk.w, rk.w);

		return v;
	}

	inline Float4 operator * (float lk, const Float4 & rk)
	{
		return rk * lk;
	}
}


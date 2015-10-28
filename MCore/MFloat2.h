/*
*	Float2
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"

namespace Rad {

	class M_ENTRY Float2
	{
		DECLARE_ALLOC();

	public:
		static const Float2 Zero;

	public:
		union {

			struct {
				float x, y;
			};

			struct {
				float m[2];
			};
		};


	public:
		Float2(float _x, float _y);
		Float2(const Float2 & rk);
		Float2();
		~Float2();

		Float2 &
			operator =(const Float2 & rk);

		bool 
			operator ==(const Float2 & rk) const;
		bool 
			operator !=(const Float2 & rk) const;
		bool 
			operator > (const Float2 & rk) const;
		bool 
			operator >=(const Float2 & rk) const;
		bool 
			operator < (const Float2 & rk) const;
		bool 
			operator <=(const Float2 & rk) const;

		Float2 
			operator -() const;

		Float2 
			operator + (const Float2 & rk) const;
		Float2 
			operator - (const Float2 & rk) const;
		Float2 
			operator * (const Float2 & rk) const;
		Float2 
			operator / (const Float2 & rk) const;
		Float2 
			operator * (float f) const;
		Float2 
			operator / (float f) const;

		Float2 & 
			operator += (const Float2 & rk);
		Float2 & 
			operator -= (const Float2 & rk);
		Float2 & 
			operator *= (float rk);
		Float2 & 
			operator /= (float rk);

		Float2 & 
			operator *= (const Float2 & rk);
		Float2 &
			operator /= (const Float2 & rk);

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
			Distance(const Float2 & rk) const;
		float 
			DistanceSq(const Float2 & rk) const;

		float 
			Dot(const Float2 & rk) const;

		void 
			Reflect(const Float2 & n);
		void 
			Refract(const Float2 & n, float eta);

		void 
			Saturate();

		float 
			Cross(const Float2 & rk) const { return Cross(*this, rk); }
		Float2
			Lerp(const Float2 & rk, float t) const { return Lerp(*this, rk, t); }

		static float 
			Cross(const Float2 & lk, const Float2 & rk);
		static Float2 
			Lerp(const Float2 & lk, const Float2 & rk, float t);
		static Float2 
			Minimum(const Float2 & lk, const Float2 & rk) ;
		static Float2 
			Maximum(const Float2 & lk, const Float2 & rk);
	};

	inline float Float2::Cross(const Float2 & lk, const Float2 & rk)
	{
		return lk.x * rk.y - lk.y * rk.x;
	}

	inline Float2 Float2::Lerp(const Float2 & lk, const Float2 & rk, float t)
	{
		return lk + (rk - lk) * t;
	}

	inline Float2 Float2::Minimum(const Float2 & lk, const Float2 & rk)
	{
		Float2 v;

		v.x = Min(lk.x, rk.x);
		v.y = Min(lk.y, rk.y);

		return v;
	}

	inline Float2 Float2::Maximum(const Float2 & lk, const Float2 & rk)
	{
		Float2 v;

		v.x = Max(lk.x, rk.x);
		v.y = Max(lk.y, rk.y);

		return v;
	}

	inline Float2 operator * (float lk, const Float2 & rk)
	{
		return rk * lk;
	}
}


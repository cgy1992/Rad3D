/*
*	Util
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MLog.h"
#include "MTypes.h"

namespace Rad { namespace util {

	inline Float3 Float4ToFloat3(const Float4 & f4)
	{
		return Float3(f4.x, f4.y, f4.z);
	}

	inline Float4 Float3ToFloat4(const Float3 & f3)
	{
		return Float4(f3.x, f3.y, f3.z, 1);
	}

	inline Rgba32 TO_RGBE(const Float3 & color)
	{
		Rgba32 c;

		float e;
		e = Max(color.r, color.g);
		e = Max(e, color.b);

		if (e <= 1)
		{
			c.r = (unsigned char)(color.r * 255);
			c.g = (unsigned char)(color.g * 255);
			c.b = (unsigned char)(color.b * 255);
			c.a = 0;
		}
		else
		{
			Float3 rk = color / e;
			e = Min(e, 3.0f) - 1;

			c.r = (unsigned char)(rk.r * 255);
			c.g = (unsigned char)(rk.g * 255);
			c.b = (unsigned char)(rk.b * 255);
			c.a = (unsigned char)(e / 2.0f * 255);
		}

		return c;
	}

	inline Rgba32 TO_RGBE_EX(const Float4 & color)
	{
		return TO_RGBE(Float4ToFloat3(color));
	}

	inline Float3 RGBE_TO(Rgba32 color)
	{
		Float3 c;

		float e = 1 + color.a / 255.0f * 2.0f;
		c.r = color.r / 255.0f * e;
		c.g = color.g / 255.0f * e;
		c.b = color.b / 255.0f * e;

		return c;
	}

	inline Float4 RGBE_TO_EX(Rgba32 color)
	{
		Float4 c;

		float e = 1 + color.a / 255.0f * 2.0f;
		c.r = color.r / 255.0f * e;
		c.g = color.g / 255.0f * e;
		c.b = color.b / 255.0f * e;
		c.a = 1;

		return c;
	}

	inline bool LineIntersects(Float2 & cp, const Float2 & A, const Float2 & B, const Float2 & C, const Float2 & D)
	{
		float denominator = (B.x-A.x)*(D.y-C.y) - (B.y-A.y)*(D.x-C.x);

		if (Math::Abs(denominator) < EPSILON_E4)
			return false;

		float numeratorR = (A.y-C.y)*(D.x-C.x) - (A.x-C.x)*(D.y-C.y);
		float numeratorS = (A.y-C.y)*(B.x-A.x) - (A.x-C.x)*(B.y-A.y);

		float r = numeratorR / denominator;
		float s = numeratorS / denominator;

		cp.x = A.x+r*(B.x-A.x);
		cp.y = A.y+r*(B.y-A.y);

		if ( r >= 0 && r <= 1 && s >= 0 && s <= 1 )
			return true;
		else
			return false;
	}

	inline void FastNormalize2(Float2 & n)
	{
		float len = n.LengthSq();
		if (len > 0)
		{
			n *= Math::FastInvSqrt(len);
		}
	}

	inline void FastNormalize3(Float3 & n)
	{
		float len = n.LengthSq();
		if (len > 0)
		{
			n *= Math::FastInvSqrt(len);
		}
	}

	inline Float2 RandomDir2()
	{
		float theta = Math::RandomUnit() * PI2;

		Float2 d;
		d.x = Math::FastCos(theta);
		d.y = Math::FastSin(theta);

		return d;
	}

	inline Float3 RandomDir3()
	{
		float theta = Math::RandomUnit() * PI;
		float beta = Math::RandomUnit() * PI2;

		float r = Math::FastSin(theta);

		Float3 d;
		d.x = Math::FastCos(beta) * r;
		d.y = Math::FastCos(theta);
		d.z = Math::FastSin(beta) * r;

		return d;
	}

}
}
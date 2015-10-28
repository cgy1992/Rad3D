/*
*	Int2
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"

namespace Rad {

	struct M_ENTRY Int2
	{
		union {

			struct {
				int x, y;
			};

			int m[2];
		};

		Int2() : x(0), y(0) {}
		Int2(int _x, int _y) : x(_x), y(_y) {}

		Int2 operator -() const
		{
			return Int2(-x, -y);
		}

		Int2 operator +(const Int2 & rk) const
		{
			return Int2(x + rk.x, y + rk.y);
		}

		Int2 operator -(const Int2 & rk) const
		{
			return Int2(x - rk.x, y - rk.y);
		}

		void operator +=(const Int2 & rk)
		{
			x += rk.x;
			y += rk.y;
		}

		void operator -=(const Int2 & rk)
		{
			x -= rk.x;
			y -= rk.y;
		}

		Int2 operator *(const Int2 & rk) const
		{
			return Int2(x * rk.x, y * rk.y);
		}

		Int2 operator /(const Int2 & rk) const
		{
			return Int2(x / rk.x, y / rk.y);
		}

		void operator *=(const Int2 & rk)
		{
			x *= rk.x;
			y *= rk.y;
		}

		void operator /=(const Int2 & rk)
		{
			x /= rk.x;
			y /= rk.y;
		}

		int operator[](int i) const
		{
			d_assert (i < 2);

			return m[i];
		}

		int & operator[](int i)
		{
			d_assert (i < 2);

			return m[i];
		}

		bool operator ==(const Int2 & rk) const
		{
			return x == rk.x && y == rk.y;
		}

		bool operator !=(const Int2 & rk) const
		{
			return x != rk.x || y != rk.y;
		}

		bool operator > (const Int2 & rk) const
		{
			return x > rk.x && y > rk.y;
		}

		bool operator >=(const Int2 & rk) const
		{
			return x >= rk.x && y >= rk.y;
		}

		bool operator < (const Int2 & rk) const
		{
			return x < rk.x && y < rk.y;
		}

		bool operator <=(const Int2 & rk) const
		{
			return x <= rk.x && y <= rk.y;
		}

		Int2 & operator =(const Int2 & rk)
		{
			x = rk.x, y = rk.y;
			return *this;
		}
	};

}
/*
*	Rect
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

namespace Rad {

	template <class T>
	struct Rect2
	{
		union {
			struct {
				T x1, y1;
				T x2, y2;
			};

			struct {
				T m[4];
			};
		};
		

		Rect2() : x1(0), y1(0), x2(0), y2(0) {}
		Rect2(T _x1, T _y1, T _x2, T _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

		Rect2 operator +(const Rect2 & k) const
		{
			Rect2 rect;
			rect.x1 = x1 + k.x1;
			rect.y1 = y1 + k.y1;
			rect.x2 = x2 + k.x2;
			rect.y2 = y2 + k.y2;

			return rect;
		}

		Rect2 operator -(const Rect2 & k) const
		{
			Rect2 rect;
			rect.x1 = x1 - k.x1;
			rect.y1 = y1 - k.y1;
			rect.x2 = x2 - k.x2;
			rect.y2 = y2 - k.y2;

			return rect;
		}

		Rect2 operator *(T k) const
		{
			Rect2 rect;
			rect.x1 = x1 * k;
			rect.y1 = y1 * k;
			rect.x2 = x2 * k;
			rect.y2 = y2 * k;

			return rect;
		}

		Rect2 operator /(T k) const
		{
			Rect2 rect;
			rect.x1 = x1 / k;
			rect.y1 = y1 / k;
			rect.x2 = x2 / k;
			rect.y2 = y2 / k;

			return rect;
		}

		Rect2 & operator +=(const Rect2 & k)
		{
			x1 = x1 + k.x1;
			y1 = y1 + k.y1;
			x2 = x2 + k.x2;
			y2 = y2 + k.y2;

			return *this;
		}
		
		Rect2 & operator -=(const Rect2 & k)
		{
			x1 = x1 - k.x1;
			y1 = y1 - k.y1;
			x2 = x2 - k.x2;
			y2 = y2 - k.y2;

			return *this;
		}

		Rect2 & operator *=(T k)
		{
			x1 = x1 * k;
			y1 = y1 * k;
			x2 = x2 * k;
			y2 = y2 * k;

			return *this;
		}
		
		Rect2 & operator /=(T k)
		{
			x1 = x1 / k;
			y1 = y1 / k;
			x2 = x2 / k;
			y2 = y2 / k;

			return *this;
		}

		bool operator !=(const Rect2 & k) const
		{
			return x1 != k.x1 || y1 != k.y1 || x2 != k.x2 || y2 != k.y2;
		}

		bool operator ==(const Rect2 & k) const
		{
			return x1 == k.x1 && y1 == k.y1 && x2 == k.x2 && y2 == k.y2;
		}

		T Width() const
		{ 
			return x2 - x1;
		}
		
		T Height() const
		{
			return y2 - y1; 
		}

		T Square() const
		{
			return (x2 - x1) * (y2 - y2);
		}

		bool Valid() const
		{
			return x2 >= x1 && y2 >= y1;
		}

		void Merge(const Rect2 & rk)
		{
			x1 = Min(x1, rk.x1);
			y1 = Min(y1, rk.y1);
			x2 = Max(x2, rk.x2);
			y2 = Max(y2, rk.y2);
		}

		void Merge(T x, T y)
		{
			x1 = Min(x1, x);
			y1 = Min(y1, y);
			x2 = Max(x2, x);
			y2 = Max(y2, y);
		}

		bool Contain(T x, T y) const
		{
			return x >= x1 && x <= x2 && y >= y1 && y <= y2;
		}

		bool Contain(const Rect2 & rk) const
		{
			return rk.x1 >= x1 && rk.x2 <= x2 && rk.y1 >= y1 && rk.y2 <= y2;
		}

		bool Intersect(const Rect2 & rk) const
		{
			Rect2 rect;
			rect.x1 = Max(x1, rk.x1);
			rect.y1 = Max(y1, rk.y1);
			rect.x2 = Min(x2, rk.x2);
			rect.y2 = Min(y2, rk.y2);

			return rect.Valid();
		}

		bool Intersect(T x, T y, T radius) const
		{
			if (y < y1)
			{
				if (y1 - y > radius)
					return false;

				if (x < x1)
				{
					return (x1 - x) < radius;
				}
				else if (x > x2)
				{
					return (x - x2) < radius;
				}
				else
				{
					return true;
				}
			}
			else if (y > y2)
			{
				if (y - y2 > radius)
					return false;

				if (x < x1)
				{
					return (x1 - x) < radius;
				}
				else if (x > x2)
				{
					return (x - x2) < radius;
				}
				else
				{
					return true;
				}
			}
			else
			{
				if (x < x1)
				{
					return (x1 - x) < radius;
				}
				else if (x > x2)
				{
					return (x - x2) < radius;
				}
				else
				{
					return true;
				}
			}
		}

		Rect2 GetIntersectRect2(const Rect2 & rk) const
		{
			Rect2 rect;
			rect.x1 = Max(x1, rk.x1);
			rect.y1 = Max(y1, rk.y1);
			rect.x2 = Min(x2, rk.x2);
			rect.y2 = Min(y2, rk.y2);

			return rect;
		}
	};

	typedef Rect2<int> RectI;
	typedef Rect2<float> RectF;
}

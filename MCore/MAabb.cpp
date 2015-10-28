#include "MAABB.h"
#include "MSphere.h"
#include "MObb.h"

namespace Rad {

	const Aabb Aabb::Invalid   = Aabb(FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	const Aabb Aabb::Infinite  = Aabb(-FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	const Aabb Aabb::Zero      = Aabb(0, 0, 0, 0, 0, 0);
	const Aabb Aabb::Identiy   = Aabb(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f);
	const Aabb Aabb::Default   = Aabb::Identiy * UNIT_METRES;

	Aabb::Aabb()
	{
	}

	Aabb::Aabb(const Float3 & vMin, const Float3 & vMax)
	: minimum(vMin.x, vMin.y, vMin.z),
	  maximum(vMax.x, vMax.y, vMax.z)
	{
	}

	Aabb::Aabb(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ)
	: minimum(fMinX, fMinY, fMinZ),
	  maximum(fMaxX, fMaxY, fMaxZ)
	{
	}

	Aabb::~Aabb()
	{
	}

	Aabb & Aabb::operator =(const Aabb & aabb)
	{
		minimum = aabb.minimum;
		maximum = aabb.maximum;
		return *this;
	}

	Aabb Aabb::operator *(float x) const
	{
		return Aabb(minimum * x, maximum * x);
	}

	Aabb Aabb::operator /(float x) const
	{
		x = 1.0f / x;
		return Aabb(minimum * x, maximum * x);
	}

	Aabb & Aabb::operator *=(float x)
	{
		minimum *= x;
		maximum *= x;
		return *this;
	}

	Aabb & Aabb::operator /=(float x)
	{
		x = 1.0f / x;
		minimum *= x;
		maximum *= x;
		return *this;
	}

	Float3 Aabb::GetSize() const
	{
		return Float3(maximum.x - minimum.x, maximum.y - minimum.y, maximum.z - minimum.z);
	}

	Float3 Aabb::GetHalfSize() const
	{
		return Float3(maximum.x - minimum.x, maximum.y - minimum.y, maximum.z - minimum.z) * 0.5f;
	}

	Float3 Aabb::GetCenter() const
	{
		return (maximum + minimum) * 0.5f; 
	}

	void Aabb::GetPoints(Float3 * points) const
	{
		points[0] = Float3(minimum.x, minimum.y, minimum.z);
		points[1] = Float3(minimum.x, maximum.y, minimum.z);
		points[2] = Float3(maximum.x, minimum.y, minimum.z);
		points[3] = Float3(maximum.x, maximum.y, minimum.z);

		points[4] = Float3(minimum.x, minimum.y, maximum.z);
		points[5] = Float3(minimum.x, maximum.y, maximum.z);
		points[6] = Float3(maximum.x, minimum.y, maximum.z);
		points[7] = Float3(maximum.x, maximum.y, maximum.z);
	}

	bool Aabb::Valid() const
	{
		return maximum.x >= minimum.x && maximum.y >= minimum.y && maximum.z >= minimum.z;
	}

	float Aabb::Volume() const
	{
		return (maximum.x - minimum.x) * (maximum.y - minimum.y) * (maximum.z - minimum.z);
	}

	bool Aabb::Contain(const Aabb & rk) const
	{
		return 
			(rk.minimum.x >= minimum.x && rk.maximum.x <= maximum.x) &&
			(rk.minimum.y >= minimum.y && rk.maximum.y <= maximum.y) &&
			(rk.minimum.z >= minimum.z && rk.maximum.z <= maximum.z);
	}

	bool Aabb::Contain(const RectF & rect) const
	{
		return 
			(rect.x1 >= minimum.x && rect.x2 <= maximum.x) &&
			(rect.y1 >= minimum.z && rect.y2 <= maximum.z);
	}

	bool Aabb::Contain(const Float3 & rk) const
	{
		return
			(rk.x >= minimum.x && rk.x <= maximum.x) &&
			(rk.y >= minimum.y && rk.y <= maximum.y) &&
			(rk.z >= minimum.z && rk.z <= maximum.z);
	}

	bool Aabb::Contain(const Sphere & sph) const
	{
		return
			Contain(sph.center - Float3(1, 1, 1) * sph.radius) &&
			Contain(sph.center + Float3(1, 1, 1) * sph.radius);
	}

	void Aabb::Merge(const Aabb & rk)
	{
		minimum.x = Min(minimum.x, rk.minimum.x);
		minimum.y = Min(minimum.y, rk.minimum.y);
		minimum.z = Min(minimum.z, rk.minimum.z);
		maximum.x = Max(maximum.x, rk.maximum.x);
		maximum.y = Max(maximum.y, rk.maximum.y);
		maximum.z = Max(maximum.z, rk.maximum.z);
	}

	void Aabb::Merge(const Float3 & rk)
	{
		minimum.x = Min(minimum.x, rk.x);
		minimum.y = Min(minimum.y, rk.y);
		minimum.z = Min(minimum.z, rk.z);
		maximum.x = Max(maximum.x, rk.x);
		maximum.y = Max(maximum.y, rk.y);
		maximum.z = Max(maximum.z, rk.z);
	}

	void Aabb::Transform(const Mat4 & mat)
	{
		float av, bv;
		int i, j;

		Float3 nmin(mat._41, mat._42, mat._43);
		Float3 nmax(mat._41, mat._42, mat._43);

		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				av = mat[j][i] * minimum[j];
				bv = mat[j][i] * maximum[j];

				if (av < bv)
				{
					nmin[i] += av;
					nmax[i] += bv;
				}
				else
				{
					nmin[i] += bv;
					nmax[i] += av;
				}
			}
		}

		minimum = nmin;
		maximum = nmax;

#if 0
		Float3 p[8];
		GetPoints(p);

		for (int i = 0; i < 8; ++i)
			p[i].TransformA(mat);

		minimum = p[0];
		maximum = p[0];

		for (int i = 1; i < 8; ++i)
		{
			minimum = Float3::Minimum(minimum, p[i]);
			maximum = Float3::Maximum(maximum, p[i]);
		}
#endif
	}

	bool Aabb::Intersect(const RectF & rect) const
	{
		if (rect.x1 >= maximum.x ||
			rect.y1 >= maximum.z ||
			rect.x2 <= minimum.x ||
			rect.y2 <= minimum.z)
			return false;

		return true;
	}

	bool Aabb::Intersect(const Aabb & rk) const
	{
		if (minimum.x >= rk.maximum.x ||
			minimum.y >= rk.maximum.y ||
			minimum.z >= rk.maximum.z ||
			maximum.x <= rk.minimum.x ||
			maximum.y <= rk.minimum.y ||
			maximum.z <= rk.minimum.z)
			return false;

		return true;
	}

	bool Aabb::Intersect(const Sphere & rk) const
	{
		Obb obb(minimum, maximum);

		return obb.Intersect(rk);
	}

	Aabb Aabb::GetIntersectAabb(const Aabb & rk) const
	{
		Aabb aabbOut;
		const Aabb & aabb1 = *this;
		const Aabb & aabb2 = rk;

		aabbOut.minimum.x = Max(aabb1.minimum.x, aabb2.minimum.x);
		aabbOut.minimum.y = Max(aabb1.minimum.y, aabb2.minimum.y);
		aabbOut.minimum.z = Max(aabb1.minimum.z, aabb2.minimum.z);

		aabbOut.maximum.x = Min(aabb1.maximum.x, aabb2.maximum.x);
		aabbOut.maximum.x = Min(aabb1.maximum.y, aabb2.maximum.y);
		aabbOut.maximum.x = Min(aabb1.maximum.z, aabb2.maximum.z);

		return aabbOut;
	}

}
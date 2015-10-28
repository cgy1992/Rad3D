#include "MSphere.h"
#include "MAabb.h"

namespace Rad {

	const Sphere Sphere::Zero     = Sphere(0, 0, 0, 0);
	const Sphere Sphere::Identity = Sphere(0, 0, 0, 1);
	const Sphere Sphere::Infinite = Sphere(0, 0, 0, FLT_MAX);

	Sphere::Sphere()
	{
	}

	Sphere::Sphere(const Float3 & _center, float _radius)
	: center(_center.x, _center.y, _center.z)
	, radius(_radius)
	{
	}

	Sphere::Sphere(float x, float y, float z, float _radius)
	: center(x, y, z),
	  radius(_radius)
	{
	}

	Sphere::~Sphere()
	{
	}

	Sphere & Sphere::operator =(const Sphere & sph)
	{
		center = sph.center;
		radius = sph.radius;
		return *this;
	}

	Sphere Sphere::operator *(float x) const
	{
		return Sphere(center * x, radius * x);
	}

	Sphere Sphere::operator /(float x) const
	{
		x = 1.0f / x;
		return Sphere(center * x, radius * x);
	}

	Sphere & Sphere::operator *=(float x)
	{
		center *= x;
		radius *= x;
		return *this;
	}

	Sphere & Sphere::operator /=(float x)
	{
		x = 1.0f / x;
		center *= x;
		radius *= x;
		return *this;
	}

	float Sphere::Volume() const
	{
		return (4.0f / 3.0f) *  PI * radius * radius * radius;
	}

	void Sphere::Merge(const Sphere & rk)
	{
		center = (center + rk.center) * 0.5f;
		radius = center.Distance(rk.center) + radius > rk.radius ? radius : rk.radius;
	}

	void Sphere::Merge(const Float3 & rk)
	{
		float fdist = center.Distance(rk);
		radius = (radius < fdist) ?  fdist : radius;
	}

	void Sphere::Transform(const Mat4 & mat)
	{
		center.TransformA(mat);
	}

	bool Sphere::Contain(const Float3 & rk) const
	{
		return center.DistanceSq(rk) <= radius * radius;
	}

	bool Sphere::Contain(const Sphere & rk) const
	{
		return center.Distance(rk.center) + rk.radius <= radius;
	}

	bool Sphere::Intersect(const Sphere & rk) const
	{
		float lenSq = (radius + radius);
		lenSq *= lenSq;

		return center.DistanceSq(rk.center) <= lenSq ;
	}

	bool Sphere::Intersect(const Aabb & rk) const
	{
		return rk.Intersect(*this);
	}

}

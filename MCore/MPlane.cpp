#include "MPlane.h"
#include "MSphere.h"
#include "MAabb.h"

namespace Rad {

	const Plane Plane::Zero = Plane(Float3(0, 0, 0), 0);

	Plane::Plane()
		: d(0)
	{
	}

	Plane::Plane(const Plane & p)
		: normal(p.normal), d(p.d)
	{
	}

	Plane::Plane(const Float3 & _n, float _d)
		: normal(_n.x, _n.y, _n.z), d(_d) 
	{
	}

	Plane::Plane(const Float3 & _n, const Float3 & _p)
		: normal(_n)
	{
		d = -normal.Dot(_p);
	}

	Plane::Plane(const Float3 & p1, const Float3 & p2, const Float3 & p3)
	{
		normal = Float3::Cross(p2 - p1, p3 - p1);
		d = -normal.Dot(p1);
	}

	Plane::Plane(float a, float b, float c, float d)
		: normal(a, b, c), d(d)
	{
	}

	Plane::~Plane()
	{
	}

	Plane& Plane::operator =(const Plane & p)
	{
		normal = p.normal, d = p.d;
		return *this;
	}

	Plane Plane::operator -() const
	{
		return Plane(-normal, -d);
	}

	bool Plane::operator ==(const Plane & p) const
	{
		return normal == p.normal && d == p.d;
	}

	bool Plane::operator !=(const Plane & p) const
	{
		return normal != p.normal  || d != p.d;
	}

	void Plane::Normalize()
	{
		float len = normal.Normalize();
		if (len > 0)
			d /= len;
	}

	float Plane::Distance(const Float3 & v) const
	{
		return v.x * normal.x + v.y * normal.y + v.z * normal.z + d;
	}

	Plane::Side Plane::AtSide(const Float3 & v) const
	{
		float d = Distance(v);

		if (d > 0)
			return Plane::POSITIVE;
		else if (d < 0)
			return Plane::NEGATIVE;
		else
			return Plane::BOTH;
	}

	Plane::Side Plane::AtSide(const Aabb & box) const
	{
		Float3 center = box.GetCenter();
		Float3 half = box.GetHalfSize();

		return AtSide(center, half);
	}

	Plane::Side Plane::AtSide(const Float3 & v, const Float3 & half) const
	{
		float d = Distance(v);
		float dist = Math::Abs(half.x * normal.x) + Math::Abs(half.y * normal.y) + Math::Abs(half.z * normal.z);

		if (d > dist)
			return Plane::POSITIVE;
		else if (d < -dist)
			return Plane::NEGATIVE;
		else
			return Plane::BOTH;
	}

	Plane::Side Plane::AtSide(const Sphere & sph) const
	{
		float d = Distance(sph.center);

		if (d > sph.radius)
			return Plane::POSITIVE;
		else if (d < -sph.radius)
			return Plane::NEGATIVE;
		else
			return Plane::BOTH;
	}

	void Plane::Transform(const Mat4 & m)
	{
		Float3 pos = normal * -d;
		pos.TransformA(m);
		normal.TransformN(m);

		d = -normal.Dot(pos);
	}

}

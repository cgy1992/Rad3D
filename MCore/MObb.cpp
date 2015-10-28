#include "MObb.h"
#include "MAabb.h"
#include "MSphere.h"

namespace Rad {

	const Obb Obb::Zero = Obb(Float3::Zero, Float3::Zero);
	const Obb Obb::Identiy = Obb(Float3(-0.5f, -0.5f, -0.5f), Float3(0.5f, 0.5f, 0.5f));

	Obb::Obb()
	{
	}

	Obb::Obb(const Float3 * _axis, const Float3 & _center, const Float3 & _extent)
	{
		axis[0] = _axis[0];
		axis[1] = _axis[1];
		axis[2] = _axis[2];

		center = _center;
		extent = _extent;
	}

	Obb::Obb(const Float3 & vMin, const Float3 & vMax)
	{
		axis[0] = Float3(1, 0, 0);
		axis[1] = Float3(0, 1, 0);
		axis[2] = Float3(0, 0, 1);

		center = (vMax + vMin) * 0.5f;
		extent = (vMax - vMin) * 0.5f;
	}

	Obb::~Obb()
	{
	}

	void Obb::GetPoints(Float3 * points) const
	{
		points[0] = center - axis[0] * extent.x - axis[1] * extent.y - axis[2] * extent.z;
		points[1] = center + axis[0] * extent.x - axis[1] * extent.y - axis[2] * extent.z;
		points[2] = center - axis[0] * extent.x + axis[1] * extent.y - axis[2] * extent.z;
		points[3] = center + axis[0] * extent.x + axis[1] * extent.y - axis[2] * extent.z;

		points[4] = center - axis[0] * extent.x - axis[1] * extent.y + axis[2] * extent.z;
		points[5] = center + axis[0] * extent.x - axis[1] * extent.y + axis[2] * extent.z;
		points[6] = center - axis[0] * extent.x + axis[1] * extent.y + axis[2] * extent.z;
		points[7] = center + axis[0] * extent.x + axis[1] * extent.y + axis[2] * extent.z;
	}

	Mat4 Obb::GetWorldToLocalTM() const
	{
		Mat4 form;
		form.MakeRotationAxis(axis[0], axis[1], axis[2]);
		form._41 = center.x; form._42 = center.y; form._43 = center.z;

		form.InverseOrtho();

		return form;
	}

	void Obb::Transform(const Mat4 & form)
	{
		axis[0].TransformN(form);
		axis[1].TransformN(form);
		axis[2].TransformN(form);

		center.TransformA(form);
	}

	bool Obb::Contain(const Float3 & p) const
	{
		Float3 local_p = p - center;

		float proj = local_p.Dot(axis[0]);
		if (proj > extent[0] || proj < -extent[0])
			return false;

		proj = local_p.Dot(axis[1]);
		if (proj > extent[1] || proj < -extent[1])
			return false;

		proj = local_p.Dot(axis[2]);
		if (proj > extent[2] || proj < -extent[2])
			return false;

		return true;
	}

	bool Obb::Contain(const Obb & rk) const
	{
		Float3 points[8];
		GetPoints(points);

		if (!Contain(points[0])) return false;
		if (!Contain(points[1])) return false;
		if (!Contain(points[2])) return false;
		if (!Contain(points[3])) return false;
		if (!Contain(points[4])) return false;
		if (!Contain(points[5])) return false;
		if (!Contain(points[6])) return false;
		if (!Contain(points[7])) return false;

		return true;
	}

#define DOT(x, y) (x[0] * y[0] + x[1] * y[1] + x[2] * y[2])
	bool Obb::Intersect(const Obb & rk, float epsilon) const
	{
		// Generate a rotation matrix that transforms from world space to this OBB's coordinate space.
		Mat4 R;
		for(int i = 0; i < 3; ++i)
			for(int j = 0; j < 3; ++j)
				R[i][j] = DOT(axis[i], rk.axis[j]);

		Float3 t = rk.center - center;
		// Express the translation vector in a's coordinate frame.
		t = Float3(DOT(t, axis[0]), DOT(t, axis[1]), DOT(t, axis[2]));

		Mat4 AbsR;
		for(int i = 0; i < 3; ++i)
			for(int j = 0; j < 3; ++j)
				AbsR[i][j] = Math::Abs(R[i][j]) + epsilon;

		// Test the three major axes of this OBB.
		for(int i = 0; i < 3; ++i)
		{
			float ra = extent[i];
			float rb = DOT(rk.extent, AbsR[i]);
			if (Math::Abs(t[i]) > ra + rb)
				return false;
		}

		// Test the three major axes of the OBB b.
		for(int i = 0; i < 3; ++i)
		{
			float ra = extent[0] * AbsR[0][i] + extent[1] * AbsR[1][i] + extent[2] * AbsR[2][i];
			float rb = rk.extent[i];
			if (Math::Abs(t.x * R[0][i] + t.y * R[1][i] + t.z * R[2][i]) > ra + rb)
				return false;
		}

		// Test the 9 different cross-axes.

		// A.x <cross> B.x
		float ra = extent.y * AbsR[2][0] + extent.z * AbsR[1][0];
		float rb = rk.extent.y * AbsR[0][2] + rk.extent.z * AbsR[0][1];
		if (Math::Abs(t.z * R[1][0] - t.y * R[2][0]) > ra + rb)
			return false;

		// A.x < cross> B.y
		ra = extent.y * AbsR[2][1] + extent.z * AbsR[1][1];
		rb = rk.extent.x * AbsR[0][2] + rk.extent.z * AbsR[0][0];
		if (Math::Abs(t.z * R[1][1] - t.y * R[2][1]) > ra + rb)
			return false;

		// A.x <cross> B.z
		ra = extent.y * AbsR[2][2] + extent.z * AbsR[1][2];
		rb = rk.extent.x * AbsR[0][1] + rk.extent.y * AbsR[0][0];
		if (Math::Abs(t.z * R[1][2] - t.y * R[2][2]) > ra + rb)
			return false;

		// A.y <cross> B.x
		ra = extent.x * AbsR[2][0] + extent.z * AbsR[0][0];
		rb = rk.extent.y * AbsR[1][2] + rk.extent.z * AbsR[1][1];
		if (Math::Abs(t.x * R[2][0] - t.z * R[0][0]) > ra + rb)
			return false;

		// A.y <cross> B.y
		ra = extent.x * AbsR[2][1] + extent.z * AbsR[0][1];
		rb = rk.extent.x * AbsR[1][2] + rk.extent.z * AbsR[1][0];
		if (Math::Abs(t.x * R[2][1] - t.z * R[0][1]) > ra + rb)
			return false;

		// A.y <cross> B.z
		ra = extent.x * AbsR[2][2] + extent.z * AbsR[0][2];
		rb = rk.extent.x * AbsR[1][1] + rk.extent.y * AbsR[1][0];
		if (Math::Abs(t.x * R[2][2] - t.z * R[0][2]) > ra + rb)
			return false;

		// A.z <cross> B.x
		ra = extent.x * AbsR[1][0] + extent.y * AbsR[0][0];
		rb = rk.extent.y * AbsR[2][2] + rk.extent.z * AbsR[2][1];
		if (Math::Abs(t.y * R[0][0] - t.x * R[1][0]) > ra + rb)
			return false;

		// A.z <cross> B.y
		ra = extent.x * AbsR[1][1] + extent.y * AbsR[0][1];
		rb = rk.extent.x * AbsR[2][2] + rk.extent.z * AbsR[2][0];
		if (Math::Abs(t.y * R[0][1] - t.x * R[1][1]) > ra + rb)
			return false;

		// A.z <cross> B.z
		ra = extent.x * AbsR[1][2] + extent.y * AbsR[0][2];
		rb = rk.extent.x * AbsR[2][1] + rk.extent.y * AbsR[2][0];
		if (Math::Abs(t.y * R[0][2] - t.x * R[1][2]) > ra + rb)
			return false;

		// No separating axis exists, so the two OBB don't intersect.
		return true;
	}
#undef DOT

	bool Obb::Intersect(const Aabb & rk) const
	{
		Obb obb(rk.minimum, rk.maximum);

		return Intersect(obb);
	}

	bool Obb::Intersect(const Sphere & rk) const
	{
		Float3 dir = rk.center - center;
		Float3 point = center;

		for (int i = 0; i < 3; ++i)
		{
			float d = axis[i].Dot(dir);

			d = Math::Clamp(d, -rk.radius, rk.radius);

			point += d * axis[i];
		}

		dir = point - rk.center;

		return dir.LengthSq() < rk.radius * rk.radius;
	}
}


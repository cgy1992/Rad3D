/*
*	Obb
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat3.h"
#include "MMat4.h"

namespace Rad {

	class Aabb;
	class Sphere;

	class M_ENTRY Obb
	{
		DECLARE_ALLOC();

	public:
		static const Obb Zero;
		static const Obb Identiy;

	public:
		Obb();
		Obb(const Float3 * axis, const Float3 & center, const Float3 & extent);
		Obb(const Float3 & vMin, const Float3 & vMax);
		~Obb();

		Float3 
			Size() const { return extent * 2; }
		float 
			Volume() const { return extent.x * extent.y * extent.z * 8; }
		void 
			GetPoints(Float3 * point) const;
		Mat4 
			GetWorldToLocalTM() const;

		void 
			Transform(const Mat4 & form);

		bool 
			Contain(const Float3 & rk) const;
		bool 
			Contain(const Obb & rk) const;

		bool 
			Intersect(const Obb & rk, float epsilon = EPSILON_E4) const;
		bool 
			Intersect(const Aabb & rk) const;
		bool 
			Intersect(const Sphere & rk) const;

	public:
		Float3 axis[3];
		Float3 center;
		Float3 extent;
	};

}
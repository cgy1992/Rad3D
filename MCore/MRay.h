/*
*	Ray
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat3.h"

namespace Rad {

	class Plane;
	class Aabb;
	class Obb;
	class Sphere;

	class M_ENTRY Ray
	{
		DECLARE_ALLOC();

	public:
		Ray();
		~Ray();

		Ray(const Float3& vOrigin, const Float3& vDirection);

		bool 
			Intersect(float * dist, const Plane & plane) const;
		bool 
			Intersect(float * dist, const Sphere & sph) const;
		bool 
			Intersect(float * dist, const Aabb & abb) const;
		bool 
			Intersect(float * dist, const Obb & obb) const;
		bool 
			Intersect(float * dist, const Float3 & a, const Float3 & b, const Float3 & c) const;
		bool 
			Intersect(float * dist, float & tu, float & tv, const Float3 & a, const Float3 & b, const Float3 & c) const;
		bool 
			Intersect(float * dist, const Float3 * vtx, const short * idx, int numTris, const Mat4 & form) const;

	public:
		Float3 orig;
		Float3 dir;
	};


}

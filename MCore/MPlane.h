/*
*	Plane
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

	class M_ENTRY Plane
	{
		DECLARE_ALLOC();

	public:
		static const Plane Zero;

	public:
		enum Side {
			POSITIVE,
			NEGATIVE,
			BOTH,
		};

		Float3 normal;
		float d;

	public:
		Plane();
		Plane(const Plane & p);
		Plane(const Float3 & n, float d);
		Plane(const Float3 & n, const Float3 & p);
		Plane(const Float3 & p1, const Float3 & p2, const Float3 & p3);
		Plane(float a, float b, float c, float d);
		~Plane();

		Plane& 
			operator =(const Plane & p);
		Plane 
			operator -() const;

		bool 
			operator ==(const Plane & p) const;
		bool 
			operator !=(const Plane & p) const;

	public:
		void 
			Normalize();
		float 
			Distance(const Float3 & point) const;
		void 
			Transform(const Mat4 & mat);

		Side 
			AtSide( const Float3 & point) const;
		Side 
			AtSide(const Aabb & aabb) const;
		Side 
			AtSide(const Float3 & point, const Float3 & halfSize) const;
		Side 
			AtSide(const Sphere & sph) const;
	};

}

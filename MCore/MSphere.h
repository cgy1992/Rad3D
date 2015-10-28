/*
*	Sphere
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat3.h"

namespace Rad {

	class Aabb;

	class M_ENTRY Sphere
	{
		DECLARE_ALLOC();

	public:
		static const Sphere Zero;
		static const Sphere Identity;
		static const Sphere Infinite;

	public:
		Sphere();
		Sphere(const Float3 & _center, float _radius);
		Sphere(float x, float y, float z, float _radius);
		~Sphere();

		Sphere & 
			operator =(const Sphere & sph);

		Sphere 
			operator *(float x) const;
		Sphere 
			operator /(float x) const;

		Sphere & 
			operator *=(float x);
		Sphere & 
			operator /=(float x);

	public:
		float 
			Volume() const;
		
		void 
			Merge(const Sphere & sph);
		void 
			Merge(const Float3 & point);
		
		void 
			Transform(const Mat4 & mat);

		bool 
			Contain(const Float3 & point) const;
		bool 
			Contain(const Sphere & sph) const;

		bool 
			Intersect(const Sphere & sph) const;
		bool 
			Intersect(const Aabb & aabb) const;

	public:
		Float3 center;
		float radius;
	};

}

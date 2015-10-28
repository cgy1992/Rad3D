/*
*	Aabb
*		Axis aligned bounding box
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat3.h"
#include "MMat4.h"
#include "MRect.h"
#include "MSphere.h"

namespace Rad {

	class Sphere;

	class M_ENTRY Aabb
	{
		DECLARE_ALLOC();

	public:
		static const Aabb Invalid;
		static const Aabb Infinite;
		static const Aabb Zero;
		static const Aabb Identiy;
		static const Aabb Default;

	public:
		Aabb();
		Aabb(const Float3 & vMin, const Float3 & vMax);
		Aabb(float fMinX, float fMinY, float fMinZ,
			 float fMaxX, float fMaxY, float fMaxZ);
		~Aabb();

		Aabb & 
			operator =(const Aabb & aabb);
		Aabb   
			operator *(float x) const;
		Aabb   
			operator /(float x) const;

		Aabb & 
			operator *=(float x);
		Aabb & 
			operator /=(float x);

		friend M_ENTRY Aabb 
			operator *(float x, const Aabb & aabb);
		friend M_ENTRY Aabb 
			operator /(float x, const Aabb & aabb);

	public:
		Float3	
			GetSize() const;
		Float3	
			GetHalfSize() const;
		Float3	
			GetCenter() const;
		void	
			GetPoints(Float3 * points) const;

		bool	
			Valid() const;
		float   
			Volume() const;

		void	
			Transform(const Mat4 & mat);

		void	
			Merge(const Aabb & aabb);
		void	
			Merge(const Float3 & point);

		bool	
			Contain(const Float3 & point) const;
		bool    
			Contain(const RectF & rect) const;
		bool    
			Contain(const Aabb & aabb) const;
		bool	
			Contain(const Sphere & sph) const;

		bool    
			Intersect(const RectF & rect) const;
		bool    
			Intersect(const Aabb & aabb) const;
		bool    
			Intersect(const Sphere & sph) const;
		Aabb    
			GetIntersectAabb(const Aabb & aabb) const;

	public:
		Float3 minimum;
		Float3 maximum;
	};

}

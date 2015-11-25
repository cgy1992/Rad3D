#include "MRay.h"
#include "MMath.h"
#include "MPlane.h"
#include "MSphere.h"
#include "MAabb.h"
#include "MObb.h"

namespace Rad {

	Ray::Ray()
	{
	}

	Ray::Ray(const Float3& vOrigin, const Float3& vDirection)
		: orig(vOrigin)
		, dir(vDirection)
	{
	}

	Ray::~Ray()
	{
	}

	bool Ray::Intersect(float * _dist, const Plane & rk) const
	{
		float dist;
		float ddn = dir.Dot(rk.normal);

		if (Math::Abs(ddn) < EPSILON_E5)
		{
			dist = 0;
		}
		else
		{
			float dt = rk.Distance(orig);
			float t = -(dt / ddn);

			dist = t;
		}

		if (_dist)
			*_dist = dist;

		return dist > 0;
	}

	bool Ray::Intersect(float * _dist, const Sphere & rk) const
	{
		float dist;
		const Float3 po = orig - rk.center;
		float radiusSq = rk.radius * rk.radius;
		float pol = po.LengthSq();

		if (pol < radiusSq)
		{
			dist = 0;
		}
		else
		{
			// In 2 dimension
			// solve this equations set
			//
			// (x - o)^2 = r * r
			// (p + dt) = x;
			//
			// ==>
			//
			// (dt + po)^2 = r * r
			//
			float a = dir.Dot(dir);
			float b = 2.0f * dir.Dot(po);
			float c = pol - radiusSq;

			// b^2 - 4ac
			float delta = b * b - 4.0f * a * c;
			if (delta >= 0)
			{
				// t = (-b +/- sqrt(detla) / 2a
				float sqrtd = Math::Sqrt(delta);
				float inv2a = 0.5f / a;
				float t1 = (-b - sqrtd) * inv2a;
				float t2 = (-b + sqrtd) * inv2a;

				dist = Min(t1, t2);
			}
			else
			{
				return false;
			}
		}

		if (_dist)
			*_dist = dist;

		return true;
	}

	bool Ray::Intersect(float * _dist, const Aabb & rk) const
	{
		float dist;
		bool hit = FALSE;

		if (orig > rk.minimum && orig < rk.maximum)
		{
			//inside
			dist = 0;
			hit = TRUE;
		}
		else
		{
			 //outside
			float t;
			float lowt = 0.0f;
			Float3 hitpos;

			//check each face

			//minimum x
			if (orig.x <= rk.minimum.x && dir.x > 0.0f)
			{
				t = (rk.minimum.x - orig.x) / dir.x;
				if (t >= 0)
				{
					hitpos = orig + dir * t;

					if (hitpos.y >= rk.minimum.y && hitpos.y <= rk.maximum.y &&
						hitpos.z >= rk.minimum.z && hitpos.z <= rk.maximum.z &&
						(!hit || t < lowt))
					{
						hit = TRUE;
						lowt = t;
					}
				}
			}

			//maximum x
			if (orig.x >= rk.maximum.x && dir.x < 0.0f)
			{
				t = (rk.maximum.x - orig.x) / dir.x;
				if (t >= 0)
				{
					hitpos = orig + dir * t;

					if (hitpos.y >= rk.minimum.y && hitpos.y <= rk.maximum.y &&
						hitpos.z >= rk.minimum.z && hitpos.z <= rk.maximum.z &&
						(!hit || t < lowt))
					{
						hit = TRUE;
						lowt = t;
					}
				}
			}

			//minimum y
			if (orig.y <= rk.minimum.y && dir.y > 0.0f)
			{
				t = (rk.minimum.y - orig.y) / dir.y;
				if (t >= 0)
				{
					hitpos = orig + dir * t;

					if (hitpos.x >= rk.minimum.x && hitpos.x <= rk.maximum.x &&
						hitpos.z >= rk.minimum.z && hitpos.z <= rk.maximum.z &&
						(!hit || t < lowt))
					{
						hit = TRUE;
						lowt = t;
					}
				}
			}

			//maximum y
			if (orig.y >= rk.maximum.y && dir.y < 0.0f)
			{
				t = (rk.maximum.y - orig.y) / dir.y;
				if (t >= 0)
				{
					hitpos = orig + dir * t;

					if (hitpos.x >= rk.minimum.x && hitpos.x <= rk.maximum.x &&
						hitpos.z >= rk.minimum.z && hitpos.z <= rk.maximum.z &&
						(!hit || t < lowt))
					{
						hit = TRUE;
						lowt = t;
					}
				}
			}

			//minimum z
			if (orig.z <= rk.minimum.z && dir.z > 0.0f)
			{
				t = (rk.minimum.z - orig.z) / dir.z;
				if (t >= 0)
				{
					hitpos = orig + dir * t;

					if (hitpos.x >= rk.minimum.x && hitpos.x <= rk.maximum.x &&
						hitpos.y >= rk.minimum.y && hitpos.y <= rk.maximum.y &&
						(!hit || t < lowt))
					{
						hit = TRUE;
						lowt = t;
					}
				}
			}

			//maximum z
			if (orig.z >= rk.maximum.z && dir.z < 0.0f)
			{
				t = (rk.maximum.z - orig.z) / dir.z;
				if (t >= 0)
				{
					hitpos = orig + dir * t;

					if (hitpos.x >= rk.minimum.x && hitpos.x <= rk.maximum.x &&
						hitpos.y >= rk.minimum.y && hitpos.y <= rk.maximum.y &&
						(!hit || t < lowt))
					{
						hit = TRUE;
						lowt = t;
					}
				}
			}

			dist = lowt;
		}

		if (_dist)
			*_dist = dist;

		return hit;
	}

	bool Ray::Intersect(float * _dist, const Obb & obb) const
	{
		Mat4 form = obb.GetWorldToLocalTM();

		Ray ray = *this;
		ray.orig.TransformA(form);
		ray.dir.TransformN(form);

		return ray.Intersect(_dist, Aabb(-obb.extent, obb.extent));
	}

	bool Ray::Intersect(float * _dist, float & tu, float & tv, const Float3 & a, const Float3 & b, const Float3 & c) const
	{
		/*
        p = o + dt;
        p = sa + qb + rc;
        s + q + r = 1;

        ==>

        a + (b-a)q + (c-a)r = o + dt

        ==>

        -dt + (b-a)q + (c-a)r = o - a

        ==>
        
                            | t |   
        |-d, b - a, c - a|  | q | = | o - a |
                            | r |

        ==>

               |o - a, b - a, c - a|
        t = --------------------------
                |-d, b - a, c - a|

                |-d, o - a, c - a|
        q = --------------------------
                |-d, b - a, c - a|

                |-d, b - a, o - a|
        r = --------------------------
                |-d, b - a, c - a|
		*/

		float dist = FLT_MAX;

		Float3 ab = b - a;
		Float3 ac = c - a;

		Float3 cross = Float3::Cross(dir, ac);
		float det = ab.Dot(cross);

		Float3 ray2a = orig - a;
		
		if (det < 0)
		{
			ray2a = a - orig;
			det = -det;
		}

		//parallel
		if (det < EPSILON_E4)
			return false;

		//q
		float q = ray2a.Dot(cross);

		if (q < 0.0f || q > det)
			return false;

		//r
		cross = Float3::Cross(ray2a, ab);
		float r = dir.Dot(cross);

		if (r < 0.0f || r + q > det)
			return false;

		//t
		float t = ac.Dot(cross);

		det = 1.0f / det;
		dist = t * det;
		tu = q * det;
		tv = r * det;

		if (_dist)
			*_dist = dist;

		return dist >= 0;
	}

	bool Ray::Intersect(float * _dist, const Float3 & a, const Float3 & b, const Float3 & c) const
	{
		float tu, tv;
		return Intersect(_dist, tu, tv, a, b, c);
	}

	bool Ray::Intersect(float * _dist, const Float3 * vtx, const short * idx, int numTris, const Mat4 & form) const
	{
		bool rb = false;
		float dt = FLT_MAX; 
		float dist = FLT_MAX;

		if (form == Mat4::Identity)
		{
			for (int i = 0; i < numTris * 3; i += 3)
			{
				const Float3 & a = vtx[idx[i + 0]];
				const Float3 & b = vtx[idx[i + 1]];
				const Float3 & c = vtx[idx[i + 2]];

				if (Intersect(&dt, a, b, c))
				{
					dist = Min(dt, dist);
					rb = true;
				}
			}
		}
		else
		{
			for (int i = 0; i < numTris * 3; i += 3)
			{
				Float3 a = vtx[idx[i + 0]];
				Float3 b = vtx[idx[i + 1]];
				Float3 c = vtx[idx[i + 2]];

				a.TransformA(form);
				b.TransformA(form);
				c.TransformA(form);

				if (Intersect(&dt, a, b, c))
				{
					dist = Min(dt, dist);
					rb = true;
				}
			}
		}

		if (_dist)
			*_dist = dist;

		return rb;
	}

}
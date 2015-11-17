#include "MQuat.h"
#include "MMathASM.h"

namespace Rad {

	inline void QuatMultiply(Quat & q, const Quat & q1, const Quat & q2)
	{
#ifdef SIMD_QuatMul
		SIMD_QuatMul(&q, &q1, &q2);
#else
		q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
		q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
		q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
		q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
#endif
	}


	const Quat Quat::Zero = Quat(0, 0, 0, 0);
	const Quat Quat::Identity = Quat(0, 0, 0, 1);

	Quat::Quat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{
	}

	Quat::Quat(const Quat &q) : x(q.x), y(q.y), z(q.z), w(q.w)
	{
	}

	Quat::Quat(const Float3 & axis, float rads)
	{
		FromAxis(axis, rads);
	}

	Quat::Quat()
		: x(0), y(0), z(0), w(1)
	{
	}

	Quat::~Quat()
	{
	}

	Quat & Quat::operator =(const Quat & q)
	{
		x = q.x, y = q.y, z = q.z, w = q.w;
		return *this;
	}

	Quat Quat::operator -() const
	{
		return Quat(-x, -y, -z, w);
	}

	Quat Quat::operator +(const Quat & q) const
	{
		return Quat(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	Quat Quat::operator -(const Quat & q) const
	{
		return Quat(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	Quat Quat::operator *(const Quat & q2) const
	{
		Quat q;
		QuatMultiply(q, *this, q2);
		return q;
	}

	Quat Quat::operator *(float v) const
	{
		return Quat(x * v, y * v, z * v, w * v);
	}

	Quat & Quat::operator +=(const Quat & q)
	{
		x += q.x, y += q.y, z += q.z, w += q.w;
		return *this;
	}

	Quat & Quat::operator -=(const Quat & q)
	{
		x -= q.x, y -= q.y, z -= q.z, w -= q.w;
		return *this;
	}

	Quat & Quat::operator *=(const Quat & q2)
	{
		*this = *this * q2;
		return *this;
	}

	bool Quat::operator ==(const Quat & q) const
	{
		return x == q.x && y == q.y && z == q.z && w == q.w;
	}

	bool Quat::operator !=(const Quat & q) const
	{
		return x != q.x || y != q.y || z != q.z || w != q.w;
	}

	float Quat::Dot(const Quat & rk) const
	{
		return x * rk.x + y * rk.y + z * rk.z + w * rk.w;
	}

	float Quat::Length() const
	{
		return Math::Sqrt(x * x + y * y + z * z + w * w);
	}

	void Quat::Inverse()
	{
		float len2 = x * x + y * y + z * z + w * w;

		d_assert (len2 > 0);

		len2 = 1.0f / len2;

		x = -x * len2;
		y = -y * len2;
		z = -z * len2;
		w = w * len2;
	}

	void Quat::Normalize()
	{
		float len = Length();

		d_assert (len != 0);

		if (len > 0)
		{
			float inv_len = 1.0f / len;
			x *= inv_len;
			y *= inv_len;
			z *= inv_len;
			w *= inv_len;
		}
	}

	void Quat::Conjugate()
	{
		x = -x; y = -y; z = -z;
	}

	void Quat::FromAxis(const Float3 & vAxis, float rads)
	{
		float half_rads = rads / 2.0f;
		float cosine = Math::FastCos(half_rads);
		float sine = Math::FastSin(half_rads);

		x = vAxis.x * sine;
		y = vAxis.y * sine;
		z = vAxis.z * sine;
		w = cosine;
	}

	void Quat::FromAxisFast(const Float3 & vAxis, float rads)
	{
		float half_rads = rads / 2.0f;
		float cosine = Math::FastCos(half_rads);
		float sine = Math::FastSin(half_rads);

		x = vAxis.x * sine;
		y = vAxis.y * sine;
		z = vAxis.z * sine;
		w = cosine;
	}

	void Quat::FromAxis(const Float3 & xAxis, const Float3 & yAxis, const Float3 & zAxis)
	{
		Mat4 m;

		m._11 = xAxis.x, m._12 = xAxis.y, m._13 = xAxis.z;
		m._21 = yAxis.x, m._22 = yAxis.y, m._23 = yAxis.z;
		m._31 = zAxis.x, m._32 = zAxis.y, m._33 = zAxis.z;

		FromMatrix(m);
	}

	void Quat::FromDir(const Float3 & dir1, const Float3 & dir2, const Float3 & fallbackAxis)
	{
		Float3 d1 = dir1, d2 = dir2;

		float d = d1.Dot(d2);

		if (d >= 1.0f)
		{
			*this = Quat::Identity;
		}
		else if (d < (1e-6f - 1.0f))
		{
			if (fallbackAxis != Float3::Zero)
			{
				// rotate 180 degrees about the fallback axis
				FromAxis(fallbackAxis, PI);
			}
			else
			{
				// Generate an axis
				Float3 axis = Float3::Cross(Float3(1, 0, 0), d1);

				if (axis.LengthSq() < DEFAULT_EPSILON) // pick another if colinear
				{
					axis = Float3::Cross(Float3(0, 1, 0), d1);
				}

				axis.Normalize();

				FromAxis(axis, PI);
			}
		}
		else
		{
			float s = Math::Sqrt((1 + d) * 2);
			float invs = 1 / s;

			Float3 c = Float3::Cross(d1, d2);

			x = c.x * invs;
			y = c.y * invs;
			z = c.z * invs;
			w = s * 0.5f;

			Normalize();
		}
	}

	void Quat::FromMatrix(const Mat4 & m)
	{
		Quat q;
		float s = 0.0f;
		float t = 0.0f;

		// check the diagonal
		if(m._11 + m._22 + m._33 > 0.0)
		{
			s = 0.5f / Math::Sqrt(1.0f + m._11 + m._22 + m._33);

			q.w = 0.25f / s;
			q.x = (m._23 - m._32) * s;
			q.y = (m._31 - m._13) * s;
			q.z = (m._12 - m._21) * s;
		}

		// column 1
		else if(m._11 > m._22 && m._11 > m._33)
		{
			s = 2.0f * Math::Sqrt(1.0f + m._11 - m._22 - m._33);

			q.x = 0.25f * s;
			q.y = (m._12 + m._21) / s;
			q.z = (m._13 + m._31) / s;
			q.w = (m._23 - m._32) / s;
		}

		// column 2
		else if( m._22 > m._33)
		{
			s = 2.0f * Math::Sqrt(1.0f + m._22 - m._11 - m._33);

			q.y = 0.25f * s;
			q.x = (m._21 + m._12) / s;
			q.z = (m._32 + m._23) / s;
			q.w = (m._31 - m._13) / s;
		}

		// column 3
		else
		{
			s = 2.0f * Math::Sqrt(1.0f + m._33 - m._11 - m._22);

			q.z = 0.25f * s;
			q.x = (m._31 + m._13) / s;
			q.y = (m._32 + m._23) / s;
			q.w = (m._12 - m._21) / s;
		}

		*this = q;
	}

	Quat Quat::Slerp(const Quat & a, const Quat & b, float t)
	{
		const Quat & q1 = a; const Quat & q2 = b;
		Quat q2_t;

		float cosine = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
		if (cosine < 0.0f)
		{
			cosine = -cosine;
			q2_t.x = -q2.x;
			q2_t.y = -q2.y;
			q2_t.z = -q2.z;
			q2_t.w = -q2.w;
		}
		else
		{
			q2_t = q2;
		}

		float t0, t1;
		if (1.0f - cosine > EPSILON_E4)
		{
			float theta = Math::FastACos(cosine);
			float inv_sine = 1.0f / Math::FastSin(theta);
			t0 = Math::FastSin((1.0f - t) * theta) * inv_sine;
			t1 = Math::FastSin(t * theta) * inv_sine;
		}
		else
		{
			t0 = 1.0f - t;
			t1 = t;
		}

		return q1 * t0 + q2_t * t1;
	}

	Quat Quat::Lerp(const Quat & a, const Quat & b, float t)
	{
		return a * (1 - t) + b * t;
	}

	void Quat::ToAxis(Float3 & axis, float & rads) const
	{
		rads = Math::FastACos(w);

		float sin_theta_inv = 1.0f / Math::Sin(rads);

		axis.x = x * sin_theta_inv;
		axis.y = y * sin_theta_inv;
		axis.z = z * sin_theta_inv;

		rads *= 2;
	}

	Mat4 Quat::ToMatrix() const
	{
		/*
			Left Hand
		*/
		Mat4 m;

		m._11 = 1.0f - 2.0f * (y * y + z * z);
		m._12 = 2.0f * (x * y + w * z);
		m._13 = 2.0f * (x * z - w * y);
		m._14 = 0.0f;

		m._21 = 2.0f * (x * y - w * z); 
		m._22 = 1.0f - 2.0f * (x * x + z * z);
		m._23 = 2.0f * (y * z + w * x);
		m._24 = 0.0f;

		m._31 = 2.0f * (w * y + x * z);
		m._32 = 2.0f * (y * z - w * x);
		m._33 = 1.0f - 2.0f * (x * x + y * y);
		m._34 = 0.0f;

		m._41 = 0.0f;
		m._42 = 0.0f;
		m._43 = 0.0f;
		m._44 = 1.0f;

		return m;
	}

	Float3 Quat::GetRightVector() const
	{
		Float3 xAxis;

		xAxis.x = 1.0f - 2.0f * (y * y + z * z);
		xAxis.y = 2.0f * (x * y + w * z);
		xAxis.z = 2.0f * (x * z - w * y);

		return xAxis;
	}

	Float3 Quat::GetUpVector() const
	{
		Float3 yAxis;

		yAxis.x = 2.0f * (x * y - w * z); 
		yAxis.y = 1.0f - 2.0f * (x * x + z * z);
		yAxis.z = 2.0f * (y * z + w * x);

		return yAxis;
	}

	Float3 Quat::GetDirVector() const
	{
		Float3 zAxis;

		zAxis.x = 2.0f * (w * y + x * z);
		zAxis.y = 2.0f * (y * z - w * x);
		zAxis.z = 1.0f - 2.0f * (x * x + y * y);

		return zAxis;
	}

	void Quat::ToAxis(Float3 & xAxis, Float3 & yAxis, Float3 & zAxis) const
	{
		xAxis.x = 1.0f - 2.0f * (y * y + z * z);
		xAxis.y = 2.0f * (x * y + w * z);
		xAxis.z = 2.0f * (x * z - w * y);

		yAxis.x = 2.0f * (x * y - w * z); 
		yAxis.y = 1.0f - 2.0f * (x * x + z * z);
		yAxis.z = 2.0f * (y * z + w * x);

		zAxis.x = 2.0f * (w * y + x * z);
		zAxis.y = 2.0f * (y * z - w * x);
		zAxis.z = 1.0f - 2.0f * (x * x + y * y);
	}

	void Quat::FromEulerAngle(const Float3 & angle)
	{
		float half_x = Math::DegreeToRadian(angle.x) * 0.5f;
		float sin0 = Math::FastSin(half_x);
		float cos0 = Math::FastCos(half_x);

		float half_y = Math::DegreeToRadian(angle.y) * 0.5f;
		float sin1 = Math::FastSin(half_y);
		float cos1 = Math::FastCos(half_y);

		float half_z = Math::DegreeToRadian(angle.z) * 0.5f;
		float sin2 = Math::FastSin(half_z);
		float cos2 = Math::FastCos(half_z);

		w = cos0 * cos1 * cos2 + sin0 * sin1 * sin2;
		x = sin0 * cos1 * cos2 - cos0 * sin1 * sin2;
		y = cos0 * sin1 * cos2 + sin0 * cos1 * sin2;
		z = cos0 * cos1 * sin2 - sin0 * sin1 * cos2;
	}

	Float3 Quat::ToEulerAngle() const
	{
		Float3 angle;

		angle.x = Math::ATan2(2*(y*z + w*x), w*w - x*x - y*y + z*z);
		angle.y = Math::ASin(-2*(x*z - w*y));
		angle.z = Math::ATan2(2*(x*y + w*z), w*w + x*x - y*y - z*z);
		
		angle.x = Math::RadianToDegree(angle.x);
		angle.y = Math::RadianToDegree(angle.y);
		angle.z = Math::RadianToDegree(angle.z);

		return angle;
	}

}
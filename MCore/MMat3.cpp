#include "MMat3.h"
#include "MFloat2.h"
#include "MMat4.h"

namespace Rad {

	const Mat3 Mat3::Zero = Mat3(0, 0, 0,
								 0, 0, 0,
								 0, 0, 0);

	const Mat3 Mat3::Identity = Mat3(1, 0, 0,
									 0, 1, 0,
									 0, 0, 1);

	Mat3::Mat3()
	{
	}

	Mat3::~Mat3()
	{
	}

	Mat3::Mat3(float m11, float m12, float m13, 
			   float m21, float m22, float m23, 
			   float m31, float m32, float m33) 
		: _11(m11), _12(m12), _13(m13)
		, _21(m21), _22(m22), _23(m23)
		, _31(m31), _32(m32), _33(m33)
	{
	}

	Mat3::Mat3(const float * p)
	{
		memcpy(this, p, sizeof(float) * 9);
	}

	Mat3::Mat3(const Mat3 & m)
	{
		*this = m;
	}

	Mat3& Mat3::operator =(const Mat3 & m)
	{
		_11 = m._11, _12 = m._12, _13 = m._13;
		_21 = m._21, _22 = m._22, _23 = m._23;
		_31 = m._31, _32 = m._32, _33 = m._33;
		return (*this);
	}

	Mat3 Mat3::operator -() const
	{
		return Mat3(
			-_11, -_12, -_13,
			-_21, -_22, -_23,
			-_31, -_32, -_33);
	}

	Mat3 Mat3::operator +(const Mat3 & mat) const
	{
		return Mat3(
			_11 + mat._11, _12 + mat._12, _13 + mat._13,
			_21 + mat._21, _22 + mat._22, _23 + mat._23,
			_31 + mat._31, _32 + mat._32, _33 + mat._33);
	}

	Mat3 Mat3::operator -(const Mat3 & mat) const
	{
		return Mat3(
			_11 - mat._11, _12 - mat._12, _13 - mat._13,
			_21 - mat._21, _22 - mat._22, _23 - mat._23,
			_31 - mat._31, _32 - mat._32, _33 - mat._33);
	}

	Mat3 Mat3::operator *(const Mat3 & m2) const
	{
		Mat3 m, m1 = *this;

		m._11 = m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31;
		m._12 = m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32;
		m._13 = m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33;

		m._21 = m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31;
		m._22 = m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32;
		m._23 = m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33;

		m._31 = m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31;
		m._32 = m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32;
		m._33 = m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33;

		return m;
	}

	Mat3 Mat3::operator *(float v) const
	{
		return Mat3(
			_11 * v, _12 * v, _13 * v,
			_21 * v, _22 * v, _23 * v,
			_31 * v, _32 * v, _33 * v);
	}

	Mat3 Mat3::operator /(float v) const
	{
		d_assert (v != 0);
		return ((*this)) * (1 / v);
	}

	Mat3& Mat3::operator +=(const Mat3 & mat)
	{
		_11 += mat._11, _12 += mat._12, _13 += mat._13;
		_21 += mat._21, _22 += mat._22, _23 += mat._23;
		_31 += mat._31, _32 += mat._32, _33 += mat._33;

		return (*this);
	}

	Mat3& Mat3::operator -=(const Mat3 & mat)
	{
		_11 -= mat._11, _12 -= mat._12, _13 -= mat._13;
		_21 -= mat._21, _22 -= mat._22, _23 -= mat._23;
		_31 -= mat._31, _32 -= mat._32, _33 -= mat._33;

		return (*this);
	}

	Mat3& Mat3::operator *=(const Mat3 & mat)
	{
		(*this) = ((*this)) * mat;
		return (*this);
	}

	Mat3& Mat3::operator *=(float v)
	{
		_11 *= v, _12 *= v, _13 *= v;
		_21 *= v, _22 *= v, _23 *= v;
		_31 *= v, _32 *= v, _33 *= v;
		return (*this);
	}

	Mat3& Mat3::operator /=(float v)
	{
		d_assert (v != 0);
		return ((*this)) *= (1 / v);
	}

	bool Mat3::operator ==(const Mat3 & m) const
	{
		return 
			_11 == m._11 && _12 == m._12 && _13 == m._13 &&
			_21 == m._21 && _22 == m._22 && _23 == m._23 &&
			_31 == m._31 && _32 == m._32 && _33 == m._33;
	}

	bool Mat3::operator !=(const Mat3 & m) const
	{
		return 
			_11 != m._11 || _12 != m._12 || _13 != m._13 ||
			_21 != m._21 || _22 != m._22 || _23 != m._23 ||
			_31 != m._31 || _32 != m._32 || _33 != m._33;
	}

	float* Mat3::operator [](int index)
	{
		d_assert(index < 3);
		return m[index];
	}

	const float* Mat3::operator [](int index) const
	{
		d_assert(index < 3);
		return m[index];
	}

	float Mat3::Minor(int r0, int r1, int c0, int c1) const
	{
		return m[r0][c0] * m[r1][c1] - m[r1][c0] * m[r0][c1];
	}

	float Mat3::Det() const
	{
		Mat4 m4(*this);

		return m4.Minor(1, 2, 3, 1, 2, 3);
	}

	void Mat3::Transpose()
	{
		Mat3 m;

		m._11 = _11, m._12 = _21, m._13 = _31;
		m._21 = _12, m._22 = _22, m._23 = _32;
		m._31 = _13, m._32 = _23, m._33 = _33;

		(*this) = m;
	}

	void Mat3::Inverse()
	{
		Mat4 m4(*this);
		m4.Inverse();

		_11 = m4._11, _12 = m4._12, _13 = m4._13;
		_21 = m4._21, _22 = m4._22, _23 = m4._22;
		_31 = m4._31, _32 = m4._32, _33 = m4._33;
	}

	void Mat3::MakeTransform(const Float2 & trans, const float rotatoin, const Float2 & scale)
	{
		Mat3 mRotate;
		mRotate.MakeRotation(rotatoin);

		_11 = mRotate._11 * scale.x;
		_12 = mRotate._12 * scale.y;

		_21 = mRotate._21 * scale.x;
		_22 = mRotate._22 * scale.y;

		_31 = trans.x; _32 = trans.y;

		_13 = 0; _23 = 0; _33 = 1;
	}

	void Mat3::MakeTranslate(float x, float y)
	{
		(*this) = Mat3::Identity;
		_31 = x;
		_32 = y;
	}

	void Mat3::MakeRotation(float rads)
	{
		float sine = Math::FastSin(rads);
		float cosine = Math::FastCos(rads);

		(*this) = Mat3::Identity;
		_11 = cosine;
		_21 = -sine;
		_12 = sine;
		_22 = cosine;
	}

	void Mat3::MakeScale(float x, float y)
	{
		(*this) = Mat3::Identity;
		_11 = x;
		_22 = y;
	}

}
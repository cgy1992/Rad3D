/*
*	Mat3
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"

namespace Rad {

	class Float2;

	class M_ENTRY Mat3
	{
		DECLARE_ALLOC();

	public:
		static const Mat3 Identity;
		static const Mat3 Zero;

	public:
		union 
		{
			struct 
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			float m[3][3];
			float m9[9];
		};

	public:
		Mat3();
		~Mat3();

		Mat3(const float * p);
		Mat3(const Mat3 & m);
		Mat3(float m11, float m12, float m13,
			 float m21, float m22, float m23,
			 float m31, float m32, float m33);

		Mat3& 
			operator =(const Mat3 & mat);

		Mat3 
			operator -() const;
		Mat3 
			operator +(const Mat3 & mat) const; 
		Mat3 
			operator -(const Mat3 & mat) const; 
		Mat3 
			operator *(const Mat3 & mat) const; 

		Mat3&
			operator +=(const Mat3 & mat);
		Mat3& 
			operator -=(const Mat3 & mat);
		Mat3& 
			operator *=(const Mat3 & mat);

		Mat3 
			operator *(float v) const;
		Mat3 
			operator /(float v) const;

		Mat3& 
			operator *=(float v);
		Mat3& 
			operator /=(float v);

		bool 
			operator ==(const Mat3 & mat) const;
		bool 
			operator !=(const Mat3 & mat) const;

		float* 
			operator [](int index);
		const float* 
			operator [](int index) const;

	public:
		const float * 
			ToFloatPtr() const;

		float 
			Minor(int r0, int r1, int c0, int c1) const;
		float 
			Det() const;

		void 
			Transpose();
		void 
			Inverse();

		void 
			MakeTransform(const Float2 & trans, float rotation, const Float2 & scale);
		void 
			MakeTranslate(float x, float y);
		void 
			MakeRotation(float rads);
		void 
			MakeScale(float x, float y);
	};

}
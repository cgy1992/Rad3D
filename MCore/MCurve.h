/*
*	Curve
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat3.h"

namespace Rad {

	template <class T>
	class LinearCurve
	{
		DECLARE_ALLOC();

	public:
		LinearCurve() 
		{
			mNumPoints = 100;
			mPoints = new T[mNumPoints];
		}

		LinearCurve(int count)
		{
			mNumPoints = count;
			mPoints = new T[mNumPoints];
		}

		~LinearCurve()
		{
			safe_delete_array (mPoints);
		}

		void SetPointCount(int count)
		{
			d_assert (count > 0);

			if (mNumPoints != count)
			{
				mNumPoints = count;
				safe_delete_array (mPoints);

				mPoints = new T[count];
			}
		}

		int GetPointCount() const
		{ 
			return mNumPoints;
		}

		void SetPoint(int index, const T & p)
		{
			d_assert (index < mNumPoints);

			mPoints[index] = p;
		}

		const T & GetPoint(int index)
		{
			d_assert (index < mNumPoints);

			return mPoints[index];
		}

		T GetPointLerp(float t)
		{
			if (t <= 0)
				return GetPoint(0);

			if (t >= 1)
				return GetPoint(mNumPoints - 1);

			float findex = t * (mNumPoints - 1);

			int index0 = (int)findex;
			int index1 = index0 + 1;
			float k = findex - index0;

			const T & p0 = GetPoint(index0);
			const T & p1 = GetPoint(index1);

			return p0 + (p1 - p0) * k;
		}

		void Smooth(int step)
		{
			T * points = new T[mNumPoints];

			for (int i = 0; i < mNumPoints; ++i)
			{
				int count = 1;
				T sum = mPoints[i];

				int start = (i - step > 0) ? i - step : 0;
				int end = (i + step < mNumPoints) ? i + step : mNumPoints;

				for (int j = start; j < i; ++j)
					sum += mPoints[j], count++;

				for (int j = i + 1; j < end; ++j)
					sum += mPoints[j], count++;

				points[i] = sum / count;
			}

			for (int i = 0; i < mNumPoints; ++i)
				mPoints[i] = points[i];

			delete points;
		}

		LinearCurve(const Curve & rk)
		{
			mPoints = NULL;

			*this = rk;
		}

		LinearCurve & operator = (const Curve & rk)
		{
			d_assert (this != &rk);

			safe_delete_array (mPoints);

			mPoints = new T[rk.mNumPoints];

			for (int i = 0; i < mNumPoints; ++i)
				mPoints[i] = rk.mPoints[i];
		}

	protected:
		int mNumPoints;
		T * mPoints;
	};

	//
	template <class T>
	class BezierCurve
	{
	public:
		BezierCurve()
		{
		}

		~BezierCurve()
		{
		}

		void SetControl(int index, const T & control)
		{
			mControl[index] = control;
		}

		const T & GetControl(int index) const
		{
			d_assert (index >= 0 && index < 4);

			return mControl[index];
		}

		T GetPoint(float t)
		{
			if (t < 0)
				t = 0;
			else if (t > 1)
				t = 1;

			return mControl[0] * (1 - t) * (1 - t) * (1 - t) + 
				mControl[1] * 3 * (1 - t) * (1 - t) * t +
				mControl[2] * 3 * (1 - t) * t * t +
				mControl[3] * t * t * t;
		}

		void ToCurve(Curve<T> & curve)
		{
			int numPoints = curve.GetPointCount();

			for (int i = 0; i < numPoints; ++i)
			{
				float t = i / (float)(numPoints - 1);

				curve.SetPoint(i, GetPoint(t));
			}
		}

	protected:
		T mControl[4];
	};

}


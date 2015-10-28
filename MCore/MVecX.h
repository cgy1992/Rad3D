/*
*	VecX
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"

namespace Rad {

	template <class T, int num>
	struct VecX
	{
	public:
		VecX();
		VecX(const T * mem);
		VecX(const VecX & v);
		~VecX();

		VecX& operator = (const VecX & v);
		operator T * ();
		operator const T * () const;

		bool operator ==(const VecX & v) const;
		bool operator !=(const VecX & v) const;
		VecX operator -() const;
		VecX operator + (const VecX & v) const;
		VecX operator - (const VecX & v) const;
		VecX operator * (const VecX & v) const;
		VecX operator / (const VecX & v) const;
		VecX operator * (T i) const;
		VecX operator / (T i) const;

		VecX& operator += (const VecX & v);
		VecX& operator -= (const VecX & v);
		VecX& operator *= (T i);
		VecX& operator /= (T i);

		T& operator [](int index);
		const T& operator [](int index) const;

	public:
		T m[num];
	};

	template<int num>
	struct IntX : public VecX<int, num> {};
	
	template<int num>
	struct FloatX : public VecX<float, num> {};

	//
	template <class T, int num>
	inline VecX<T, num>::VecX()
	{
	}

	template <class T, int num>
	inline VecX<T, num>::VecX(const T * mem)
	{
		for (int i = 0; i < num; ++i)
		{
			m[i] = mem[i];
		}
	}

	template <class T, int num>
	inline VecX<T, num>::VecX(const VecX<T, num> & v)
	{
		for (int i = 0; i < num; ++i)
		{
			m[i] = v.m[i];
		}
	}

	template <class T, int num>
	inline VecX<T, num>::~VecX()
	{
	}

	template <class T, int num>
	inline VecX<T, num>& VecX<T, num>::operator = (const VecX<T, num> & v)
	{
		for (int i = 0; i < num; ++i)
		{
			m[i] = v.m[i];
		}

		return *this;
	}

	template <class T, int num>
	inline VecX<T, num>::operator T* ()
	{
		return m;
	}

	template <class T, int num>
	inline VecX<T, num>::operator const T* () const
	{
		return m;
	}

	template <class T, int num>
	inline bool VecX<T, num>::operator ==(const VecX<T, num> & v) const
	{
		for (int i = 0; i < num; ++i)
		{
			if (m[i] != v.m[i])
				return false;
		}

		return true;
	}

	template <class T, int num>
	inline bool VecX<T, num>::operator !=(const VecX<T, num> & v) const
	{
		for (int i = 0; i < num; ++i)
		{
			if (m[i] == v.m[i])
				return false;
		}

		return true;
	}

	template <class T, int num>
	inline VecX<T, num> VecX<T, num>::operator -() const
	{
		VecX<T, num> v;

		for (int i = 0; i < num; ++i)
			v.m[i] = -m[i];

		return v;
	}

	template <class T, int num>
	inline VecX<T, num> VecX<T, num>::operator + (const VecX<T, num> & v) const
	{
		VecX ret;

		for (int i = 0; i < num; ++i)
			ret.m[i] = m[i] + v.m[i];

		return ret;
	}

	template <class T, int num>
	inline VecX<T, num> VecX<T, num>::operator - (const VecX<T, num> & v) const
	{
		VecX<T, num> ret;

		for (int i = 0; i < num; ++i)
			ret.m[i] = m[i] - v.m[i];

		return ret;
	}

	template <class T, int num>
	inline VecX<T, num> VecX<T, num>::operator * (const VecX<T, num> & v) const
	{
		VecX<T, num> ret;

		for (int i = 0; i < num; ++i)
			ret.m[i] = m[i] * v.m[i];

		return ret;
	}

	template <class T, int num>
	inline VecX<T, num> VecX<T, num>::operator / (const VecX<T, num> & v) const
	{
		VecX<T, num> ret;

		for (int i = 0; i < num; ++i)
			ret.m[i] = m[i] / v.m[i];

		return ret;
	}

	template <class T, int num>
	inline VecX<T, num> VecX<T, num>::operator * (T i) const
	{
		VecX<T, num> ret;

		for (int i = 0; i < num; ++i)
			ret.m[i] = m[i] * i;

		return ret;
	}

	template <class T, int num>
	inline VecX<T, num> VecX<T, num>::operator / (T i) const
	{
		VecX<T, num> ret;

		for (int i = 0; i < num; ++i)
			ret.m[i] = m[i] / i;

		return ret;
	}

	template <class T, int num>
	inline VecX<T, num>& VecX<T, num>::operator += (const VecX<T, num> & v)
	{
		for (int i = 0; i < num; ++i)
			m[i] += v.m[i];

		return *this;
	}

	template <class T, int num>
	inline VecX<T, num>& VecX<T, num>::operator -= (const VecX<T, num> & v)
	{
		for (int i = 0; i < num; ++i)
			m[i] -= v.m[i];

		return *this;
	}

	template <class T, int num>
	inline VecX<T, num>& VecX<T, num>::operator *= (T v)
	{
		for (int i = 0; i < num; ++i)
			m[i] *= v;

		return *this;
	}

	template <class T, int num>
	inline VecX<T, num>& VecX<T, num>::operator /= (T v)
	{
		for (int i = 0; i < num; ++i)
			m[i] /= v;

		return *this;
	}

	template <class T, int num>
	inline T& VecX<T, num>::operator [](int index)
	{
		return m[index];
	}

	template <class T, int num>
	inline const T& VecX<T, num>::operator [](int index) const
	{
		return m[index];
	}

}
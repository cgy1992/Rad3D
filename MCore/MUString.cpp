#include "MUString.h"

namespace Rad {

	UString::UString(const UString & _str)
	{
		mSize = _str.mSize;
		mStr = (uchar_t *)pool_alloc(sizeof(uchar_t) * mSize);
		u_strcpy(mStr, _str.c_str());
	}

	UString::UString(const uchar_t * _str)
	{
		d_assert (_str != NULL);

		mSize = u_strlen(_str) + 1;
		mStr = (uchar_t *)pool_alloc(sizeof(uchar_t) * mSize);
		u_strcpy(mStr, _str);
	}

	UString::UString()
	{
		mSize = 16;
		mStr = (uchar_t *)pool_alloc(sizeof(uchar_t) * mSize);
		mStr[0] = 0;
	}

	UString::~UString()
	{
		pool_free(mStr);
	}

	bool UString::operator ==(const UString & _str) const
	{
		return u_strcmp(mStr, _str.c_str()) == 0;
	}

	bool UString::operator !=(const UString & _str) const
	{
		return u_strcmp(mStr, _str.c_str()) != 0;
	}

	void UString::operator +=(const UString & _str)
	{
		int needSize = Length() + _str.Length() + 1;

		if (mSize < needSize)
		{
			mSize = needSize;

			uchar_t * newStr = (uchar_t *)pool_alloc(sizeof(uchar_t) * mSize);

			u_strcpy(newStr, mStr);
			u_strcat(newStr, _str.c_str());

			pool_free (mStr);
			mStr = newStr;
		}
		else
		{
			u_strcat(mStr, _str.c_str());
		}
	}

	void UString::operator +=(uchar_t _char)
	{
		Insert(Length(), &_char, 1);
	}


	UString & UString::operator =(const UString & _str)
	{
		if (mSize < _str.Length() + 1)
		{
			pool_free (mStr);

			mSize = _str.mSize;
			mStr = (uchar_t *)pool_alloc(sizeof(uchar_t) * mSize);
			u_strcpy(mStr, _str.c_str());
		}
		else
		{
			u_strcpy(mStr, _str.c_str());
		}

		return *this;
	}

	uchar_t & UString::operator [](int _i)
	{
		return mStr[_i];
	}

	const uchar_t & UString::operator [](int _i) const
	{
		return mStr[_i];
	}

	const uchar_t * UString::c_str() const
	{
		return mStr;
	}

	int UString::Length() const
	{
		return u_strlen(mStr);
	}

	void UString::Resize(int _size)
	{
		if (_size <= mSize)
			return ;

		mSize = _size;

		uchar_t * newStr = (uchar_t *)pool_alloc(sizeof(uchar_t) * mSize);
		u_strcpy(newStr, mStr);

		pool_free (mStr);
		mStr = newStr;
	}

	void UString::Insert(int _i, uchar_t _str)
	{
		Insert(_i, &_str, 1);
	}

	void UString::Insert(int _pos, const uchar_t * _str, int _count)
	{
		int length = Length();

		d_assert(_pos >= 0 && _pos <= length);

		int needSize = _count + length + 1;

		if (mSize < needSize)
		{
			Resize(needSize + 16);
		}

		for (int i = length; i >= _pos; --i)
		{
			mStr[i + _count] = mStr[i];
		}

		for (int i = 0; i < _count; ++i, ++_pos)
		{
			mStr[_pos] = _str[i];
		}
	}

	void UString::Erase(int _i, int _count)
	{
		d_assert (_i + _count <= Length());

		if (_count == 0)
			return ;

		const uchar_t * str = mStr + _i;

		for (int i = 0; i < _count; ++i)
		{
			mStr[i + _i] = 0;
			++str;
		}

		u_strcat(mStr, str);
	}
}

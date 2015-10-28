/*
*	UString
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MCharSet.h"

namespace Rad {

	class M_ENTRY UString
	{
		DECLARE_ALLOC();

	public:
		UString(const UString & _str);
		UString(const uchar_t * _str);
		UString();
		~UString();

		bool 
			operator ==(const UString & _str) const;
		bool 
			operator !=(const UString & _str) const;
		void 
			operator +=(const UString & _str);
		void 
			operator +=(uchar_t _char);
		UString & 
			operator =(const UString & _str);

		uchar_t & 
			operator [](int _i);
		const uchar_t & 
			operator [](int _i) const;

		const uchar_t * 
			c_str() const;
		int 
			Length() const;

		void 
			Resize(int _size);

		void 
			Insert(int _i, uchar_t _ch);
		void 
			Insert(int _i, const uchar_t * _str, int _count);
		void 
			Erase(int _i, int _count);

	protected:
		uchar_t * mStr;
		int mSize;
	};
}

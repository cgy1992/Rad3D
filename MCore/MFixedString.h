/*
*	FixedString
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAllocator.h"
#include "MCharSet.h"

namespace Rad {

	template <int MAX_SIZE>
	class FixedString
	{
		DECLARE_ALLOC();

	public:
		struct hash_t
		{
			// sbdsm hash

			int operator ()(const FixedString & rk) const
			{
				const char * str = rk.c_str();
				unsigned int hash = 0;

				while (*str)
				{
					// equivalent to: hash = 65599*hash + (*str++);
					hash = (*str++) + (hash << 6) + (hash << 16) - hash;
				}

				return (hash & 0x7FFFFFFF);
			}
		};

	public:
		FixedString()
		{
			mStr[0] = 0;
		}

		~FixedString()
		{
		}

		FixedString(const FixedString & rk)
		{
			*this = rk;
		}

		FixedString(const char * rk)
		{
			*this = rk;
		}

		char & operator [](int i)
		{
			d_assert (i < MAX_SIZE);

			return mStr[i];
		}

		const char & operator [](int i) const
		{
			d_assert (i < MAX_SIZE);

			return mStr[i];
		}

		//operator = 
		FixedString & operator =(const FixedString & rk)
		{
			strcpy(mStr, rk.c_str());
			return *this;
		}

		FixedString & operator =(const char * rk)
		{
			d_assert(rk != NULL && strlen(rk) < MAX_SIZE);

			strcpy_s(mStr, MAX_SIZE, rk);
			return *this;
		}

		FixedString & operator +=(const FixedString & rk)
		{
			strcat_s(mStr, MAX_SIZE, rk.c_str());

			return *this;
		}

		FixedString & operator +=(const char * rk)
		{
			strcat_s(mStr, MAX_SIZE, rk);

			return *this;
		}

		FixedString operator +(const FixedString & rk) const
		{
			FixedString tmp = *this;
			tmp += rk;
			return tmp;
		}

		FixedString operator +(const char * rk) const
		{
			FixedString tmp = *this;
			tmp += rk;
			return tmp;
		}

		friend FixedString operator +(const char * lk, const FixedString & rk)
		{
			return FixedString(lk) + rk;
		}

		bool operator ==(const FixedString & rk) const
		{
			return strcmp(mStr, rk.c_str()) == 0;
		}

		bool operator ==(const char * rk) const
		{
			d_assert(rk != NULL);
			return strcmp(mStr, rk) == 0;
		}

		friend bool operator ==(const char * lk, const FixedString & rk)
		{
			return rk == lk;
		}

		bool operator <(const FixedString & rk) const
		{
			return strcmp(mStr, rk.c_str()) < 0;
		}

		bool operator >(const FixedString & rk) const
		{
			return strcmp(mStr, rk.c_str()) > 0;
		}

		bool operator !=(const FixedString & rk) const
		{
			return strcmp(mStr, rk.c_str()) != 0;
		}

		bool operator !=(const char * rk) const
		{
			d_assert(rk != NULL);
			return strcmp(mStr, rk) != 0;
		}

		friend bool operator !=(const char * lk, const FixedString & rk)
		{
			return rk != lk;
		}

		void Trim()
		{
			if (mStr[0] == 0)
				return;

			char * str = c_str();
			int len = Length();

			//remove back
			int i = len - 1;
			while (i >= 0 && (str[i] == ' ' ||
				str[i] == '\t' ||
				str[i] == '\r' ||
				str[i] == '\n'))
			{
				--len;
				--i;
			}

			//remove front
			i = 0;
			while (i < len && (str[i] == ' ' ||
				str[i] == '\t' ||
				str[i] == '\r' ||
				str[i] == '\n'))
			{
				++i;
			}

			if (i > 0)
			{
				int iPos = 0;
				int iCopyPos = i;
				while (iCopyPos < len)
					str[iPos++] = str[iCopyPos++];
			}

			len -= i;
			str[len] = 0;
		}

		void Erase(int pos, int count)
		{
			int len = Length();

			d_assert(pos >= 0 && pos + count <= len);

			char * str = c_str();

			int iPos = pos;
			int iCopyPos = pos + count;

			while (iCopyPos < len)
				str[iPos++] = str[iCopyPos++];

			len -= count;
			str[len] = '\0';
		}

		void Format(const char * fmt, ...)
		{
			char text[MAX_SIZE];
			va_list arglist;

			va_start(arglist, fmt);
			vsprintf(text, fmt, arglist);
			va_end(arglist);

			(*this) = text;
		}

		void Clear()
		{
			mStr[0] = 0;
		}

		int Length() const
		{
			return strlen(mStr);
		}

		const char * c_str() const
		{
			return mStr;
		}

		char * c_str()
		{
			return mStr;
		}

		const uchar_t * c_wstr() const
		{
			static uchar_t buffer[MAX_SIZE];

			CharSet::MbStrToWbStr(buffer, mStr, MAX_SIZE);

			return buffer;
		}

		bool Match(const FixedString & rk) const
		{
			const char * left = c_str();
			const char * right = rk.c_str();
			int ret = 0;

			while (*right)
			{
				ret = (*left - *right);
				if (ret != 0)
					break;

				++left, ++right;
			}

			return ret;
		}

		int Find(char c, int offset, bool forward = true) const
		{
			int len = Length();

			if (offset > len)
				return -1;

			const char * str = c_str();

			if (forward)
			{
				for (int i = offset; i < len; ++i)
				{
					if (str[i] == c)
						return i;
				}

				return -1;
			}
			else
			{
				for (int i = pos; i >= 0; --i)
				{
					if (str[i] == c)
						return i;
				}

				return -1;
			}
		}

		int Find(const FixedString & rk, int offset = 0) const
		{
			const char * dst = c_str();
			const char * src = rk.c_str();
			int index = offset;
			char eq = 0;

			while (*dst)
			{
				dst = c_str() + index;
				src = rk.c_str();

				while (*src && !(eq = (*src - *dst)))
					++src, ++dst;

				if (!eq)
					break;

				++index;
			}

			return eq ? -1 : index;
		}

		void Replace(char oldc, char newc)
		{
			char * str = c_str();
			int len = Length();

			for (int i = 0; i < len; ++i)
			{
				if (str[i] == oldc)
					str[i] = newc;
			}
		}

		FixedString SubStr(int pos, int size) const
		{
			FixedString tmp;

			memcpy(tmp.c_str(), c_str() + pos, size * sizeof(char));
			tmp[size] = 0;

			return tmp;
		}

		void FromUnicode(const uchar_t * str)
		{
			CharSet::WbStrToMbStr(mStr, str, MAX_SIZE);
		}

	protected:
		char mStr[MAX_SIZE];
	};

	typedef FixedString<8>       FixedString8;
	typedef FixedString<16>      FixedString16;
	typedef FixedString<32>      FixedString32;
	typedef FixedString<48>      FixedString48;
	typedef FixedString<64>      FixedString64;
	typedef FixedString<256>     FixedString256;
}

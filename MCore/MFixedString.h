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

		const uchar_t * c_wstr() const
		{
			static uchar_t buffer[MAX_SIZE];

			CharSet::MbStrToWbStr(buffer, mStr, MAX_SIZE);

			return buffer;
		}

		char * c_str()
		{
			return mStr;
		}

		int ToInt() const
		{
			return atoi(mStr);
		}

		float ToFloat() const
		{
			return (float)atof(mStr);
		}

		double ToDouble() const
		{
			return atof(mStr);
		}

		bool ToBool() const
		{
			return *this == "true" || *this == "True";
		}

		FixedString & ToLower()
		{
			str_lwr(mStr);

			return *this;
		}

		FixedString & ToUpper()
		{
			str_lwr(mStr);

			return *this;
		}

		int Find(int pos, char c, bool bForward = true) const
		{
			int len = Length();

			if (pos > len)
				return -1;

			const char * str = c_str();

			if (bForward)
			{
				for (int i = pos; i < len; ++i)
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

		bool BeginOF(const FixedString & rk) const
		{
			const char * dst = c_str();
			const char * src = rk.c_str();

			while (*dst && *src)
			{
				if (*dst != *src)
					return false;

				++dst, ++src;
			}

			if (*src == 0)
				return true;

			return false;
		}

		int Find(const FixedString & rk, int pos = 0) const
		{
			const char * dst = c_str();
			const char * src = rk.c_str();
			int index = pos;
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

		void SplitFileName(FixedString & base, FixedString & path) const
		{
			FixedString name = *this;

			name.Replace('\\', '/');

			int len = name.Length();
			int split = name.Find(len, '/', false);

			if (split != -1)
			{
				base = name.SubStr(split + 1, len - split);
				path = name.SubStr(0, split);
			}
			else
			{
				base = name;
				path = ".";
			}
		}

		void SplitFileNameR(FixedString & base, FixedString & path)
		{
			Replace('\\', '/');

			int len = Length();
			int split = Find(len, '/', false);

			if (split != -1)
			{
				base = SubStr(split + 1, len - split);
				path = SubStr(0, split);
			}
			else
			{
				base = *this;
				path = ".";
			}
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

		FixedString SubStr(int offset, char cStart, char cEnd) const
		{
			int len = Length();
			int is, ie;
			if (offset >= len)
				return "";

			const char * str = c_str();
			is = offset;
			if (cStart != 0)
			{
				for (is = offset; is < len; is++)
				{
					if (str[is] == cStart)
						break;
				}

				is++;
				offset = is;
			}

			ie = len;
			if (cEnd != 0)
			{
				for (ie = offset; ie < len; ie++)
				{
					if (str[ie] == cEnd)
						break;
				}

				if (ie >= len)
					return "";
			}

			if (is < ie)
			{
				return SubStr(is, ie - is);
			}

			return "";
		}

		int Split(char c, FixedString & front, FixedString & back) const
		{
			front = "";
			back = "";

			int len = Length();
			int mid = -1;

			for (int i = 0; i < len; ++i)
			{
				if (mStr[i] == c)
				{
					mid = i;
					break;
				}
			}

			if (mid != -1)
			{
				memcpy(front.c_str(), c_str(), mid);
				front[mid] = 0;

				memcpy(back.c_str(), c_str() + mid + 1, len - (mid + 1));
				back[len - (mid + 1)] = 0;
			}

			return mid;
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

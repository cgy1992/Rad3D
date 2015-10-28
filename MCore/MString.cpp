#include "MString.h"
#include "MMemory.h"

namespace Rad {

#define STR_NO_POOL 0x80000000

	const String String::null("");

	String::String()
		: mStr(NULL)
		, mCapacity(0)
	{
		Alloc(16);
		mStr[0] = 0;
	}

	String::String(const String & str)
		: mStr(NULL)
		, mCapacity(0)
	{
		int len = str.Length();

		Alloc(len + 1);

		strcpy(mStr, str.mStr);
	}

	String::String(const char * str)
		: mStr(NULL)
		, mCapacity(0)
	{
		int len = strlen(str);

		Alloc(len + 1);

		strcpy(mStr, str);
	}

	String::~String()
	{
		Dealloc();
	}

	void String::Alloc(int size)
	{
		int nSize = mCapacity & (STR_NO_POOL - 1);

		if (nSize < size)
		{
			if (size < nSize * 2)
			{
				nSize *= 2;
			}
			else
			{
				nSize = size;
			}

			if ((mCapacity & STR_NO_POOL) == 0 && Memory::IsInit())
			{
				char * newStr = (char *)pool_alloc(nSize);

				if (mStr != NULL)
				{
					strcpy(newStr, mStr);
					pool_free(mStr);
				}

				mCapacity = nSize;
				mStr = newStr;
			}
			else
			{
				char * newStr = new char[nSize];

				if (mStr != NULL)
				{
					strcpy(newStr, mStr);
					safe_delete_array(mStr);
				}

				mCapacity = nSize | STR_NO_POOL;
				mStr = newStr;
			}

		}
	}

	void String::Dealloc()
	{
		if (mStr != NULL)
		{
			if ((mCapacity & STR_NO_POOL) == 0)
			{
				pool_free(mStr);
				mCapacity = 0;
			}
			else
			{
				delete[] mStr;
				mCapacity = STR_NO_POOL;
			}
		}

		mStr = NULL;
	}

	void String::Trim()
	{
		int len = Length();
		char * str = c_str();

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
			{
				str[iPos++] = str[iCopyPos++];
			}
		}
		
		len -= i;
		str[len] = '\0';
	}

	void String::Insert(int _pos, const char * _str, int _count)
	{
		int len = Length();

		d_assert(_pos >= 0 && _pos <= len);

		int needSize = _count + len + 1;

		Alloc(needSize);

		for (int i = len; i >= _pos; --i)
		{
			mStr[i + _count] = mStr[i];
		}

		for (int i = 0; i < _count; ++i, ++_pos)
		{
			mStr[_pos] = _str[i];
		}
	}

	void String::Erase(int pos, int count)
	{
		int len = Length();
		int iCopyPos = pos + count;

		d_assert(pos >= 0 && iCopyPos <= len);

		char * str = c_str();
		while (iCopyPos < len)
			str[pos++] = str[iCopyPos++];

		len -= count;
		str[len] = '\0';
	}

	void String::Format(const char * fmt, ...)
	{
		const int TEXT_BUFFER_SIZE = 2048;
		char text[TEXT_BUFFER_SIZE];
		va_list arglist;

		va_start(arglist, fmt);
		vsprintf(text, fmt, arglist);
		va_end(arglist);

		(*this) = text;
	}

	const uchar_t * String::c_wstr() const
	{
		static uchar_t buffer[1024];

		CharSet::MbStrToWbStr(buffer, mStr, 1024);

		return buffer;
	}

	void String::ToLower()
	{
		str_lwr(mStr);
	}

	void String::ToUpper()
	{
		str_upr(mStr);
	}

	int String::Find(char c, int offset) const
	{
		int len = Length();
		const char * str = c_str();

		for (int i = offset; i < len; ++i)
		{
			if (str[i] == c)
				return i;
		}

		return -1;
	}

	int String::FindR(char c, int offset) const
	{
		int len = Length();
		const char * str = c_str();

		offset = len - offset - 1;
		for (int i = offset; i >= 0; --i)
		{
			if (str[i] == c)
				return i;
		}

		return -1;
	}

	int String::Find(const String & key, int offset) const
	{
		int len = Length();
		const char * str = c_str();
		const char * end = &mStr[len];

		int keyLen = key.Length();
		char eq = 0;
		while (end - str > keyLen)
		{
			str = c_str() + offset++;

			if (strcmp(str, key.c_str()) == 0)
				return offset;
		}

		return -1;
	}

	int	String::FindR(const String & key, int offset) const
	{
		const char * str = c_str();
		char eq = 0;

		offset = Length()- key.Length() - offset - 1;

		while (str > mStr)
		{
			str = c_str() - offset--;

			if (strcmp(str, key.c_str()) == 0)
				return offset;
		}

		return -1;
	}

	int String::Compare(const String & str) const
	{
		const char * left = c_str();
		const char * right = str.c_str();
		int ret = 0;

		while (!(ret = (*left - *right)) && (*left))
			++left, ++right;

		return ret;
	}

	void String::SplitFileName(String & base, String & path) const
	{
		String name = *this;

		name.Replace('\\', '/');

		int len = name.Length();
		int split = name.FindR('/', 0);

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

	void String::Replace(char oldc, char newc)
	{
		char * str = c_str();

		int len = Length();
		for (int i = 0; i < len; ++i)
		{
			if (str[i] == oldc)
				str[i] = newc;
		}
	}

	String & String::operator =(const char * rk)
	{
		int needSize = strlen(rk) + 1;

		Alloc(needSize);

		strcpy(mStr, rk);

		return *this;
	}

	String & String::operator +=(const char * rk)
	{
		int len = Length();

		Alloc(len + strlen(rk) + 1);

		strcpy(mStr + len, rk);

		return *this;
	}

	String & String::operator +=(char ch)
	{
		char str[2] = { ch, 0 };

		return *this += str;
	}

	String String::operator +(const char * rk) const
	{
		String sOut(*this);

		sOut += rk;

		return sOut;
	}

	String & String::operator =(const String & rk)
	{
		Alloc(rk.Length() + 1);

		strcpy(mStr, rk.c_str());

		return *this;
	}

	String & String::operator +=(const String & rk)
	{
		int len = Length();

		Alloc(len + rk.Length() + 1);
		
		strcpy(c_str() + len, rk.c_str());

		return *this;
	}

	String String::operator +(const String & rk) const
	{
		String sOut(*this);

		sOut += rk;

		return sOut;
	}

	String String::SubStr(int pos, int size) const
	{
		int len = Length();
		if (pos >= len && pos + size > len)
			return "";

		String sOut;

		sOut.Alloc(size + 1);
		memcpy(sOut.c_str(), c_str() + pos, size * sizeof(char));
		sOut.c_str()[size] = 0;

		return sOut;
	}

	void String::FromUnicode(const uchar_t * wstr)
	{
		int needSize = u_strlen(wstr) * 2 + 2;

		Alloc(needSize);

		CharSet::WbStrToMbStr(mStr, wstr, needSize);
	}

	//
#ifndef M_PLATFORM_WIN32

	void strcat_s(char * dest, int max_size, const char * src)
	{
		d_assert (dest != NULL && src != NULL);

		while (*dest)
		{
			++dest;
			--max_size;
		}

		strcpy_s(dest, max_size, src);
	}

	void strcpy_s(char * dest, int max_size, const char * src)
	{
		d_assert (dest != NULL && src != NULL);

		int i = 0;
		while (i < max_size - 1 && src[i])
		{
			dest[i] = src[i];
			++i;
		}

		dest[i] = 0;
	}

#endif

	//
	const char * str_match(const char * str, const char * key)
	{
		d_assert (*key != 0);

		int length = 0;
		while (*str && *key && *str == *key)
		{
			++str;
			++key;
			++length;
		}

		return *key == 0 ? str : NULL;
	}

	void str_trim(char * str, int length)
	{
		if (length == 0)
			length = strlen(str);

		while (length > 0 && str[length - 1] == ' ')
		{
			str[length - 1] = 0;
			length -= 1;
		}
	}

	const char * str_skip(const char * str, char ch)
	{
		while (*str != 0 && *str == ch)
		{
			++str;
		}

		return str;
	}

	char * str_skip(char * str, char ch)
	{
		while (*str != 0 && *str == ch)
		{
			++str;
		}

		return str;
	}

	const char * str_skipwhite(const char * str)
	{
		while (*str != 0 && (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n'))
		{
			++str;
		}

		return str;
	}

	char * str_skipwhite(char * str)
	{
		while (*str != 0 && (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n'))
		{
			++str;
		}

		return str;
	}

	const char * str_getline(char * buffer, int maxcount, const char * str)
	{
		int n = 0;
		while (n < maxcount && *str && *str != '\n')
		{
			buffer[n++] = *str++;
		}

		d_assert (n < maxcount);

		if (*str == '\n')
			++str;

		buffer[n] = 0;

		return str;
	}

	const char * str_substring(char * buffer, int maxcount, const char * str, char end, bool skipwhite)
	{
		if (skipwhite)
		{
			str = str_skipwhite(str);
		}

		int n = 0;
		while (n < maxcount && *str)
		{
			if (*str == end)
				break;

			buffer[n++] = *str++;
		}

		buffer[n] = 0;

		return str;
	}

	const char * str_substring2(char * buffer, int maxcount, const char * str, char start, char end)
	{
		while (*str && *str != start)
			++str;

		if (*str && *str == start)
			++str;

		int n = 0, d = 0;

		if (start == end)
		{
			while (n < maxcount && *str)
			{
				if (*str == end)
					break;

				buffer[n++] = *str++;
			}
		}
		else
		{
			while (n < maxcount && *str)
			{
				if (*str == start)
					++d;

				if (*str == end && d-- <= 0)
					break;

				buffer[n++] = *str++;
			}
		}

		buffer[n] = 0;

		return str;
	}

	String f2str(float v, bool removeNil, const char * fmt)
	{
		char str[256];

		sprintf(str, fmt, v);

		if (removeNil)
		{
			int len = strlen(str);
			int p_idx = 0;

			while (p_idx == len || str[p_idx] != '.')
			{
				++p_idx;
			}

			if (p_idx != len)
			{
				int i = len - 1;
				while (i > p_idx)
				{
					if (str[i] != '0')
						break;

					str[i--] = '\0';
				}

				if (p_idx == i)
				{
					str[p_idx] = '\0';
				}
			}
		}

		return str;
	}

	String i2str(int v, const char * fmt)
	{
		char str[256];

		sprintf(str, fmt, v);

		return str;
	}

	String u2str(uint32_t v, const char * fmt)
	{
		char str[256];

		sprintf(str, fmt, v);

		return str;
	}
}

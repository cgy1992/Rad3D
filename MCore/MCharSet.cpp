#include "MCharSet.h"
#include "iconv.h"
#include <wchar.h>

namespace Rad
{
//#ifndef isascii
//	#define isascii(c) ((unsigned)(c) < 0x80)
//#endif
//
//	int __mbstowcs(wchar_t * ws, const char * s, int n)
//	{
//		int written = 0;
//
//		while (*s != '\0')
//		{
//			int len;
//			if (isascii(*s))
//			{
//				*ws = (wchar_t) *s;
//				len = 1;
//			}
//			else
//				len = mbtowc(ws, s, n);
//
//			if (len < 1)
//			{
//				written = -1;
//				break;
//			}
//			else
//			{
//				++ws;
//				++written;
//				s += len;
//				n -= len;
//			}
//		}
//
//		if (n > 0)
//			*ws = (wchar_t) 0;
//
//		return written;
//	}

	char _g_char_set[128] = "utf-8";

	void CharSet::SetCharSet(const char * _charset)
	{
		strcpy(_g_char_set, _charset);
	}

	void CharSet::GetCharSet(char * _charset)
	{
		strcpy(_charset, _g_char_set);
	}




	int CharSet::MbStrToWbStr(uchar_t * ws, const char * s, int max_count)
	{
		size_t result;

		size_t in_size = (strlen(s) + 1) * sizeof(char);
		size_t out_size = max_count * sizeof(uchar_t);

		iconv_t env = iconv_open("ucs-2le", _g_char_set);

		if (env == (iconv_t)-1)  
			return -1;  

		result = iconv(env, &s, &in_size, (char**)&ws, &out_size);

		iconv_close(env);

		return (int)result;
	}

	int CharSet::WbStrToMbStr(char * s, const uchar_t * ws, int max_count)
	{
		size_t result;

		size_t in_size = (u_strlen(ws) + 1) * sizeof(uchar_t);
		size_t out_size = max_count * sizeof(char);

		iconv_t env = iconv_open(_g_char_set, "ucs-2le");

		if (env == (iconv_t)-1)  
			return -1;  

		result = iconv(env, (const char**)&ws, &in_size, &s, &out_size);

		iconv_close(env);

		return (int)result;
	}

	int CharSet::Convert(char * out_str, const char * out_charset,
						 const char * in_str, const char * in_charset,
						 int max_count)
	{
		size_t result;

		size_t in_size = (strlen(in_str) + 1) * sizeof(char);
		size_t out_size = max_count * sizeof(char);

		iconv_t env = iconv_open(out_charset, in_charset);

		if (env == (iconv_t)-1)  
			return -1;  

		result = iconv(env, &in_str, &in_size, &out_str, &out_size);

		iconv_close(env);

		return (int)result;
	}


	int u_strlen(const uchar_t * str)
	{
		int len = 0;

		while (*str != 0)
		{
			++str;
			++len;
		}

		return len;
	}

	void u_strcpy(uchar_t * dest, const uchar_t * src)
	{
		while (*src != 0)
			*dest++ = *src++;

		*dest = 0;
	}

	void u_strcat(uchar_t * dest, const uchar_t * src)
	{
		while (*dest != 0)
			++dest;

		while (*src != 0)
			*dest++ = *src++;

		*dest = 0;
	}

	int u_strcmp(const uchar_t * str1, const uchar_t * str2)
	{
		while (*str1 != 0 && *str1 == *str2)
		{
			++str1, ++str2;
		}

		return *str1 - *str2;
	}

	int u_strncmp(const uchar_t * str1, const uchar_t * str2, int count)
	{
		int n = 0;

		while (n < count && *str1 != 0 && *str1 == *str2)
		{
			++n, ++str1, ++str2;
		}

		return n == count ? 0 : *str1 - *str2;
	}

	uchar_t * u_strchr(uchar_t * str, uchar_t ch)
	{
		while(*str != 0 && *str != ch) 
		{
			++str; 
		} 

		return *str == ch ? str : NULL;
	}

	const uchar_t * u_strchr(const uchar_t * str, uchar_t ch)
	{
		while(*str != 0 && *str != ch) 
		{
			++str; 
		} 

		return *str == ch ? str : NULL;
	}

	uchar_t * u_strstr(uchar_t * str, const uchar_t * sub_str)
	{
		int n = u_strlen(sub_str);
		
		while (str = u_strchr(str, *sub_str))
		{
			if (u_strncmp(str, sub_str, n) == 0)
			{
				return str;
			}

			++str;
		}

		return NULL;
	}

	const uchar_t * u_strstr(const uchar_t * str, const uchar_t * sub_str)
	{
		int n = u_strlen(sub_str);

		while (str = u_strchr(str, *sub_str))
		{
			if (u_strncmp(str, sub_str, n) == 0)
			{
				return str;
			}

			++str;
		}

		return NULL;
	}

	int u_strat(const uchar_t * str, uchar_t ch)
	{
		int _at = 0;

		while (*str != 0 && *str != ch)
		{
			++str;
			++_at;
		}

		return *str == ch ? _at : -1;
	}


	const uchar_t * to_ustr(const char * str)
	{
		static uchar_t buffer[512];

		CharSet::MbStrToWbStr(buffer, str, 512);

		return buffer;
	}

	void str_lwr(char * str, int len)
	{
		if (len == 0)
			len = strlen(str);

		char * p = str;
		while (p - str < len)
		{
			byte ch = (byte)(*p);

			if (ch >= 'A' && ch <= 'Z')
			{
				*p |= 0x20;
			}
			else if (ch > 0x7F) // not ascii
			{
				p += 1;
			}

			++p;
		}
	}

	void str_upr(char * str, int len)
	{
		if (len == 0)
			len = strlen(str);

		char * p = str;
		while (p - str < len)
		{
			byte ch = (byte)(*p);

			if (ch >= 'a' && ch <= 'z') // not ascii
			{
				*p &= ~0x20;
			}
			else if (ch > 0x7F)
			{
				p += 1;
			}

			++p;
		}
	}
}
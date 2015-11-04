/*
*	String
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAllocator.h"
#include "MCharSet.h"

namespace Rad {

	class M_ENTRY String
	{
	public:
		static const String null;

	public:
		struct hash_t
		{
			// sbdsm hash

			int operator ()(const String & s) const
			{
				const char * str = s.c_str();
				unsigned int hash = 0;

				while (*str)
				{
					// equivalent to: hash = 65599*hash + (*str++);
					hash = (*str++) + (hash << 6) + (hash << 16) - hash;
				}

				return (hash & 0x7FFFFFFF);
			}
		};

		struct chash_t
		{
			// sbdsm hash
			int operator ()(const char * str) const
			{
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
		String();
		String(const String & str);
		String(const char * str);
		~String();

		char &
			operator [](int i);
		const char &
			operator [](int i) const;

		String &
			operator =(const char * str);
		String &
			operator +=(const char * str);
		String &
			operator +=(char ch);
		String
			operator +(const char * str) const;
		bool
			operator ==(const char * str) const;
		bool
			operator <(const char * str) const;
		bool
			operator >(const char * str) const;
		bool
			operator !=(const char * str) const;

		String &
			operator =(const String & str);
		String &
			operator +=(const String & str);
		String
			operator +(const String & str) const;
		bool
			operator ==(const String & str) const;
		bool
			operator <(const String & str) const;
		bool
			operator >(const String & str) const;
		bool
			operator !=(const String & str) const;
		friend String
			operator +(const char * s1, const String & s2);

		void
			Alloc(int size);
		void
			Dealloc();
		void            
			Trim();
		void			
			Insert(int pos, const char * str, int count);
		void           
			Erase(int pos, int count);
		void			
			Format(const char * fmt, ...);
		bool           
			Empty() const;
		void            
			Clear();
		int             
			Length() const;
		const char *    
			c_str() const;
		char *          
			c_str();
		const uchar_t * 
			c_wstr() const;
		int             
			ToInt() const;
		float           
			ToFloat() const;
		double          
			ToDouble() const;
		String &            
			ToLower();
		String &            
			ToUpper();

		int 			
			Find(char c, int offset) const;
		int 			
			FindR(char c, int offset) const;
		int				
			Find(const String & str, int offset) const;
		int				
			FindR(const String & str, int offset) const;
		int				
			Compare(const String & str) const;
		void            
			SplitFileName(String & base, String & path) const;
		void            
			Replace(char oldc, char newc);
		String          
			SubStr(int i, int size) const;

		void			
			FromUnicode(const uchar_t * wstr);

	protected:
		char * mStr;
		int mCapacity;
	};

	//
	inline char & String::operator [](int i)
	{
		return mStr[i];
	}

	inline const char & String::operator [](int i) const
	{
		return mStr[i];
	}

	inline bool String::operator ==(const char * str) const
	{
		return strcmp(mStr, str) == 0;
	}

	inline bool String::operator <(const char * str) const
	{
		return strcmp(mStr, str) < 0;
	}

	inline bool String::operator >(const char * str) const
	{
		return strcmp(mStr, str) > 0;
	}

	inline bool String::operator !=(const char * str) const
	{
		return strcmp(mStr, str) != 0;
	}

	inline bool String::operator ==(const String & str) const
	{
		return Compare(str) == 0;
	}

	inline bool String::operator <(const String & str) const
	{
		return Compare(str) < 0;
	}

	inline bool String::operator >(const String & str) const
	{
		return Compare(str) > 0;
	}

	inline bool String::operator !=(const String & str) const
	{
		return Compare(str) != 0;
	}

	inline String operator +(const char * s1, const String & s2)
	{
		return String(s1) + s2;
	}

	inline void String::Clear()
	{
		mStr[0] = 0;
	}

	inline bool String::Empty() const
	{
		return *mStr == 0;
	}

	inline int String::Length() const
	{
		return strlen(mStr);
	}

	inline const char* String::c_str() const
	{
		return mStr;
	}

	inline char* String::c_str()
	{
		return mStr;
	}

	inline int String::ToInt() const
	{
		return atoi(c_str());
	}

	inline float String::ToFloat() const
	{
		return (float)atof(c_str());
	}

	inline double String::ToDouble() const
	{
		return atof(c_str());
	}

	inline String & String::ToLower()
	{
		str_lwr(mStr);

		return *this;
	}

	inline String & String::ToUpper()
	{
		str_upr(mStr);

		return *this;
	}

	inline int64 atoi64(const char * a)
	{
		int64 i = 0;

		bool neg = false;

		if (*a == '-')
		{
			neg = true;
			++a;
		}

		while (*a)
		{
			char ch = *a;

			if (ch >= '0' && ch <= '9')
			{
				i *= 10;
				i += int64(ch - '0');
			}
			else
			{
				break;
			}

			++a;
		}

		return !neg ? i : -i;
	}

	inline uint32_t atou(const char * a)
	{
		uint32_t i = 0;

		while (*a)
		{
			char ch = *a;

			if (ch >= '0' && ch <= '9')
			{
				i *= 10;
				i += uint32_t(ch - '0');
			}
			else
			{
				break;
			}

			++a;
		}

		return i;
	}

	inline uint64_t atou64(const char * a)
	{
		uint64_t i = 0;

		while (*a)
		{
			char ch = *a;

			if (ch >= '0' && ch <= '9')
			{
				i *= 10;
				i += uint64_t(ch - '0');
			}
			else
			{
				break;
			}

			++a;
		}

		return i;
	}

#ifndef M_PLATFORM_WIN32

	M_ENTRY void 
		strcat_s(char * dest, int destsize, const char * src);
	M_ENTRY void 
		strcpy_s(char * dest, int destsize, const char * src);

	#define	stricmp strcasecmp

#endif

	M_ENTRY const char *
		str_match(const char * str, const char * key);

	M_ENTRY void 
		str_trim(char * str, int length);

	M_ENTRY const char * 
		str_skip(const char * str, char ch);
	M_ENTRY char * 
		str_skip(char * str, char ch);

	M_ENTRY const char *
		str_skipwhite(const char * str);
	M_ENTRY char *
		str_skipwhite(char * str);

	M_ENTRY const char * 
		str_getline(char * buffer, int maxcount, const char * str);
	M_ENTRY const char * 
		str_substring(char * buffer, int maxcount, const char * str, char end = ' ', bool skipwhite = true);
	M_ENTRY const char * 
		str_substring2(char * buffer, int maxcount, const char * str, char start, char end);

	M_ENTRY String
		f2str(float v, bool removeNil = false, const char * fmt = "%f");
	M_ENTRY String
		i2str(int v, const char * fmt = "%d");
	M_ENTRY String
		u2str(uint32_t u, const char * fmt = "%u");

	
	inline bool is_letter(char ch)
	{
		return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
	}

	inline bool is_digit(char ch)
	{
		return (ch >= '0' && ch <= '0');
	}

	inline const char * str_getfloat(float & v, const char * str)
	{
		char buffer[128];

		str = str_substring(buffer, 128, str);

		v = (float)atof(buffer);

		return str;
	}

	inline const char * str_getint(int & v, const char * str)
	{
		char buffer[128];

		str = str_substring(buffer, 128, str);

		v = atoi(buffer);

		return str;
	}

	inline const char * str_getuint(uint32_t & v, const char * str)
	{
		char buffer[128];

		str = str_substring(buffer, 128, str);

		v = atou(buffer);

		return str;
	}
}


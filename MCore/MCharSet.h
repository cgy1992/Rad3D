/*
*	CharSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once
/*
All CharSet:

European languages
	ASCII, ISO-8859-{1,2,3,4,5,7,9,10,13,14,15,16}, KOI8-R, KOI8-U, KOI8-RU, CP{1250,1251,1252,1253,1254,1257}, CP{850,866,1131}, Mac{Roman,CentralEurope,Iceland,Croatian,Romania}, Mac{Cyrillic,Ukraine,Greek,Turkish}, Macintosh

Semitic languages
	ISO-8859-{6,8}, CP{1255,1256}, CP862, Mac{Hebrew,Arabic}

Japanese
	EUC-JP, SHIFT_JIS, CP932, ISO-2022-JP, ISO-2022-JP-2, ISO-2022-JP-1

Chinese
	EUC-CN, HZ, GBK, CP936, GB18030, EUC-TW, BIG5, CP950, BIG5-HKSCS, BIG5-HKSCS:2004, BIG5-HKSCS:2001, BIG5-HKSCS:1999, ISO-2022-CN, ISO-2022-CN-EXT

Korean
	EUC-KR, CP949, ISO-2022-KR, JOHAB

Armenian
	ARMSCII-8

Georgian
	Georgian-Academy, Georgian-PS

Tajik
	KOI8-T

Kazakh
	PT154, RK1048

Thai
	ISO-8859-11, TIS-620, CP874, MacThai

Laotian
	MuleLao-1, CP1133

Vietnamese
	VISCII, TCVN, CP1258

Platform specifics
	HP-ROMAN8, NEXTSTEP

Full Unicode
	UTF-8 
	UCS-2, UCS-2BE, UCS-2LE 
	UCS-4, UCS-4BE, UCS-4LE 
	UTF-16, UTF-16BE, UTF-16LE 
	UTF-32, UTF-32BE, UTF-32LE 
	UTF-7 
	C99, JAVA

Full Unicode, in terms of uint16_t or uint32_t (with machine dependent endianness and alignment)
	UCS-2-INTERNAL, UCS-4-INTERNAL

Locale dependent, in terms of `char' or `wchar_t' (with machine dependent endianness and alignment, and with OS and locale dependent semantics)
	char, wchar_t

European languages
	CP{437,737,775,852,853,855,857,858,860,861,863,865,869,1125}

Semitic languages
	CP864

Japanese
	EUC-JISX0213, Shift_JISX0213, ISO-2022-JP-3

Chinese
	BIG5-2003 (experimental)

Turkmen
	TDS565

Platform specifics
	ATARIST, RISCOS-LATIN1

*/

#include "MMemory.h"

namespace Rad {

#ifdef M_PLATFORM_WIN32
	typedef wchar_t uchar_t;
#else
	typedef unsigned short uchar_t;
#endif

	// 
	// CharSet -- Default "utf-8"
	//
	class M_ENTRY CharSet
	{
	public:
		static void
			SetCharSet(const char * _charset);
		static void
			GetCharSet(char * _charset);

		static int
			MbStrToWbStr(uchar_t * ws, const char * s, int max_count);
		static int
			WbStrToMbStr(char * s, const uchar_t * ws, int max_count);

		static int
			Convert(char * out_str, const char * out_charset,
			        const char * in_str, const char * in_charset,
					int max_count);
	};

	M_ENTRY int
		u_strlen(const uchar_t * str);
	M_ENTRY void
		u_strcpy(uchar_t * dest, const uchar_t * src);
	M_ENTRY void
		u_strcat(uchar_t * dest, const uchar_t * src);
	M_ENTRY int
		u_strcmp(const uchar_t * str1, const uchar_t * str2);
	M_ENTRY int
		u_strncmp(const uchar_t * str1, const uchar_t * str2, int count);
	M_ENTRY uchar_t *
		u_strchr(uchar_t * str, uchar_t ch);
	M_ENTRY const uchar_t *
		u_strchr(const uchar_t * str, uchar_t ch);
	M_ENTRY uchar_t *
		u_strstr(uchar_t * str, const uchar_t * sub_str);
	M_ENTRY const uchar_t *
		u_strstr(const uchar_t * str, const uchar_t * sub_str);

	M_ENTRY int
		u_strat(const uchar_t * str, uchar_t ch);

	M_ENTRY const uchar_t *
		to_ustr(const char * str);

#ifdef M_PLATFORM_WIN32
	#define WSTR_(str) L##str
#else
	#define WSTR_(str) to_ustr(str)
#endif

#define WCHAR_(ch) (uchar_t)(L##ch)

#define WS WSTR_
#define WC WCHAR_


	M_ENTRY void
		str_lwr(char * str, int len = 0);
	M_ENTRY void
		str_upr(char * str, int len = 0);
}

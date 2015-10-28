/*
*	Define
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

// win32:		M_PLATFORM_WIN32
// ios :		M_PLATFORM_IOS
// android :	M_PLATFORM_ANDROID
//
#include "MPlatform.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifndef M_PLATFORM_WIN32
#include <sys/types.h>
#endif

namespace Rad {

#ifndef M_ENTRY
#define M_ENTRY
#endif

#ifdef M_PLATFORM_WIN32
#ifdef _DEBUG
#define M_DEBUG
#endif
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MEMORY_IN_SYSTEM 0
#define MEMORY_IN_POOL16 16
#define MEMORY_IN_POOL32 32
#define MEMORY_IN_POOL64 64
#define MEMORY_IN_POOL128 128
#define MEMORY_IN_POOL256 256

#define MEMORY_ALIGN_0 0
#define MEMORY_ALIGN_4 4
#define MEMORY_ALIGN_8 8
#define MEMORY_ALIGN_16	16
#define MEMORY_ALIGN_32	32

#ifdef M_PLATFORM_WIN32
#define OFFSET_OF_CLASS(c, m) ((size_t)&(((c *)0)->m))
#else
#define OFFSET_OF_CLASS(c, m) ((size_t)&(((c *)1)->m) - 1)
#endif

#define SIZE_OF_CLASS(c, m) sizeof(((c *)0)->m)

//
#define PLT_WIN32		0x01
#define PLT_ANDROID		0x02
#define PLT_OIS			0x03

#ifndef M_PLATFORM
	#ifdef M_PLATFORM_WIN32
		#define M_PLATFORM PLT_WIN32
	#endif

	#ifdef M_PLATFORM_ANDROID
		#define M_PLATFORM PLT_ANDROID
	#endif

	#ifdef M_PLATFORM_IOS
		#define M_PLATFORM PLT_OIS
	#endif
#endif

//
#define CPU_X86		0x01
#define CPU_ARM		0x02
#define CPU_ARM_V7	0x03

#ifndef M_CPU_TYPE
	#ifdef M_PLATFORM_WIN32
		#define M_CPU_TYPE CPU_X86
	#endif
	
	#ifdef M_PLATFORM_ANDROID
		#define M_CPU_TYPE CPU_ARM
	#endif

	#ifdef M_PLATFORM_IOS
		#define M_CPU_TYPE CPU_ARM
	#endif
#endif

//
#ifdef M_PLATFORM_WIN32
//#define X86_ASM
#endif


//
#ifndef M_INT_DEFINED

	typedef int8_t	 int8;
	typedef int16_t	 int16;
	typedef int32_t	 int32;
	typedef int64_t	 int64;

	typedef uint8_t  uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;

#ifdef M_PLATFORM_WIN32
	typedef uint16	word;
	typedef uint32	dword;
	typedef uint64	qword;
#else
	typedef uint8	byte;
	typedef uint16	word;
	typedef uint32	dword;
	typedef uint64	qword;
#endif

#endif

//
#define MAKE_WORD(c0, c1) ((c0) << 8 | c1)
#define MAKE_DWORD(c0, c1, c2, c3) ((c0) << 24 | (c1) << 16 | (c2) << 8 | (c3))
#define MAKE_DWORD2(s0, s1) ((s0) << 16 | s1)

	inline int M_RGBA(int r, int g, int b, int a) { return a << 24 | b << 16 | g << 8 | r; }
	inline int M_RGBA_F(float r, float g, float b, float a)  { return M_RGBA((int)(r * 255), (int)(g * 255), (int)(b * 255), (int)(a * 255)); }
	inline int M_RGBA_A(int c) { return (c & 0xFF000000) >> 24; }
	inline int M_RGBA_R(int c) { return (c & 0x000000FF) >> 0; }
	inline int M_RGBA_G(int c) { return (c & 0x0000FF00) >> 8; }
	inline int M_RGBA_B(int c) { return (c & 0x00FF0000) >> 16; }
	
#define M_BIG_ENDIAN() ((*(short *)"1") > 0xFF)
	inline int M_SWAP16(int i) { return (i & 0x00FF) << 8 | (i & 0xFF00) >> 8; }
	inline int M_SWAP32(int i) { return (i & 0x000000FF) << 24 | (i & 0x0000FF00) << 8 | (i & 0x00FF0000) >> 8 | (i & 0xFF000000) >>24; }
	inline int M_INT16_LITTLE_TO_HOST(int i) { return M_BIG_ENDIAN() ? M_SWAP16(i) : i; }
	inline int M_INT32_LITTLE_TO_HOST(int i) { return M_BIG_ENDIAN() ? M_SWAP32(i) : i; }
	inline int M_INT16_BIG_TO_HOST(int i) { return M_BIG_ENDIAN() ? i : M_SWAP16(i); }
	inline int M_INT32_BIG_TO_HOST(int i) { return M_BIG_ENDIAN() ? i : M_SWAP32(i); }

}


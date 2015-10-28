/*
*	Platform for win32
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#define M_PLATFORM_WIN32

#include <windows.h>

#ifdef M_EXPORT
	#define M_ENTRY __declspec(dllexport)
#else
	#define M_ENTRY __declspec(dllimport)
#endif

#ifdef M_PLATFORM_WIN32
#pragma warning(disable : 4251)
#pragma warning(disable : 4996)
#endif

namespace Rad {

	M_ENTRY void d_printf(const char * fmt, ...);
}

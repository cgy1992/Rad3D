/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MBase.h"
#include "MGUI.h"

#ifdef M_PLATFORM_WIN32

#ifdef UTIL_EXPORT
#define UTIL_ENTRY __declspec(dllexport)
#else
#define UTIL_ENTRY __declspec(dllimport)
#endif

#else
#define UTIL_ENTRY
#endif

namespace Rad {

	UTIL_ENTRY bool 
		Util_QuestDlg(HWND hwnd, const char * caption);

	UTIL_ENTRY String 
		Util_GetOpenFile(const char * filter, const String & path = "");
	UTIL_ENTRY String 
		Util_GetSaveFile(const String & path, const char * filter);

	UTIL_ENTRY String 
		Util_GetOpenFloder();

	UTIL_ENTRY void 
		Util_StartEXE(const char * command);

}


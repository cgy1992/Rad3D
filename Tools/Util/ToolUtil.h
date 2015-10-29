/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MInclude.h"
#include <oleidl.h>

bool Util_QuestDlg(HWND hwnd, const char * caption);

String Util_GetOpenFile(const char * filter, const String & path = "");
String Util_GetSaveFile(const String & path, const char * filter);

String Util_GetOpenFloder();

void Util_StartEXE(const char * command);

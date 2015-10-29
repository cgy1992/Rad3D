/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#ifdef MSTUDIO_EXPORT
#define MSTUDIO_ENTRY __declspec(dllexport)
#else
#define MSTUDIO_ENTRY __declspec(dllimport)
#endif

#include "MInclude.h"

#define INDENT_TIME 0.2f
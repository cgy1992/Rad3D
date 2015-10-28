/*
*	MGUI Entry
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MCore.h"
#include "MInput.h"
#include "MRenderSystem.h"
#include "MResourceManager.h"
#include "MKeyController.h"

#ifdef M_PLATFORM_WIN32

#ifdef MGUI_EXPORT
	#define MGUI_ENTRY __declspec(dllexport)
#else
	#define MGUI_ENTRY __declspec(dllimport)
#endif

#else

#define  MGUI_ENTRY

#endif







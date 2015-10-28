/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MCore.h"
#include "MWorld.h"

#ifdef M_PLATFORM_WIN32

#ifdef PHY_EXPORT
	#define PHY_ENTRY __declspec(dllexport)
#else
	#define PHY_ENTRY __declspec(dllimport)
#endif

#else
	#define PHY_ENTRY
#endif


namespace Rad {

	typedef void * PhyHandle;

}
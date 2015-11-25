/*
*	ParticleFX
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MBase.h"
#include "MWorld.h"
#include "MRenderSystem.h"

#ifdef M_PLATFORM_WIN32

#ifdef FX_EXPORT
#define FX_ENTRY __declspec(dllexport)
#else
#define FX_ENTRY __declspec(dllimport)
#endif

#else

#define FX_ENTRY

#endif

namespace Rad {

	class FX_ENTRY ParticleFX
	{
	public:
		ParticleFX();
		~ParticleFX();
	};

}

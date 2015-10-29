/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LightFX_Define.h"

namespace Rad {

	class FX_Entity
	{
		DECLARE_ALLOC();
		DECLARE_RTTI_ROOT(FX_Entity);

	public:
		FX_Entity() {}
		virtual ~FX_Entity() {}
	};

}
/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LightFX_Define.h"
#include "LightFX_Entity.h"

namespace Rad {

	class FX_AOGenerator : public Singleton<FX_AOGenerator>
	{
	public:
		FX_AOGenerator();
		~FX_AOGenerator();

		Float3 DoAmbientOcclusion(const FX_Vertex & v, int flags, void * entity);

	protected:
		Field<Float4, 29> hd;
		Field<Float4, 13> ld;
	};

}


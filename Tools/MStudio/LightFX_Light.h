/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LightFX_Define.h"

namespace Rad {

	struct LIGHTFX_ENTRY FX_Light
	{
		int Type;

		Float3 Position;
		Float3 Direction;
		Float3 Color;

		float AttenStart;
		float AttenEnd;
		float AttenFallOff;

		float SpotInner;
		float SpotOuter;
		float SpotFallOff;

		FX_Light()
		{
			Type = eLightType::POINT;

			Position = Float3(0, 0, 0);
			Direction = Float3(0, 0, 1);

			Color = Float3(0, 0, 0);

			AttenStart = 0;
			AttenEnd = 1 * UNIT_METRES;
			AttenFallOff = 1;

			SpotInner = 1;
			SpotOuter = 0.7071f;
			SpotFallOff = 1;
		}
	};

	void FX_DoLighting(float & kd, float & ka, float & ks, const FX_Vertex & v, FX_Light * pLight);

}
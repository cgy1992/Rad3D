/*
*	EvParam
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MObject.h"

namespace Rad {

	struct M_ENTRY EvKeyFrame : public IObject
	{
		DECLARE_PROPERTY(IObject);

		float SkyLum;
		float SkyVOff;
		String SkyTexture;

		float SunPicth;
		float SunRoll;
		float SunSize;
		String SunTexture;

		Float3 MainLightAmbient;
		Float3 MainLightDiffuse;
		Float3 MainLightSpecular;
		float MainLightStrength;

		float FogStart;
		float FogEnd;
		Float3 FogColor;

		float WindDirX;
		float WindDirZ;

		float GrassWaveSpeed;
		float GrassWaveStrength;
		float GrassVisibleRadius;

		EvKeyFrame();

		static void 
			Lerp(EvKeyFrame & o, const EvKeyFrame & k0, const EvKeyFrame & k1, float t);
	};

	struct M_ENTRY EvParam
	{
		float SkyLum;
		float SkyU;
		float SkyV;
		String SkyTexture;

		Float3 SunPos;
		Float3 SunDir;
		float SunSize;
		String SunTexture;

		Float3 MainLightDir;
		Float3 MainLightAmbient;
		Float3 MainLightDiffuse;
		Float3 MainLightSpecular;

		float FogStart;
		float FogEnd;
		Float3 FogColor;

		Float3 WindDir;

		float GrassWaveSpeed;
		float GrassWaveStrength;
		float GrassVisibleRadius;
	};

}

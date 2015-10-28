#include "MEvParam.h"

namespace Rad {

	DF_PROPERTY_BEGIN(EvKeyFrame)
		/*
		DF_PROPERTY(EvKeyFrame, SkyLum, "Sky", "Lum", PT_Float)
		DF_PROPERTY(EvKeyFrame, SkyVOff, "Sky", "VOff", PT_Float)
		DF_PROPERTY(EvKeyFrame, SkyTexture, "Sky", "Texture", PT_Filename)
		*/

		DF_PROPERTY(EvKeyFrame, SunPicth, "Sun", "Pitch", PT_Float)
		DF_PROPERTY(EvKeyFrame, SunRoll, "Sun", "Roll", PT_Float)

		/*
		DF_PROPERTY(EvKeyFrame, SunSize, "Sun", "Size", PT_Float)
		DF_PROPERTY(EvKeyFrame, SunTexture, "Sun", "Texture", PT_Filename)
		*/

		DF_PROPERTY_EX(EvKeyFrame, MainLightAmbient, "MainLight", "Ambient", "PT_Color3", PT_Float3)
		DF_PROPERTY_EX(EvKeyFrame, MainLightDiffuse, "MainLight", "Diffuse", "PT_Color3", PT_Float3)
		DF_PROPERTY_EX(EvKeyFrame, MainLightSpecular, "MainLight", "Specular", "PT_Color3", PT_Float3)
		DF_PROPERTY(EvKeyFrame, MainLightStrength, "MainLight", "Strength", PT_Float)

		DF_PROPERTY(EvKeyFrame, FogStart, "Fog", "Start", PT_Float)
		DF_PROPERTY(EvKeyFrame, FogEnd, "Fog", "End", PT_Float)
		DF_PROPERTY_EX(EvKeyFrame, FogColor, "Fog", "Color", "PT_Color3", PT_Float3)

		DF_PROPERTY(EvKeyFrame, WindDirX, "Wave", "WindDirX", PT_Float)
		DF_PROPERTY(EvKeyFrame, WindDirZ, "Wave", "WindDirZ", PT_Float)

		DF_PROPERTY(EvKeyFrame, GrassWaveSpeed, "Grass", "WaveSpeed", PT_Float)
		DF_PROPERTY(EvKeyFrame, GrassWaveStrength, "Grass", "WaveStrength", PT_Float)
		DF_PROPERTY(EvKeyFrame, GrassVisibleRadius, "Grass", "VisibleRadius", PT_Float)

	DF_PROPERTY_END();

	EvKeyFrame::EvKeyFrame()
	{
		SkyLum = 1;
		SkyVOff = 0;
		SkyTexture = "SkyGradient.png";

		SunPicth = -45.0f;
		SunRoll = 45.0f;
		SunSize = 20;
		SunTexture = "Sun.png";

		MainLightAmbient = Float3(0.5f, 0.5f, 0.5f);
		MainLightDiffuse = Float3(1.0f, 1.0f, 1.0f);
		MainLightSpecular = Float3(0.0f, 0.0f, 0.0f);
		MainLightStrength = 1;

		FogStart = 50 * UNIT_METRES;
		FogEnd = 200 * UNIT_METRES;
		FogColor = Float3(1.0f, 1.0f, 1.0f);

		WindDirX = 1;
		WindDirZ = 0;

		GrassWaveSpeed = 0.8f;
		GrassWaveStrength = 0.15f;
		GrassVisibleRadius = 2500;
	}

	void EvKeyFrame::Lerp(EvKeyFrame & o, const EvKeyFrame & k0, const EvKeyFrame & k1, float t)
	{
		o.SkyLum = k0.SkyLum + (k1.SkyLum - k0.SkyLum) * t;
		o.SkyVOff = k0.SkyVOff + (k1.SkyVOff - k0.SkyVOff) * t;

		o.SunPicth = k0.SunPicth + (k1.SunPicth - k0.SunPicth) * t;
		o.SunRoll = k0.SunRoll + (k1.SunRoll - k0.SunRoll) * t;
		o.SunSize = k0.SunSize + (k1.SunSize - k0.SunSize) * t;

		o.MainLightAmbient = k0.MainLightAmbient + (k1.MainLightAmbient - k0.MainLightAmbient) * t;
		o.MainLightDiffuse = k0.MainLightDiffuse + (k1.MainLightDiffuse - k0.MainLightDiffuse) * t;
		o.MainLightSpecular = k0.MainLightSpecular + (k1.MainLightSpecular - k0.MainLightSpecular) * t;
		o.MainLightStrength = k0.MainLightStrength + (k1.MainLightStrength - k0.MainLightStrength) * t;

		o.FogStart = k0.FogStart + (k1.FogStart - k0.FogStart) * t;
		o.FogEnd = k0.FogEnd + (k1.FogEnd - k0.FogEnd) * t;
		o.FogColor = k0.FogColor + (k1.FogColor - k0.FogColor) * t;

		o.WindDirX = k0.WindDirX + (k1.WindDirX - k0.WindDirX) * t;
		o.WindDirZ = k0.WindDirZ + (k1.WindDirZ - k0.WindDirZ) * t;

		o.GrassWaveSpeed = k0.GrassWaveSpeed + (k1.GrassWaveSpeed - k0.GrassWaveSpeed) * t;
		o.GrassWaveStrength = k0.GrassWaveStrength + (k1.GrassWaveStrength - k0.GrassWaveStrength) * t;
		o.GrassVisibleRadius = k0.GrassVisibleRadius + (k1.GrassVisibleRadius - k0.GrassVisibleRadius) * t;
	}

}
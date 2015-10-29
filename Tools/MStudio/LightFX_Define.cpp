#include "LightFX_Define.h"

namespace Rad {

	DF_ENUM_BEGIN(FX_MSAA)
		DF_ENUM(NONE)
		DF_ENUM(MSAA_2X)
		DF_ENUM(MSAA_4X)
		DF_ENUM(MSAA_8X)
		DF_ENUM(MSAA_16X)
	DF_ENUM_END()

	DF_ENUM_BEGIN(FX_MapSize)
		DF_ENUM(SIZE_64)
		DF_ENUM(SIZE_128)
		DF_ENUM(SIZE_256)
		DF_ENUM(SIZE_512)
	DF_ENUM_END()

	DF_ENUM_BEGIN(FX_AOLevel)
		DF_ENUM(LOW)
		DF_ENUM(HIGH)
	DF_ENUM_END()

	DF_PROPERTY_BEGIN(FX_Setting)
		DF_PROPERTY_ENUM(FX_Setting, MSAA, "MSAA", "MSAA", FX_MSAA)

		DF_PROPERTY(FX_Setting, AOEnable, "AO", "Enable", PT_Bool)
		DF_PROPERTY_ENUM(FX_Setting, AOLevel, "AO", "Level", FX_AOLevel)
		DF_PROPERTY(FX_Setting, AORadius, "AO", "Radius", PT_Float)
		DF_PROPERTY_EX(FX_Setting, AOColor, "AO", "Color", "PT_Color3", PT_Float3)
		DF_PROPERTY(FX_Setting, AOStrength, "AO", "Strength", PT_Float)

		DF_PROPERTY(FX_Setting, AOGrassEnable, "GrassAO", "Enable", PT_Bool)
		DF_PROPERTY(FX_Setting, AOGrassRadius, "GrassAO", "Radius", PT_Float)
		DF_PROPERTY(FX_Setting, AOGrassFactory, "GrassAO", "Factory", PT_Float)
		DF_PROPERTY_EX(FX_Setting, AOGrassColor, "GrassAO", "Color", "PT_Color3", PT_Float3)

		DF_PROPERTY(FX_Setting, NumThread, "Editor", "NumThread", PT_Bool)
		DF_PROPERTY(FX_Setting, AutoShutdown, "Editor", "Shutdown", PT_Bool)
	DF_PROPERTY_END()

}
/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MCore.h"
#include "MMath2.h"
#include "MRenderSystem.h"

namespace Rad {

#define LIGHTFX_ENTRY

	struct FX_Vertex
	{
		Float3 Position;
		Float3 Normal;
		Float2 UV;
		Float2 LUV;
		int MaterialId;

		FX_Vertex()
		{
			Position = Float3(0, 0, 0);
			Normal = Float3(0, 0, 0);
			UV = Float2(0, 0);
			LUV = Float2(0, 0);
			MaterialId = 0;
		}

		void LerpFrom(const FX_Vertex & a, const FX_Vertex & b, float k)
		{
			Position = a.Position + (b.Position - a.Position) * k;
			Normal = a.Normal + (b.Normal - a.Normal) * k;
			UV = a.UV + (b.UV - a.UV) * k;
			LUV = a.LUV + (b.LUV - a.LUV) * k;
			MaterialId = a.MaterialId;

			Normal.Normalize();
		}
	};

	struct FX_Triangle
	{
		int Index0, Index1, Index2;
		int MaterialId;

		FX_Triangle()
		{
			Index0 = 0;
			Index1 = 0;
			Index2 = 0;
			MaterialId = 0;
		}
	};

	struct FX_Material
	{
		bool opacity;
		Float3 color;
		TexturePtr texture;

		FX_Material()
		{
			opacity = true;
			color = Float3(1, 1, 1);
			texture = NULL;
		}
	};

	class FX_Entity;

	struct FX_Contract
	{
		float td;
		float tu, tv;
		Float3 tn;
		int triIndex;
		FX_Entity * entity;
	};


	struct FX_Flag
	{
		enum enum_t
		{
			MESH = 1,
			TERRAIN = 2,

			ALL = 0xFFFFFFFF,
		};
	};

	struct FX_MSAA : public IEnum
	{
		DECLARE_ENUM(FX_MSAA);

		enum enum_t {
			NONE = 1,
			MSAA_2X = 2,
			MSAA_4X = 4,
			MSAA_8X = 8,
			MSAA_16X = 16,
		};
	};

	struct FX_MapSize : public IEnum
	{
		DECLARE_ENUM(FX_MapSize);

		enum enum_t {
			SIZE_64 = 64,
			SIZE_128 = 128,
			SIZE_256 = 256,
			SIZE_512 = 512,
		};
	};

	struct FX_AOLevel : public IEnum
	{
		DECLARE_ENUM(FX_AOLevel);

		enum enum_t {
			LOW,
			HIGH,
		};
	};
	
	struct FX_Setting : public IObject
	{
		DECLARE_PROPERTY(IObject);

		int MSAA;

		bool AOEnable;
		int AOLevel;
		float AORadius;
		Float3 AOColor;
		float AOStrength;

		bool AOGrassEnable;
		float AOGrassRadius;
		float AOGrassFactory;
		Float3 AOGrassColor;

		int NumThread;
		bool AutoShutdown;

		FX_Setting()
		{
			NumThread = 4;
			MSAA = FX_MSAA::NONE;

			AOEnable = false;
			AOLevel = FX_AOLevel::HIGH;
			AORadius = UNIT_METRES;
			AOColor = Float3(0.3f, 0.3f, 0.3f);
			AOStrength = 5.0f;

			AOGrassEnable = false;
			AOGrassRadius = 0.8f * UNIT_METRES;
			AOGrassFactory = 2.5f * UNIT_METRES;
			AOGrassColor = Float3(0.3f, 0.3f, 0.3f);

			AutoShutdown = false;
		}
	};
}
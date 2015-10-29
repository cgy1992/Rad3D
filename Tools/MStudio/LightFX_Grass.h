/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LightFX_Define.h"
#include "LightFX_Light.h"
#include "MGrassManager.h"

namespace Rad {

	struct FX_Grass
	{
		enum {
			K_SEGMENT = 6,
		};

		GrassInstance * mGI;
		Field<FX_Vertex, K_SEGMENT> mVertex;
		Field<Float3, K_SEGMENT> mLightingColor;

		FX_Grass()
		{
			mGI = NULL;
			for (int i = 0; i < array_count(mLightingColor); ++i)
			{
				mLightingColor[i] = Float3(0, 0, 0);
			}
		}
	};

	struct FX_GrassBlock
	{
		RectF mRect;
		Array<FX_Grass> mGrassArray;

		void GetGrass(Array<FX_Grass *> & ga, const Float3 & p, float radius);

		bool IsVisible(FX_Light * pLight);

		void DoAmbient();
		void DoLighting(FX_Light * pLight);

		void DoAmbientOcclusion();
	};

	class FX_GrassManager
	{
	public:
		FX_GrassManager();
		~FX_GrassManager();

		void 
			Clear();
		void 
			Build(int xblock, int zblock, float xblockSize, float zblockSize);
		
		void 
			AddGrass(const FX_Grass & gs);
		void 
			GetGrass(Array<FX_Grass *> & ga, const Float3 & p, float radius);

		Float3 
			DoAmbientOcclusion(const FX_Vertex & v);

		void 
			CalcuVertexColor();

		void 
			CalcuAmbientOcclusion();

		void 
			ProcessResult();

	protected:
		Array<FX_GrassBlock *> mGrassBlockArray;
	};
}
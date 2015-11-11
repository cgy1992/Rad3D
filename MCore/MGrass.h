/*
*	Grass
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"
#include "MNode.h"

namespace Rad {

#define K_VERTEX_COUNT_PER_GRASS 8
#define K_PRIM_COUNT_PER_GRASS 4
#define K_GRASS_RENDER_POOL 1024
#define K_GRASS_ROTATION_SEED 8

	//
	struct M_ENTRY GrassVertex
	{
		Float3 position;
		Float3 color;
		Float2 uv;
	};

	//
	struct M_ENTRY GrassTemplate
	{
		friend class GrassRenderer;

		int Id;
		RectF UVRect;
		bool CastAO;

		static void
			InitVertex();
		static void
			InitVertexEx(float rads[K_GRASS_ROTATION_SEED]);

		void 
			GetVertex(GrassVertex * v, const Float3 & pos, const Float3 & color, int rotation, float scale) const;

	protected:
		static Field<Float3, K_GRASS_ROTATION_SEED * K_VERTEX_COUNT_PER_GRASS> R_POSITION;
	};

	//
	struct M_ENTRY GrassInstance
	{
		int Id;
		Float3 Position;
		int Rotation;
		float Size;
		Float3 Normal;
		Float3 LightingColor;
	};

	//
	class GrassSection;

	struct M_ENTRY GrassBlock
	{
		friend class GrassSection;

		RectF RcBound;
		Array<GrassInstance> InstanceArray;

		GrassSection *
			GetSection() { return mSection; }

	protected:
		GrassSection * mSection;
	};
}

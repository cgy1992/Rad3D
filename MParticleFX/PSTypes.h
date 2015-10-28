/*
*	PSTypes
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"

namespace Rad {

	struct FX_ENTRY PS_BillboardType : public IEnum
	{
		DECLARE_ENUM(PS_BillboardType);

		enum {
			POINT,
			ORIENTED,
			ORIENTED_COMMON,
			PERPENDICULAR,
			PERPENDICULAR_COMMON,
		};
	};

	struct FX_ENTRY PS_BlendMode : public IEnum
	{
		DECLARE_ENUM(PS_BlendMode);

		enum {
			OPACITY,
			ADD,
			ALPHA_BLEND,
			COLOR_BLEND,
		};
	};

	struct FX_ENTRY PS_Operation : public IEnum
	{
		DECLARE_ENUM(PS_Operation);

		enum enum_t {
			SET,
			ADD,
			SUB,
			MUL,
			AVG
		};
	};

	//
	struct FX_ENTRY PS_Vertex
	{
		Float3 position;
		Float4 color;
		Float2 uv;
	};

	//
	struct FX_ENTRY PS_SortOp
	{
		PS_Set * mRender;
		Float3 mCameraPosition;

		int operator()(Particle * p1, Particle * p2)
		{
			Float3 position1 = p1->Position;
			Float3 position2 = p2->Position;

			if (mRender->IsLocalSpace())
			{
				const Mat4 & form = mRender->GetParent()->GetWorldTM();

				position1.TransformA(form);
				position2.TransformA(form);
			}

			float lenSq1 = mCameraPosition.DistanceSq(position1);
			float lenSq2 = mCameraPosition.DistanceSq(position2);

			return lenSq1 < lenSq2 ? -1 : (lenSq1 > lenSq2 ? 1 : 0);
		}
	};

}
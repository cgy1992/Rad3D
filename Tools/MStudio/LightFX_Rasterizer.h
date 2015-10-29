/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LightFX_Define.h"

namespace Rad {

	class FX_Mesh;

	class FX_Rasterizer
	{
	public:
		static void DoBlur(Float3 * data, int xMapSize, int zMapSize);

	public:
		FX_Mesh * _mesh;
		int _width, _height;
		Array<Float3> _rmap;
		bool _ao;

	public:
		FX_Rasterizer(FX_Mesh * mesh, int width, int height, bool ao);
		~FX_Rasterizer();

		void DoRasterize2(const FX_Vertex & A, const FX_Vertex & B, const FX_Vertex & C);

	protected:
		void _rasterize2(const FX_Vertex * A, const FX_Vertex * B, const FX_Vertex * C);
		void _output2(int x, int y, const FX_Vertex & v);
	};
}
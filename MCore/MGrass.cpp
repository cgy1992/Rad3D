#include "MGrass.h"

namespace Rad {

	Field<Float3, K_GRASS_ROTATION_SEED * K_VERTEX_COUNT_PER_GRASS> GrassTemplate::R_POSITION;

	void GrassTemplate::InitVertex()
	{
		int index = 0;
		for (int j = 0; j < K_GRASS_ROTATION_SEED; ++j)
		{
			int gi = j * K_VERTEX_COUNT_PER_GRASS;
			float rads = (float)j / (float)(K_GRASS_ROTATION_SEED) * PI2;

			Mat4 form;
			form.MakeRotationY(rads);

			//
			R_POSITION[index++] = Float3(-0.5f, 0, 0) * form;
			R_POSITION[index++] = Float3(+0.5f, 0, 0) * form;
			R_POSITION[index++] = Float3(-0.5f, 1, 0) * form;
			R_POSITION[index++] = Float3(+0.5f, 1, 0) * form;

			//
			R_POSITION[index++] = Float3(0, 0, -0.5f) * form;
			R_POSITION[index++] = Float3(0, 0, +0.5f) * form;
			R_POSITION[index++] = Float3(0, 1, -0.5f) * form;
			R_POSITION[index++] = Float3(0, 1, +0.5f) * form;
		}
	}

	void GrassTemplate::InitVertexEx(float rads[K_GRASS_ROTATION_SEED])
	{
		int index = 0;
		for (int j = 0; j < K_GRASS_ROTATION_SEED; ++j)
		{
			int gi = j * K_VERTEX_COUNT_PER_GRASS;

			Mat4 form;
			form.MakeRotationY(rads[j]);

			//
			R_POSITION[index++] = Float3(-0.5f, 0, 0) * form;
			R_POSITION[index++] = Float3(+0.5f, 0, 0) * form;
			R_POSITION[index++] = Float3(-0.5f, 1, 0) * form;
			R_POSITION[index++] = Float3(+0.5f, 1, 0) * form;

			//
			R_POSITION[index++] = Float3(0, 0, -0.5f) * form;
			R_POSITION[index++] = Float3(0, 0, +0.5f) * form;
			R_POSITION[index++] = Float3(0, 1, -0.5f) * form;
			R_POSITION[index++] = Float3(0, 1, +0.5f) * form;
		}
	}

	void GrassTemplate::GetVertex(GrassVertex * v, const Float3 & pos, const Float3 & color, int rotation, float scale) const
	{
		d_assert (rotation < K_GRASS_ROTATION_SEED);

		Float2 R_UV[K_VERTEX_COUNT_PER_GRASS] = {
			Float2(UVRect.x1, UVRect.y2),
			Float2(UVRect.x2, UVRect.y2),
			Float2(UVRect.x1, UVRect.y1),
			Float2(UVRect.x2, UVRect.y1),

			Float2(UVRect.x1, UVRect.y2),
			Float2(UVRect.x2, UVRect.y2),
			Float2(UVRect.x1, UVRect.y1),
			Float2(UVRect.x2, UVRect.y1),
		};

		int index = rotation * K_GRASS_ROTATION_SEED;
		for (int i = 0; i < K_VERTEX_COUNT_PER_GRASS; ++i)
		{
			v[i].position = R_POSITION[index + i] * scale + pos;
			v[i].uv = R_UV[i];
			v[i].color = color;
		}
	}

}
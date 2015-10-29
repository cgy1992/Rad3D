#include "LightFX_Rasterizer.h"
#include "LightFX_Mesh.h"
#include "LightFX_World.h"

namespace Rad {

	FX_Rasterizer::FX_Rasterizer(FX_Mesh * mesh, int width, int height, bool ao)
		: _mesh(mesh)
		, _width(width)
		, _height(height)
		, _ao(ao)
	{
		d_assert (width > 16 && height > 16);

		_rmap.Resize(width * height);
		memset(_rmap.c_ptr(), 0, sizeof(Float3) * _rmap.Size());
	}

	FX_Rasterizer::~FX_Rasterizer()
	{
	}

	void FX_Rasterizer::DoRasterize2(const FX_Vertex & A, const FX_Vertex & B, const FX_Vertex & C)
	{
		FX_Vertex v1 = A;
		FX_Vertex v2 = B;
		FX_Vertex v3 = C;

		v1.LUV.x = v1.LUV.x * (_width - 0);
		v1.LUV.y = v1.LUV.y * (_height - 0);

		v2.LUV.x = v2.LUV.x * (_width - 0);
		v2.LUV.y = v2.LUV.y * (_height - 0);

		v3.LUV.x = v3.LUV.x * (_width - 0);
		v3.LUV.y = v3.LUV.y * (_height - 0);

		_rasterize2(&v1, &v2, &v3);
	}

	void FX_Rasterizer::_rasterize2(const FX_Vertex * PA, const FX_Vertex * PB, const FX_Vertex * PC)
	{
		int min_x = 0, max_x = _width - 1;
		int min_y = 0, max_y = _height - 1;

		// 确保点A是最上面的点
		if (PA->LUV.y > PB->LUV.y)
			Swap(PA, PB);

		if (PA->LUV.y > PC->LUV.y)
			Swap(PA, PC);

		// 确保B在C的左边
		if (PB->LUV.x > PC->LUV.x)
			Swap(PB, PC);

		Int2 A = Int2((int)PA->LUV.x, (int)PA->LUV.y);
		Int2 B = Int2((int)PB->LUV.x, (int)PB->LUV.y);
		Int2 C = Int2((int)PC->LUV.x, (int)PC->LUV.y);

		Int2 AB = B - A, AC = C - A;

		FX_Vertex v1, v2, v;

		if (B.y > C.y)
		{
			// 三角形类型1 (B.y < C.y)
			//		A*
			//        
			//			*C
			//	B*
			//

			// 画上半部分
			int cy = A.y, ey = C.y;
			cy = Max(cy, min_y);
			ey = Min(ey, max_y);

			while (cy <= ey)
			{
				float kab = AB.y > 0 ? (float)(cy - A.y) / (float)AB.y : 1;
				float kac = AC.y > 0 ? (float)(cy - A.y) / (float)AC.y : 1;

				int x1 = (int)(A.x + AB.x * kab); 
				int x2 = (int)(A.x + AC.x * kac);

				v1.LerpFrom(*PA, *PB, kab);
				v2.LerpFrom(*PA, *PC, kac);

				if (x1 > x2)
				{
					Swap(x1, x2);
					Swap(v1, v2);
				}

				int sx = Max(x1, min_x);
				int ex = Min(x2, max_x);

				for (int x = sx; x <= ex; x += 1)
				{
					float k = (x2 - x1) > 0 ? (float)(x - x1) / (float)(x2 - x1) : 1;

					v.LerpFrom(v1, v2, k);

					_output2(x, cy, v);
				}

				cy += 1;
			}
			
			// 画下半部分
			Int2 CB = Int2(B.x - C.x, B.y - C.y);
			ey = Min(B.y, max_y);

			while (cy <= ey)
			{
				float kab = AB.y > 0 ? (float)(cy - A.y) / (float)AB.y : 1;
				float kcb = CB.y > 0 ? (float)(cy - C.y) / (float)CB.y : 1;

				int x1 = (int)(A.x + AB.x * kab); 
				int x2 = (int)(C.x + CB.x * kcb);

				if (x1 > x2)
				{
					Swap(x1, x2);
					Swap(v1, v2);
				}

				int sx = Max(x1, min_x);
				int ex = Min(x2, max_x);

				v1.LerpFrom(*PA, *PB, kab);
				v2.LerpFrom(*PC, *PB, kcb);

				for (int x = sx; x <= ex; x += 1)
				{
					float k = (x2 - x1) > 0 ? (float)(x - x1) / (float)(x2 - x1) : 1;

					v.LerpFrom(v1, v2, k);

					_output2(x, cy, v);
				}

				cy += 1;
			}
		}
		else
		{
			// 三角形类型2 (B在C上面)
			//       A*
			//        
			//	 B*    
			//          *C
			//

			// 画上半部分
			int cy = A.y, ey = B.y;
			cy = Max(cy, min_y);
			ey = Min(ey, max_y);

			while (cy <= ey)
			{
				float kab = AB.y > 0 ? (float)(cy - A.y) / (float)AB.y : 1;
				float kac = AC.y > 0 ? (float)(cy - A.y) / (float)AC.y : 1;

				int x1 = (int)(A.x + AB.x * kab); 
				int x2 = (int)(A.x + AC.x * kac);

				v1.LerpFrom(*PA, *PB, kab);
				v2.LerpFrom(*PA, *PC, kac);

				if (x1 > x2)
				{
					Swap(x1, x2);
					Swap(v1, v2);
				}

				int sx = Max(x1, min_x);
				int ex = Min(x2, max_x);

				for (int x = sx; x <= ex; x += 1)
				{
					float k = (x2 - x1) > 0 ? (float)(x - x1) / (float)(x2 - x1) : 1;

					v.LerpFrom(v1, v2, k);

					_output2(x, cy, v);
				}

				cy += 1;
			}

			// 画下半部分
			Int2 BC = Int2(C.x - B.x, C.y - B.y);
			ey = Min(C.y, max_y);

			if (BC.y > 0)
			{
				while (cy <= ey)
				{
					float kbc = BC.y > 0 ? (float)(cy - B.y) / (float)BC.y : 1;
					float kac = AC.y > 0 ? (float)(cy - A.y) / (float)AC.y : 1;

					int x1 = (int)(B.x + BC.x * kbc); 
					int x2 = (int)(A.x + AC.x * kac);

					v1.LerpFrom(*PB, *PC, kbc);
					v2.LerpFrom(*PA, *PC, kac);

					if (x1 > x2)
					{
						Swap(x1, x2);
						Swap(v1, v2);
					}

					int sx = Max(x1, min_x);
					int ex = Min(x2, max_x);

					for (int x = sx; x <= ex; x += 1)
					{
						float k = (x2 - x1) > 0 ? (float)(x - x1) / (float)(x2 - x1) : 1;

						v.LerpFrom(v1, v2, k);

						_output2(x, cy, v);
					}

					cy += 1;
				}
			}
		}
	}

	void FX_Rasterizer::_output2(int x, int y, const FX_Vertex & v)
	{
		if (!_ao)
		{
			Float3 color = Float3(0, 0, 0);

			for (int i = 0; i < FX_World::Instance()->GetLightCount(); ++i)
			{
				FX_Light * pLight = FX_World::Instance()->GetLight(i);

				color += _mesh->_doLighting(v, pLight, _mesh->IsRecieveShadow());
			}

			_rmap[y * _width + x] = color;
		}
		else
		{
			Float3 color = FX_AOGenerator::Instance()->DoAmbientOcclusion(v, FX_Flag::MESH | FX_Flag::TERRAIN, _mesh);

			_rmap[y * _width + x] = color;
		}
	}

	void FX_Rasterizer::DoBlur(Float3 * data, int xMapSize, int zMapSize)
	{
		Float3 * temp = new Float3[xMapSize * zMapSize];

		int index = 0;
		for (int v = 0; v < zMapSize; ++v)
		{
			for (int u = 0; u < xMapSize; ++u)
			{
				Float3 color(0, 0, 0);

				for (int y = -1; y <= 1; ++y)
				{
					for (int x = -1; x <= 1; ++x)
					{
						int s = Math::Clamp(u + x, 0, xMapSize - 1);
						int t = Math::Clamp(v + y, 0, zMapSize - 1);

						color += data[t * xMapSize + s];
					}
				}

				color /= 9.0f;

				temp[index++] = color;
			}
		}

		memcpy(data, temp, xMapSize * zMapSize * sizeof(Float3));

		delete[] temp;
	}
}


#include "LightFX_Terrain.h"
#include "LightFX_World.h"
#include "MEnvironment.h"
#include "MUtil.h"

namespace Rad {

	ImplementRTTI(FX_Terrain, FX_Entity);

	FX_Terrain::FX_Terrain(Terrain * pTerrain)
		: mTerrain(pTerrain)
		, mMaterialColor(1, 1, 1)
	{
		int xGridCount = mTerrain->GetInfo()->GridCount.x;
		int zGridCount = mTerrain->GetInfo()->GridCount.y;

		mVertexBuffer.Alloc(xGridCount * zGridCount * 4);
		mTriBuffer.Alloc(xGridCount * zGridCount * 2);

		int index = 0;
		for (int j = 0; j < zGridCount; ++j)
		{
			for (int i = 0; i < xGridCount; ++i)
			{
				FX_Vertex v;
				int x, z;

				// a
				x = i, z = j;
				v.Position = mTerrain->_getPosition(x, z);
				v.Normal = mTerrain->_getNormal(x, z);
				v.UV = Float2(0, 0);
				v.LUV = Float2((float)(x) / xGridCount, (float)(z) / zGridCount);
				v.MaterialId = 0;
				mVertexBuffer.PushBack(v);

				// b
				x = i + 1, z = j;
				v.Position = mTerrain->_getPosition(x, z);
				v.Normal = mTerrain->_getNormal(x, z);
				v.UV = Float2(0, 0);
				v.LUV = Float2((float)(x) / xGridCount, (float)(z) / zGridCount);
				v.MaterialId = 0;
				mVertexBuffer.PushBack(v);

				// c
				x = i, z = j + 1;
				v.Position = mTerrain->_getPosition(x, z);
				v.Normal = mTerrain->_getNormal(x, z);
				v.UV = Float2(0, 0);
				v.LUV = Float2((float)(x) / xGridCount, (float)(z) / zGridCount);
				v.MaterialId = 0;
				mVertexBuffer.PushBack(v);

				// d
				x = i + 1, z = j + 1;
				v.Position = mTerrain->_getPosition(x, z);
				v.Normal = mTerrain->_getNormal(x, z);
				v.UV = Float2(0, 0);
				v.LUV = Float2((float)(x) / xGridCount, (float)(z) / zGridCount);
				v.MaterialId = 0;
				mVertexBuffer.PushBack(v);

				//
				FX_Triangle t1, t2;

				t1.Index0 = index + 2;
				t1.Index1 = index + 1;
				t1.Index2 = index + 0;

				t2.Index0 = index + 2;
				t2.Index1 = index + 3;
				t2.Index2 = index + 1;

				mTriBuffer.PushBack(t1);
				mTriBuffer.PushBack(t2);

				index += 4;
			}
		}
	}

	FX_Terrain::~FX_Terrain()
	{
	}

	void FX_Terrain::SetMaterialColor(const Float3 & color)
	{
		mMaterialColor = color;
	}

	void FX_Terrain::BuildKDTree()
	{
		if (mVertexBuffer.Size() == 0)
			return ;

		Aabb bound;
		bound.minimum = mVertexBuffer[0].Position;
		bound.maximum = mVertexBuffer[0].Position;

		for (int i = 1; i < mVertexBuffer.Size(); ++i)
		{
			bound.minimum = Float3::Minimum(bound.minimum, mVertexBuffer[i].Position);
			bound.maximum = Float3::Maximum(bound.maximum, mVertexBuffer[i].Position);
		}

		mKDTree.Build(bound, 8);

		for (int i = 0; i < mTriBuffer.Size(); ++i)
		{
			bool hr = _addTri(mKDTree.GetRoot(), i);

			d_assert (hr != NULL);
		}
	}

	void FX_Terrain::_rayCheckImp(FX_Contract & contract, KDTree<int>::Node * node, const Ray & ray, float length)
	{
		float dist = 0;

		if (!ray.Intersect(&dist, node->aabb) || dist >= contract.td || dist >= length)
			return ;

		for (int i = 0; i < node->elems.Size(); ++i)
		{
			int triIndex = node->elems[i];
			const FX_Triangle & triangle = mTriBuffer[triIndex];

			const Float3 & a = mVertexBuffer[triangle.Index0].Position;
			const Float3 & b = mVertexBuffer[triangle.Index1].Position;
			const Float3 & c = mVertexBuffer[triangle.Index2].Position;

			float tu, tv;
			if (ray.Intersect(&dist, tu, tv, a, b, c) && dist < contract.td && dist <= length)
			{
				contract.td = dist;
				contract.triIndex = triIndex;
				contract.tu = tu;
				contract.tv = tv;
				contract.tn = Float3::GetNormal(a, b, c);
				contract.entity = this;
			}
		}

		if (node->child[0] != NULL)
			_rayCheckImp(contract, node->child[0], ray, length);

		if (node->child[1] != NULL)
			_rayCheckImp(contract, node->child[1], ray, length);
	}

	void FX_Terrain::RayCheck(FX_Contract & contract, const Ray & ray, float length)
	{
		d_assert (mKDTree.GetRoot() != NULL);

		_rayCheckImp(contract, mKDTree.GetRoot(), ray, length);
	}

	bool FX_Terrain::_addTri(KDTree<int>::Node * node, int t)
	{
		const FX_Triangle & triangle = mTriBuffer[t];

		const Float3 & a = mVertexBuffer[triangle.Index0].Position;
		const Float3 & b = mVertexBuffer[triangle.Index1].Position;
		const Float3 & c = mVertexBuffer[triangle.Index2].Position;

		if (node->aabb.Contain(a) && node->aabb.Contain(b) && node->aabb.Contain(c))
		{
			if (node->child[0] != NULL && _addTri(node->child[0], t))
				return true;

			if (node->child[1] != NULL && _addTri(node->child[1], t))
				return true;

			node->elems.PushBack(t);

			return true;
		}

		return false;
	}

	void FX_Terrain::CalcuLightingMap()
	{
		int mapSize = mTerrain->GetInfo()->LMapSize;
		d_assert (mapSize > 0);

		int xMapSize = mapSize * mTerrain->GetInfo()->BlockCount.x;
		int zMapSize = mapSize * mTerrain->GetInfo()->BlockCount.y;

		mLightingMap.Resize(xMapSize * zMapSize);

		int index = 0;
		int msaa = FX_World::Instance()->GetSetting()->MSAA;
		float invMSAA = 1.0f / (msaa);
		for (int j = 0; j < zMapSize; ++j)
		{
			for (int i = 0; i < xMapSize; ++i)
			{
				Float3 lightingColor(0, 0, 0);

				for (int y = 0; y < msaa; ++y)
				{
					for (int x = 0; x < msaa; ++x)
					{
						float u = (i + x * invMSAA) / (xMapSize - 1);
						float v = (j + y * invMSAA) / (zMapSize - 1);

						FX_Vertex p;
						p.Position.x = u * mTerrain->GetInfo()->Size.x;
						p.Position.z = v * mTerrain->GetInfo()->Size.y;
						p.UV = Float2(0, 0);
						p.LUV = Float2(0, 0);
						p.MaterialId = 0;

						mTerrain->GetHeightAt(p.Position.y, p.Position.x, p.Position.z);
						mTerrain->GetNormalAt(p.Normal, p.Position.x, p.Position.z);

						for (int l = 0; l < FX_World::Instance()->GetLightCount(); ++l)
						{
							FX_Light * pLight = FX_World::Instance()->GetLight(l);

							lightingColor += _doLighting(p, pLight);
						}
					}
				}

				lightingColor /= (float)msaa * msaa;

				mLightingMap[index++] = lightingColor + Environment::Instance()->GetEvParam()->MainLightAmbient;
			}

			FX_World::Instance()->SetProgress((int)(j / (float)zMapSize * 100.0f));
		}
	}

	void FX_Terrain::GetLightingMap(Array<Color> & colors)
	{
		colors.Resize(mLightingMap.Size());

		for (int i = 0; i <  mLightingMap.Size(); ++i)
		{
			colors[i] = util::TO_RGBE(mLightingMap[i]);
		}
	}

	Float3 FX_Terrain::_doLighting(const FX_Vertex & v, FX_Light * pLight)
	{
		float kd = 0, ka = 0, ks = 0;

		FX_DoLighting(kd, ka, ks, v, pLight);

		if (kd * ka * ks >= 0)
		{
			float len = 0;
			Ray ray;

			if (pLight->Type != eLightType::DIRECTION)
			{
				ray.dir = pLight->Position - v.Position;
				len = ray.dir.Normalize();
			}
			else
			{
				ray.dir = -pLight->Direction;
				len = MAX_FLOAT;
			}

			ray.orig = v.Position + ray.dir * UNIT_METRES * 0.01f;

			if (len > 0.01f * UNIT_METRES)
			{
				FX_Contract contract;
				if (FX_World::Instance()->RayCheck(contract, ray, len, FX_Flag::MESH))
				{
					kd = ka = ks = 0;
				}
			}
		}

		return kd * ka * ks * mMaterialColor * pLight->Color;
	}

	void FX_Terrain::CalcuAmbientOcclusion()
	{
		int mapSize = mTerrain->GetInfo()->LMapSize;
		d_assert (mapSize > 0);

		int xMapSize = mapSize * mTerrain->GetInfo()->BlockCount.x;
		int zMapSize = mapSize * mTerrain->GetInfo()->BlockCount.y;

		int index = 0;
		int msaa = FX_World::Instance()->GetSetting()->MSAA;
		float invMSAA = 1.0f / (msaa);
		for (int j = 0; j < zMapSize; ++j)
		{
			for (int i = 0; i < xMapSize; ++i)
			{
				Float3 color(0, 0, 0);

				for (int y = 0; y < msaa; ++y)
				{
					for (int x = 0; x < msaa; ++x)
					{
						float u = (i + x * invMSAA) / (xMapSize - 1);
						float v = (j + y * invMSAA) / (zMapSize - 1);

						FX_Vertex p;
						p.Position.x = u * mTerrain->GetInfo()->Size.x;
						p.Position.z = v * mTerrain->GetInfo()->Size.y;

						mTerrain->GetHeightAt(p.Position.y, p.Position.x, p.Position.z);
						mTerrain->GetNormalAt(p.Normal, p.Position.x, p.Position.z);

						Float3 ao = FX_AOGenerator::Instance()->DoAmbientOcclusion(p, FX_Flag::MESH, this);

						if (FX_World::Instance()->GetGrassManager() &&
							FX_World::Instance()->GetSetting()->AOGrassEnable)
						{
							ao *= FX_World::Instance()->GetGrassManager()->DoAmbientOcclusion(p);
						}

						color += ao;
					}
				}

				color /= (float)msaa * msaa;
				color.Saturate();

				mLightingMap[index++] *= color;
			}

			FX_World::Instance()->SetProgress((int)(j / (float)zMapSize * 100.0f));
		}
	}

	void FX_Terrain::Finish()
	{
		int mapSize = mTerrain->GetInfo()->LMapSize;
		d_assert (mapSize > 0);

		int xMapSize = mapSize * mTerrain->GetInfo()->BlockCount.x;
		int zMapSize = mapSize * mTerrain->GetInfo()->BlockCount.y;

		FX_Rasterizer::DoBlur(&mLightingMap[0], xMapSize, zMapSize);
	}
}
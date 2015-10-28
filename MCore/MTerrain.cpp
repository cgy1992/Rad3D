#include "MTerrain.h"
#include "MTerrainMesh.h"
#include "MResourceManager.h"
#include "MWorld.h"
#include "MUtil.h"

namespace Rad {

	DF_PROPERTY_BEGIN(Terrain::Info)
		DF_PROPERTY(Terrain::Info, MorphStart, "", "", PT_Float)
		DF_PROPERTY(Terrain::Info, BlocksPerSection, "", "", PT_Int2)
		DF_PROPERTY(Terrain::Info, DefaultHeight, "", "", PT_Float)
		DF_PROPERTY(Terrain::Info, WMapSize, "", "", PT_Int)
		DF_PROPERTY(Terrain::Info, LMapSize, "", "", PT_Int)
	DF_PROPERTY_END()

	DF_PROPERTY_BEGIN(Terrain::Layer)
		DF_PROPERTY_EX(Terrain::Layer, DetailMap, "", "", "PT_Filename", PT_String)
		DF_PROPERTY_EX(Terrain::Layer, NormalMap, "", "", "PT_Filename", PT_String)
		DF_PROPERTY(Terrain::Layer, UVScale, "", "", PT_Float)
		DF_PROPERTY(Terrain::Layer, MaterialId, "", "", PT_Int)
	DF_PROPERTY_END()

	float Terrain::PixelError = 4;
	float Terrain::ErrorDistanceBias = 100 * 100;

	ImplementSingleton(Terrain);

	Terrain::Terrain()
		: mMeshes(NULL)
	{
		memset(mBodyIndex, 0, Terrain::kMaxDetailLevel * sizeof(IndexPool));
		memset(mConecterIndex, 0, Terrain::kMaxDetailLevel * Terrain::kMaxDetailLevel * 4 * sizeof(IndexPool));
		genBodyIndex();
		genConecterIndex();

		genXZUVBuffer();

		mShaderFX[0] = ShaderFXManager::Instance()->Load("Terrain1", "Shader/MTerrain.mfx", "#define D_LAYER1\n");
		mShaderFX[1] = ShaderFXManager::Instance()->Load("Terrain2", "Shader/MTerrain.mfx", "#define D_LAYER2\n");
		mShaderFX[2] = ShaderFXManager::Instance()->Load("Terrain3", "Shader/MTerrain.mfx", "#define D_LAYER3\n");
		mShaderFX[3] = ShaderFXManager::Instance()->Load("Terrain4", "Shader/MTerrain.mfx", "#define D_LAYER4\n");

		d_assert (mShaderFX[0] && mShaderFX[1] && mShaderFX[2] && mShaderFX[3]);

		mLightingMapShaderFX[0] = 
			ShaderFXManager::Instance()->Load("Terrain1Lightingmap", "Shader/MTerrainLightingmap.mfx", "#define D_LAYER1\n");
		mLightingMapShaderFX[1] = 
			ShaderFXManager::Instance()->Load("Terrain2Lightingmap", "Shader/MTerrainLightingmap.mfx", "#define D_LAYER2\n");
		mLightingMapShaderFX[2] = 
			ShaderFXManager::Instance()->Load("Terrain3Lightingmap", "Shader/MTerrainLightingmap.mfx", "#define D_LAYER3\n");
		mLightingMapShaderFX[3] = 
			ShaderFXManager::Instance()->Load("Terrain4Lightingmap", "Shader/MTerrainLightingmap.mfx", "#define D_LAYER4\n");

		mLightingShaderFX[eLightType::DIRECTION][0] = 
			ShaderFXManager::Instance()->Load("Terrain1DirLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER1\n #define DIRECTION\n");
		mLightingShaderFX[eLightType::DIRECTION][1] = 
			ShaderFXManager::Instance()->Load("Terrain2DirLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER2\n #define DIRECTION\n");
		mLightingShaderFX[eLightType::DIRECTION][2] = 
			ShaderFXManager::Instance()->Load("Terrain3DirLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER3\n #define DIRECTION\n");
		mLightingShaderFX[eLightType::DIRECTION][3] = 
			ShaderFXManager::Instance()->Load("Terrain4DirLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER4\n #define DIRECTION\n");

		mLightingShaderFX[eLightType::POINT][0] = 
			ShaderFXManager::Instance()->Load("Terrain1PointLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER1\n #define POINT\n");
		mLightingShaderFX[eLightType::POINT][1] = 
			ShaderFXManager::Instance()->Load("Terrain2PointLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER2\n #define POINT\n");
		mLightingShaderFX[eLightType::POINT][2] = 
			ShaderFXManager::Instance()->Load("Terrain3PointLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER3\n #define POINT\n");
		mLightingShaderFX[eLightType::POINT][3] = 
			ShaderFXManager::Instance()->Load("Terrain4PointLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER4\n #define POINT\n");

		mLightingShaderFX[eLightType::SPOT][0] = 
			ShaderFXManager::Instance()->Load("Terrain1SpotLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER1\n #define SPOT\n");
		mLightingShaderFX[eLightType::SPOT][1] = 
			ShaderFXManager::Instance()->Load("Terrain2SpotLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER2\n #define SPOT\n");
		mLightingShaderFX[eLightType::SPOT][2] = 
			ShaderFXManager::Instance()->Load("Terrain3SpotLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER3\n #define SPOT\n");
		mLightingShaderFX[eLightType::SPOT][3] = 
			ShaderFXManager::Instance()->Load("Terrain4SpotLighting", "Shader/MTerrainLighting.mfx", "#define D_LAYER4\n #define SPOT\n");

		World::Instance()->E_LoadSetting += new cListener0<Terrain>(this, &Terrain::_OnLoadSetting);
	}

	Terrain::~Terrain()
	{
		d_assert (mMeshes == NULL);

		for (int i = 0; i < Terrain::kMaxDetailLevel; ++i)
		{
			safe_delete_array(mBodyIndex[i].indeces);

			for (int j = 0; j < Terrain::kMaxDetailLevel; ++j)
			{
				safe_delete_array(mConecterIndex[i][j][TN_NorthIndex].indeces);
				safe_delete_array(mConecterIndex[i][j][TN_SouthIndex].indeces);
				safe_delete_array(mConecterIndex[i][j][TN_WestIndex].indeces);
				safe_delete_array(mConecterIndex[i][j][TN_EastIndex].indeces);
			}
		}
	}

	void Terrain::_OnLoadSetting()
	{
		mLayers.Clear();

		rml_node * node = World::Instance()->GetSetting()->first_node("Terrain");
		if (node != NULL)
		{
			mInfo.Serialize(node, false);

			rml_node * layerNode = node->first_node("Layer");
			while (layerNode != NULL)
			{
				Layer layer;
				layer.Serialize(layerNode, false);
				mLayers.PushBack(layer);

				layerNode = layerNode->next_sibling("Layer");
			}
		}

		d_assert (mInfo.BlocksPerSection.x > 0 && mInfo.BlocksPerSection.y > 0);

		const World::Info * wi = World::Instance()->GetInfo();

		mInfo.BlockCount = wi->SectionCount * mInfo.BlocksPerSection;
		mInfo.GridCount = mInfo.BlockCount * Int2(kMeshGridCount, kMeshGridCount);
		mInfo.VertexCount = mInfo.GridCount + Int2(1, 1);

		mInfo.GridSize = wi->SectionSize.x / (kMeshGridCount * mInfo.BlocksPerSection.x);
		mInfo.BlockSize = mInfo.GridSize * kMeshGridCount;

		mInfo.InvGridSize = 1 / mInfo.GridSize;
		mInfo.InvBlockSize = 1 / mInfo.BlockSize;

		mInfo.Size.x = mInfo.BlockSize * mInfo.BlockCount.x;
		mInfo.Size.y = mInfo.BlockSize * mInfo.BlockCount.y;
		mInfo.InvSize = Float2(1, 1) / mInfo.Size;

		if (node != NULL)
		{
			mHeights.Resize(mInfo.VertexCount.x * mInfo.VertexCount.y);
			for (int i = 0; i < mHeights.Size(); ++i)
			{
				mHeights[i] = mInfo.DefaultHeight;
			}

			mNormals.Resize(mInfo.VertexCount.x * mInfo.VertexCount.y);
			for (int i = 0; i < mNormals.Size(); ++i)
			{
				mNormals[i] = Color(128, 255, 128, 255);
			}

			mMeshes = new TerrainMesh[mInfo.BlockCount.x * mInfo.BlockCount.y];
			for (int j = 0; j < mInfo.BlockCount.y; ++j)
			{
				for (int i = 0; i < mInfo.BlockCount.x; ++i)
				{
					mMeshes[j * mInfo.BlockCount.x + i].Init(Int2(i, j));
				}
			}
		}
	}

	bool Terrain::IsValid()
	{
		return mHeights.Size() > 0;
	}

	bool Terrain::Load(const String & filename)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);
		if (stream == NULL || !IsValid())
			return false;

		ISerializerD IS(stream);

		int magic, version;
		IS >> magic;
		IS >> version;

		d_assert (magic == K_FILE_MAGIC);
		d_assert (version == K_VERSION);

		int ckId;
		while (IS.Read(&ckId, sizeof(int)) && ckId != 0)
		{
			if (ckId == 1001)
			{
				Int2 vertexCount;
				IS >> vertexCount;

				if (vertexCount == mInfo.VertexCount)
				{
					stream->Read(&mHeights[0], sizeof(float) * vertexCount.x * vertexCount.y);
				}
				else if (vertexCount.x < mInfo.VertexCount.x && vertexCount.y < mInfo.VertexCount.y)
				{
					for (int i = 0; i < vertexCount.y; ++i)
					{
						stream->Read(&mHeights[i * vertexCount.x], sizeof(float) * vertexCount.x);
					}
				}
				else
				{
					d_log("?: terrain vertex count don't matched.");
				}

				_calcuNormals();
			}
			else
			{
				d_assert (0);
			}
		}

		return true;
	}

	void Terrain::Unload()
	{
		safe_delete_array(mMeshes);

		mHeights.Clear();
		mNormals.Clear();
	}

	void Terrain::Save(const String & filename)
	{
		if (!IsValid())
			return ;

		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
		{
			d_log ("?: Save terrain failed, '%s'.", filename.c_str());
			return ;
		}

		OSerializerF OS(fp, true);
		OS << (int)K_FILE_MAGIC;
		OS << (int)K_VERSION;

		OS << 1001;
		OS << mInfo.VertexCount;
		OS.Write(&mHeights[0], sizeof(float) * mInfo.VertexCount.x * mInfo.VertexCount.y);

		OS << 0;
	}

	void Terrain::EnsureBuild()
	{
		if (!IsValid())
			return ;

		for (int j = 0; j < mInfo.BlockCount.y; ++j)
		{
			for (int i = 0; i < mInfo.BlockCount.x; ++i)
			{
				if (!mMeshes[j * mInfo.BlockCount.x + i].IsValid())
					mMeshes[j * mInfo.BlockCount.x + i].Build();
			}
		}
	}

	TexturePtr Terrain::_getDetailMap(int layer)
	{
		if (layer >= 0 && layer < mLayers.Size())
			return HWBufferManager::Instance()->LoadTexture(mLayers[layer].DetailMap);
		else
			return RenderHelper::Instance()->GetWhiteTexture();
	}

	TexturePtr Terrain::_getNormalMap(int layer)
	{
		if (layer >= 0 && layer < mLayers.Size())
			return HWBufferManager::Instance()->LoadTexture(mLayers[layer].NormalMap);
		else
			return RenderHelper::Instance()->GetWhiteTexture();
	}

	ShaderFX * Terrain::_getShaderFX(int techId, bool lightingmap)
	{
		d_assert (techId < kMaxBlendLayers);

		return lightingmap ? mLightingMapShaderFX[techId] : mShaderFX[techId];
	}

	ShaderFX * Terrain::_getLightingShaderFX(eLightType type, int techId)
	{
		d_assert (type < eLightType::MAX && techId < kMaxBlendLayers);

		return mLightingShaderFX[type][techId];
	}

	TerrainMesh * Terrain::_getBlock(int i, int j)
	{
		d_assert (i < mInfo.BlockCount.x && j < mInfo.BlockCount.y);

		return &mMeshes[j * mInfo.BlockCount.x + i];
	}

	void Terrain::_getActiveBlocks(Array<TerrainMesh *> & arr)
	{
		const FixedArray<WorldSection *, 9> & ws = World::Instance()->GetActiveSections();
		for (int i = 0; i < ws.Size(); ++i)
		{
			TerrainSection * section = ws[i]->GetDataT<TerrainSection>();
			if (section != NULL)
			{
				for (int k = 0; k < section->GetMeshes().Size(); ++k)
				{
					arr.PushBack(section->GetMeshes()[k]);
				}
			}
		}
	}

	TerrainSection * Terrain::_getCurrentSection()
	{
		WorldSection * ws = World::Instance()->GetCurrentSection();
		if (ws != NULL)
		{
			return ws->GetDataT<TerrainSection>();
		}

		return NULL;
	}

	Float3 Terrain::_getPosition(int i, int j)
	{
		float fx = i * mInfo.GridSize;
		float fy = _getHeight(i, j);
		float fz = j * mInfo.GridSize;

		return Float3(fx, fy, fz);
	}

	float Terrain::_getHeight(int i, int j)
	{
		d_assert (i < mInfo.VertexCount.x && j < mInfo.VertexCount.y);

		float height = mHeights[j * mInfo.VertexCount.x + i];

		return height;
	}

	float Terrain::_getHeightClamp(int i, int j)
	{
		i = Math::Clamp(i, 0, mInfo.VertexCount.x - 1);
		j = Math::Clamp(j, 0, mInfo.VertexCount.y - 1);

		return _getHeight(i, j);
	}

	Float3 Terrain::_getNormal(int i, int j)
	{
		d_assert (i < mInfo.VertexCount.x && j < mInfo.VertexCount.y);

		Color normal = mNormals[j * mInfo.VertexCount.x + i];

		return Float3(normal.r / 255.0f * 2 - 1, normal.g / 255.0f * 2 - 1, normal.b / 255.0f * 2 - 1);
	}

	Color Terrain::_getWeight(int i, int j)
	{
		int mapSize = mInfo.WMapSize;
		int blockX = -1, blockZ = -1;
		int localX = -1, localZ = -1;

		d_assert (i < mapSize * mInfo.BlockCount.x && j < mapSize * mInfo.BlockCount.y);

		MapBlockIndex(blockX, blockZ, localX, localZ, i, j, mapSize);

		return _getBlock(blockX, blockZ)->GetWeight(localX, localZ);
	}

	Color Terrain::_getLightingColor(int i, int j)
	{
		int mapSize = mInfo.LMapSize;
		int blockX = -1, blockZ = -1;
		int localX = -1, localZ = -1;

		d_assert (i < mapSize * mInfo.BlockCount.x && j < mapSize * mInfo.BlockCount.y);

		MapBlockIndex(blockX, blockZ, localX, localZ, i, j, mapSize);

		return _getBlock(blockX, blockZ)->GetLightingColor(localX, localZ);
	}

	bool Terrain::MapBlockIndex(int & blockX, int & blockZ, int & localX, int & localZ, int x, int z, int mapSize)
	{
		switch (mapSize)
		{
		case 64:
			blockX = x / 64;
			blockZ = z / 64;
			localX = x & 63;
			localZ = z & 63;
			return true;

		case 128:
			blockX = x / 128;
			blockZ = z / 128;
			localX = x & 127;
			localZ = z & 127;
			return true;

		case 256:
			blockX = x / 256;
			blockZ = z / 256;
			localX = x & 255;
			localZ = z & 255;
			return true;


		case 512:
			blockX = x / 512;
			blockZ = z / 512;
			localX = x & 511;
			localZ = z & 511;
			return true;

		case 1024:
			blockX = x / 1024;
			blockZ = z / 1024;
			localX = x & 1023;
			localZ = z & 1023;
			return true;
		}

		return false;
	}

	bool Terrain::GetHeightAt(float & h, float x, float z)
	{
		float fx = x * mInfo.InvGridSize;
		float fz = z * mInfo.InvGridSize;

		int ix0 = (int) fx;
		int iz0 = (int) fz;

		if (ix0 < 0 || ix0 > mInfo.VertexCount.x - 1 ||
			iz0 < 0 || iz0 > mInfo.VertexCount.y - 1 ||
			mMeshes == NULL)
			return false;

		float dx = fx - ix0;
		float dz = fz - iz0;

		int ix1 = ix0 + 1;
		int iz1 = iz0 + 1;

		ix1 = Min(ix1, mInfo.VertexCount.x - 1);
		iz1 = Min(iz1, mInfo.VertexCount.y - 1);

		float a = _getHeight(ix0, iz0);
		float b = _getHeight(ix1, iz0);
		float c = _getHeight(ix0, iz1);
		float d = _getHeight(ix1, iz1);
		float m = (b + c) * 0.5f;
		float h1, h2;

		if (dx + dz <= 1.0f)
		{
			d = m + (m - a);
		}
		else
		{
			a = m + (m - d);
		}

		h1 = a * (1.0f - dx) + b * dx;
		h2 = c * (1.0f - dx) + d * dx;

		h = h1 * (1.0f - dz) + h2 * dz;

		return true;
	}

	bool Terrain::GetNormalAt(Float3 & n, float x, float z)
	{
		float fx = x * mInfo.InvGridSize;
		float fz = z * mInfo.InvGridSize;

		int ix0 = (int) fx;
		int iz0 = (int) fz;

		if (ix0 < 0 || ix0 > mInfo.VertexCount.x - 1 ||
			iz0 < 0 || iz0 > mInfo.VertexCount.y - 1 ||
			mMeshes == NULL)
			return false;

		float dx = fx - ix0;
		float dz = fz - iz0;

		int ix1 = ix0 + 1;
		int iz1 = iz0 + 1;

		ix1 = Min(ix1, mInfo.VertexCount.x - 1);
		iz1 = Min(iz1, mInfo.VertexCount.y - 1);

		Float3 a = _getNormal(ix0, iz0);
		Float3 b = _getNormal(ix1, iz0);
		Float3 c = _getNormal(ix0, iz1);
		Float3 d = _getNormal(ix1, iz1);
		Float3 m = (b + c) * 0.5f;
		Float3 h1, h2;

		if (dx + dz <= 1.0f)
		{
			d = m + (m - a);
		}
		else
		{
			a = m + (m - d);
		}

		h1 = a * (1.0f - dx) + b * dx;
		h2 = c * (1.0f - dx) + d * dx; 

		n = h1 * (1.0f - dz) + h2 * dz;
		n.Normalize();

		return true;
	}

	bool Terrain::GetLightingColorAt(Float3 & c, float x, float z)
	{
		int mapSize = mInfo.LMapSize;
		if (mMeshes == NULL || mapSize == 0)
			return false;

		float u = x * mInfo.InvSize.x, v = z * mInfo.InvSize.y;

		if (u < 0 || u > 1 || v < 0 || v > 1)
			return false;

		int xMapSize = mapSize * mInfo.BlockCount.x;
		int zMapSize = mapSize * mInfo.BlockCount.y;

		u *= xMapSize - 1; v *= zMapSize - 1;

		int u0 = (int)u, v0 = (int)v;
		int u1 = u0 + 1, v1 = v0 + 1;

		u1 = Math::Clamp(u1, 0, xMapSize - 1);
		v1 = Math::Clamp(v1, 0, zMapSize - 1);

		Float3 c0 = util::RGBE_TO(_getLightingColor(u0, v0));
		Float3 c1 = util::RGBE_TO(_getLightingColor(u1, v0));
		Float3 c2 = util::RGBE_TO(_getLightingColor(u0, v1));
		Float3 c3 = util::RGBE_TO(_getLightingColor(u1, v1));

		float du = u - u0;
		float dv = v - v0;

		c0 = c0 + (c1 - c0) * du;
		c2 = c2 + (c3 - c2) * du;

		c = c0 + (c2 - c0) * dv;

		return true;
	}

	bool Terrain::RayCheck(Float3 * p, const Ray & ray, float step)
	{
		const int iMaxCount = 2000;

		int i = 0;
		float y = 0;
		Float3 trace = ray.orig;
		Float3 position;
		bool hr = true;

		if (ray.dir == Float3(0, 1, 0))
		{
			hr = GetHeightAt(y, trace.x, trace.z);
			if (hr && trace.y <= y)
			{
				position = Float3(trace.x, y, trace.z);
			}
		}
		else if (ray.dir == Float3(0, -1, 0))
		{
			hr = GetHeightAt(y, trace.x, trace.z);
			if (hr && trace.y >= y)
			{
				position = Float3(trace.x, y, trace.z);
			}
		}
		else
		{
			while (i++ < iMaxCount)
			{
				hr = GetHeightAt(y, trace.x, trace.z);
				if (hr && trace.y <= y)
				{
					position = Float3(trace.x, y, trace.z);
					break;
				}

				trace += ray.dir * step;
			}
		}
		
		if (hr && p != NULL)
		{
			*p = position;
		}

		return hr && i < iMaxCount;
	}

	float *	Terrain::LockHeight(const RectI & rc)
	{
		d_assert (mLockedData.Size() == 0);

		int w = rc.x2 - rc.x1 + 1;
		int h = rc.y2 - rc.y1 + 1;

		if (w <= 0 && h <= 0)
			return NULL;

		mLockedData.Resize(w * h);

		int index = 0;
		for (int j = rc.y1; j <= rc.y2; ++j)
		{
			for (int i = rc.x1; i <= rc.x2; ++i)
			{
				mLockedData[index++] = _getHeight(i, j);
			}
		}

		mLockedRect = rc;

		return &mLockedData[0];
	}

	void Terrain::UnlockHeight()
	{
		d_assert (mLockedData.Size() >= 0);

		mLockedRect.x1 = Max(0, mLockedRect.x1);
		mLockedRect.x2 = Max(0, mLockedRect.x2);
		mLockedRect.x2 = Min(mInfo.VertexCount.x - 1, mLockedRect.x2);
		mLockedRect.y2 = Min(mInfo.VertexCount.y - 1, mLockedRect.y2);

		int index = 0;
		for (int j = mLockedRect.y1; j <= mLockedRect.y2; ++j)
		{
			for (int i = mLockedRect.x1; i <= mLockedRect.x2; ++i)
			{
				mHeights[j * mInfo.VertexCount.x + i] = mLockedData[index++];
			}
		}

		// need re-calculate normals
		RectI rcNormal = mLockedRect;
		rcNormal.x1 -= 2;
		rcNormal.x2 += 2;
		rcNormal.y1 -= 2;
		rcNormal.y2 += 2;

		rcNormal.x1 = Max(0, rcNormal.x1);
		rcNormal.y1 = Max(0, rcNormal.y1);
		rcNormal.x2 = Min(mInfo.VertexCount.x - 1, rcNormal.x2);
		rcNormal.y2 = Min(mInfo.VertexCount.y - 1, rcNormal.y2);

		for (int j = rcNormal.y1; j < rcNormal.y2; ++j)
		{
			for (int i = rcNormal.x1; i < rcNormal.x2; ++i)
			{
				Float3 n = _calcuNormal(i, j);

				n.x = n.x * 0.5f + 0.5f;
				n.y = n.y * 0.5f + 0.5f;
				n.z = n.z * 0.5f + 0.5f;

				Color c;
				c.r = (unsigned char)(n.x * 255);
				c.g = (unsigned char)(n.y * 255);
				c.b = (unsigned char)(n.z * 255);
				c.a = 255;

				mNormals[j * mInfo.VertexCount.x + i] = c;
			}
		}

		// update sections
		for (int i = 0; i < mInfo.BlockCount.x * mInfo.BlockCount.y; ++i)
		{
			mMeshes[i]._updateHeight(mLockedRect);
		}

		mLockedData.Clear();
	}

	float * Terrain::LockWeightMap(const RectI & rc)
	{
		d_assert (mLockedWeightData.Size() == 0);

		int w = rc.x2 - rc.x1 + 1;
		int h = rc.y2 - rc.y1 + 1;

		if (w <= 0 && h <= 0)
			return NULL;

		mLockedWeightData.Resize(w * h);
		memset(mLockedWeightData.c_ptr(), 0, sizeof(float) * mLockedWeightData.Size());

		mLockedWeightRect = rc;

		return mLockedWeightData.c_ptr();
	}

	void Terrain::UnlockWeightMap(int layer)
	{
		d_assert (mLockedWeightData.Size() >= 0);

		const RectI & rc = mLockedWeightRect;
		Array<Color> weights;
		weights.Resize((rc.Width() + 1) * (rc.Height() + 1));

		int index = 0;
		for (int j = rc.y1; j <= rc.y2; ++j)
		{
			for (int i = rc.x1; i <= rc.x2; ++i)
			{
				float weight = mLockedWeightData[index];
				int blockX = -1, blockZ = -1;
				int localX = -1, localZ = -1;

				MapBlockIndex(blockX, blockZ, localX, localZ, i, j, mInfo.WMapSize);

				TerrainMesh * block = _getBlock(blockX, blockZ);
				int layer0 = block->GetLayer(0);
				int layer1 = block->GetLayer(1);
				int layer2 = block->GetLayer(2);
				int layer3 = block->GetLayer(3);

				Color c = block->GetWeight(localX, localZ);

				Float4 c4;
				c4.r = c.r / 255.0f;
				c4.g = c.g / 255.0f;
				c4.b = c.b / 255.0f;
				c4.a = c.a / 255.0f;

				if (layer == layer0)
					c4.r += weight;
				else if (layer == layer1)
					c4.g += weight;
				else if (layer == layer2)
					c4.b += weight;
				else if (layer == layer3)
					c4.a += weight;
				else
				{
					if (layer0 == -1)
					{
						c4.r += weight;
						block->SetLayer(0, layer);
					}
					else if (layer1 == -1)
					{
						c4.g += weight;
						block->SetLayer(1, layer);
					}
					else if (layer2 == -1)
					{
						c4.b += weight;
						block->SetLayer(2, layer);
					}
					else if (layer3 == -1)
					{
						c4.a += weight;
						block->SetLayer(3, layer);
					}
				}

				float sum = c4.r + c4.g + c4.b + c4.a;

				if (sum > 0)
				{
					c4 *= 1 / sum;
				}

				c.r = (unsigned char)(c4.r * 255);
				c.g = (unsigned char)(c4.g * 255);
				c.b = (unsigned char)(c4.b * 255);
				c.a = (unsigned char)(c4.a * 255);

				weights[index++] = c;
			}
		}

		// update weight map
		for (int i = 0; i < mInfo.BlockCount.x * mInfo.BlockCount.y; ++i)
		{
			mMeshes[i]._updateWeightMap(rc, weights);
		}

		mLockedWeightData.Clear();
	}

	void Terrain::SetLightingMap(Array<Color> & colors)
	{
		int mapSize = mInfo.LMapSize;

		d_assert (colors.Size() == mapSize * mapSize * mInfo.BlockCount.x * mInfo.BlockCount.y);

		for (int i = 0; i < mInfo.BlockCount.x * mInfo.BlockCount.y; ++i)
		{
			mMeshes[i]._updateLightingMap(colors);
		}
	}

	void Terrain::ResetLighting()
	{
		for (int i = 0; i < mInfo.BlockCount.x * mInfo.BlockCount.y; ++i)
		{
			mMeshes[i]._resetLighting();
		}
	}

	const Terrain::IndexData &  Terrain::GetIndexData(const LodKey & k)
	{
		d_assert(
			k.level < Terrain::kMaxDetailLevel &&
			k.west < Terrain::kMaxDetailLevel &&
			k.east < Terrain::kMaxDetailLevel &&
			k.south < Terrain::kMaxDetailLevel &&
			k.north < Terrain::kMaxDetailLevel);

		int ib = mIndexBufferMap.Find(k);

		if (ib == -1)
		{
			IndexData data;
			data.start = 0;
			data.index_count = 0;
			data.prim_count = 0;
			data.prim_type = ePrimType::TRIANGLE_LIST;

			int tiles = kMeshGridCount >> k.level;

			const IndexPool & body = mBodyIndex[k.level];
			const IndexPool & north = mConecterIndex[k.level][k.north][TN_NorthIndex];
			const IndexPool & south = mConecterIndex[k.level][k.south][TN_SouthIndex];
			const IndexPool & west = mConecterIndex[k.level][k.west][TN_WestIndex];
			const IndexPool & east = mConecterIndex[k.level][k.east][TN_EastIndex];

			if (body.indeces)
				data.index_count += body.size;

			if (north.indeces)
				data.index_count += (north.size - 2) * 3;

			if (south.indeces)
				data.index_count += (north.size - 2) * 3;

			if (west.indeces)
				data.index_count += (north.size - 2) * 3;

			if (east.indeces)
				data.index_count += (north.size - 2) * 3;

			d_assert (data.index_count > 0 && data.index_count < 65536);

			data.index_buffer = HWBufferManager::Instance()->NewIndexBuffer(data.index_count);
			int index = 0;

			short * indeces = (short *)data.index_buffer->Lock(eLockFlag::WRITE);

			if (body.indeces)
			{
				memcpy(indeces + index, body.indeces, body.size * sizeof(short));
				index += body.size;
			}

			if (north.indeces)
			{
				for (int i = 0; i < north.size - 2; i += 2)
				{
					short a = north.indeces[i + 0];
					short b = north.indeces[i + 1];
					short c = north.indeces[i + 2];
					short d = north.indeces[i + 3];

					indeces[index++] = a;
					indeces[index++] = b;
					indeces[index++] = c;

					indeces[index++] = c;
					indeces[index++] = b;
					indeces[index++] = d;
				}
			}

			if (south.indeces)
			{
				for (int i = 0; i < south.size - 2; i += 2)
				{
					short a = south.indeces[i + 0];
					short b = south.indeces[i + 1];
					short c = south.indeces[i + 2];
					short d = south.indeces[i + 3];

					indeces[index++] = a;
					indeces[index++] = b;
					indeces[index++] = c;

					indeces[index++] = c;
					indeces[index++] = b;
					indeces[index++] = d;
				}
			}

			if (west.indeces)
			{
				for (int i = 0; i < west.size - 2; i += 2)
				{
					short a = west.indeces[i + 0];
					short b = west.indeces[i + 1];
					short c = west.indeces[i + 2];
					short d = west.indeces[i + 3];

					indeces[index++] = a;
					indeces[index++] = b;
					indeces[index++] = c;

					indeces[index++] = c;
					indeces[index++] = b;
					indeces[index++] = d;
				}
			}

			if (east.indeces)
			{
				for (int i = 0; i < east.size - 2; i += 2)
				{
					short a = east.indeces[i + 0];
					short b = east.indeces[i + 1];
					short c = east.indeces[i + 2];
					short d = east.indeces[i + 3];

					indeces[index++] = a;
					indeces[index++] = b;
					indeces[index++] = c;

					indeces[index++] = c;
					indeces[index++] = b;
					indeces[index++] = d;
				}
			}

			data.index_buffer->Unlock();

			data.prim_count = index / 3;

			ib = mIndexBufferMap.Insert(k, data);
		}

		return mIndexBufferMap[ib].value;;
	}

	void Terrain::genXZUVBuffer()
	{
		mShaderVertexDecl.Clear();
		mShaderVertexDecl.AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT2);
		mShaderVertexDecl.AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		int vertexStride = sizeof(TN_Vertex1);
		int vertexCount = Terrain::kMeshVertexCount * Terrain::kMeshVertexCount;

		mSharedVertexBuffer = HWBufferManager::Instance()->NewVertexBuffer(vertexStride, vertexCount);

		TN_Vertex1 * v1 = (TN_Vertex1 *)mSharedVertexBuffer->Lock(eLockFlag::WRITE);
		for (int j = 0; j <= Terrain::kMeshGridCount; ++j)
		{
			for (int i = 0; i <= Terrain::kMeshGridCount; ++i)
			{
				v1->xz.x = (float)i;
				v1->xz.y = (float)Terrain::kMeshGridCount - j; // flip
				v1->uv.x = i / (float)Terrain::kMeshGridCount;
				v1->uv.y = j / (float)Terrain::kMeshGridCount;


				++v1;
			}
		}
		mSharedVertexBuffer->Unlock();
	}

	void Terrain::genBodyIndex()
	{
		for (int i = 0; i < kMaxDetailLevel; ++i)
		{
			_genBodyIndex(i);
		}
	}

	void Terrain::_genBodyIndex(int level)
	{
		d_assert (mBodyIndex[level].size == 0 &&
				  mBodyIndex[level].indeces == NULL);

		int tiles = kMeshGridCount  >> level;
		int step = 1 << level;
		int start = 0;

		if (level < kMaxDetailLevel - 1)
		{
			tiles -= 2;
			start = step * kMeshVertexCount + step;
		}

		if (tiles == 0 || tiles == 0)
			return;

		int count = tiles * tiles * 6;
		mBodyIndex[level].indeces = new short[count];

		int & index = mBodyIndex[level].size;
		short * indeces = mBodyIndex[level].indeces;

		// generate triangle strip cw
		//
		int x = 0, y = 0;
		short row_c = start;
		short row_n = row_c + kMeshVertexCount * step;
		for (y = 0; y < tiles; ++y)
		{
			for (x = 0; x < tiles; ++x)
			{
				indeces[index++] = row_n + x * step;
				indeces[index++] = row_c + x * step;
				indeces[index++] = row_n + (x + 1) * step;

				indeces[index++] = row_n + (x + 1) * step;
				indeces[index++] = row_c + x * step;
				indeces[index++] = row_c + (x + 1) * step;
			}

			row_c += kMeshVertexCount * step;
			row_n += kMeshVertexCount * step;
		}

		assert (index == count);
	}

	void Terrain::genConecterIndex()
	{
		for (int i = 0; i < kMaxDetailLevel; ++i)
		{
			for (int j = 0; j < kMaxDetailLevel; ++j)
			{
				_genConecterIndexNorth(i, j);
				_genConecterIndexSouth(i, j);
				_genConecterIndexWest(i, j);
				_genConecterIndexEast(i, j);
			}
		}
	}

	void Terrain::_genConecterIndexNorth(int level, int conecter)
	{
		assert (mConecterIndex[level][conecter][TN_NorthIndex].size == 0 &&
				mConecterIndex[level][conecter][TN_NorthIndex].indeces == NULL);

		if (conecter < level || level == kMaxDetailLevel - 1)
		{
			mConecterIndex[level][conecter][TN_NorthIndex].size = 0;
			mConecterIndex[level][conecter][TN_NorthIndex].indeces = NULL;
			return;
		}

		int self_step = 1 << level;
		int neighbor_step = 1 << conecter;
		int self_tile = kMeshGridCount  >> level;
		int neighbor_tile = kMeshGridCount  >> conecter;

		assert (self_tile >= neighbor_tile);

		int count = self_tile * 2 + 2;

		mConecterIndex[level][conecter][TN_NorthIndex].indeces = new short[count];

		int & index = mConecterIndex[level][conecter][TN_NorthIndex].size;
		short * indeces = mConecterIndex[level][conecter][TN_NorthIndex].indeces;

		// starter
		indeces[index++] = 0;
		indeces[index++] = 0;

		// middler
		for (int i = 1; i < self_tile; ++i)
		{
			int x1 = i * self_step;
			int y1 = self_step;
			int x0 = x1 / neighbor_step * neighbor_step;
			int y0 = y1 - self_step;

			int index0 = y1 * kMeshVertexCount + x1;
			int index1 = y0 * kMeshVertexCount + x0;

			indeces[index++] = index0;
			indeces[index++] = index1;
		}

		//ender
		indeces[index++] = kMeshVertexCount - 1;
		indeces[index++] = kMeshVertexCount - 1;

		assert (index == count);
	}

	void Terrain::_genConecterIndexSouth(int level, int conecter)
	{
		assert (mConecterIndex[level][conecter][TN_SouthIndex].size == 0 &&
				mConecterIndex[level][conecter][TN_SouthIndex].indeces == NULL);

		if (conecter < level || level == kMaxDetailLevel - 1)
		{
			mConecterIndex[level][conecter][TN_SouthIndex].size = 0;
			mConecterIndex[level][conecter][TN_SouthIndex].indeces = NULL;
			return;
		}

		int self_step = 1 << level;
		int neighbor_step = 1 << conecter;
		int self_tile = kMeshGridCount  >> level;
		int neighbor_tile = kMeshGridCount  >> conecter;

		assert (self_tile >= neighbor_tile);

		int count = self_tile * 2 + 2;

		mConecterIndex[level][conecter][TN_SouthIndex].indeces = new short[count];

		int & index = mConecterIndex[level][conecter][TN_SouthIndex].size;
		short * indeces = mConecterIndex[level][conecter][TN_SouthIndex].indeces;

		//starter
		indeces[index++] = kMeshGridCount  * kMeshVertexCount;
		indeces[index++] = kMeshGridCount  * kMeshVertexCount;

		// middler
		for (int i = 1; i < self_tile; ++i)
		{
			int x0 = i * self_step;
			int y0 = kMeshVertexCount - 1 - self_step;
			int x1 = x0 / neighbor_step * neighbor_step;
			int y1 = y0 + self_step;

			int index0 = y1 * kMeshVertexCount + x1;
			int index1 = y0 * kMeshVertexCount + x0;

			indeces[index++] = index0;
			indeces[index++] = index1;
		}

		//ender
		indeces[index++] = kMeshVertexCount * kMeshVertexCount - 1;
		indeces[index++] = kMeshVertexCount * kMeshVertexCount - 1;

		assert (index == count);
	}

	void Terrain::_genConecterIndexWest(int level, int conecter)
	{
		assert (mConecterIndex[level][conecter][TN_WestIndex].size == 0 &&
				mConecterIndex[level][conecter][TN_WestIndex].indeces == NULL);

		if (conecter < level || level == kMaxDetailLevel - 1)
		{
			mConecterIndex[level][conecter][TN_WestIndex].size = 0;
			mConecterIndex[level][conecter][TN_WestIndex].indeces = NULL;
			return;
		}

		int self_step = 1 << level;
		int neighbor_step = 1 << conecter;
		int self_tile = kMeshGridCount  >> level;
		int neighbor_tile = kMeshGridCount  >> conecter;

		assert (self_tile >= neighbor_tile);

		int count = self_tile * 2 + 2;

		mConecterIndex[level][conecter][TN_WestIndex].indeces = new short[count];

		int & index = mConecterIndex[level][conecter][TN_WestIndex].size;
		short * indeces = mConecterIndex[level][conecter][TN_WestIndex].indeces;

		//starter
		indeces[index++] = 0;
		indeces[index++] = 0;

		// middler
		for (int i = 1; i < self_tile; ++i)
		{
			int x0 = 0;
			int y0 = i * self_step / neighbor_step * neighbor_step;
			int x1 = self_step;
			int y1 = i * self_step;

			int index0 = y0 * kMeshVertexCount + x0;
			int index1 = y1 * kMeshVertexCount + x1;

			indeces[index++] = index0;
			indeces[index++] = index1;
		}

		//ender
		indeces[index++] = kMeshGridCount  * kMeshVertexCount;
		indeces[index++] = kMeshGridCount  * kMeshVertexCount;

		assert (index == count);
	}

	void Terrain::_genConecterIndexEast(int level, int conecter)
	{
		assert (mConecterIndex[level][conecter][TN_EastIndex].size == 0 &&
				mConecterIndex[level][conecter][TN_EastIndex].indeces == NULL);

		if (conecter < level || level == kMaxDetailLevel - 1)
		{
			mConecterIndex[level][conecter][TN_EastIndex].size = 0;
			mConecterIndex[level][conecter][TN_EastIndex].indeces = NULL;
			return;
		}

		int self_step = 1 << level;
		int neighbor_step = 1 << conecter;
		int self_tile = kMeshGridCount  >> level;
		int neighbor_tile = kMeshGridCount  >> conecter;

		assert (self_tile >= neighbor_tile);

		int count = self_tile * 2 + 2;

		mConecterIndex[level][conecter][TN_EastIndex].indeces = new short[count];

		int & index = mConecterIndex[level][conecter][TN_EastIndex].size;
		short * indeces = mConecterIndex[level][conecter][TN_EastIndex].indeces;

		//starter
		indeces[index++] = kMeshVertexCount - 1;
		indeces[index++] = kMeshVertexCount - 1;

		// middler
		for (int i = 1; i < self_tile; ++i)
		{
			int x0 = kMeshVertexCount - 1 - self_step;
			int y0 = i * self_step;
			int x1 = kMeshVertexCount - 1;
			int y1 = i * self_step / neighbor_step * neighbor_step;

			int index0 = y0 * kMeshVertexCount + x0;
			int index1 = y1 * kMeshVertexCount + x1;

			indeces[index++] = index0;
			indeces[index++] = index1;
		}

		//ender
		indeces[index++] = kMeshVertexCount * kMeshVertexCount - 1;
		indeces[index++] = kMeshVertexCount * kMeshVertexCount - 1;

		assert (index == count);
	}

	Float3 Terrain::_calcuNormal(int x, int z)
	{
		float flip = 1;
		Float3 here = _getPosition(x, z);
		Float3 right, up;

		if (x < mInfo.VertexCount.x  -1)
		{
			right = _getPosition(x + 1, z);
		}
		else 
		{
			flip *= -1;
			right = _getPosition(x - 1, z);
		}

		if (z < mInfo.VertexCount.y - 1)
		{
			up = _getPosition(x, z + 1);
		}
		else
		{
			flip *= -1;
			up = _getPosition(x, z - 1);
		}

		right -= here;
		up -= here;

		Float3 normal = Float3::Cross(up, right) * flip;
		normal.Normalize();

		return normal;
	}

	void Terrain::_calcuNormals()
	{
		d_assert (mNormals.Size() > 0);

		int index = 0;
		for (int j = 0; j < mInfo.VertexCount.y; ++j)
		{
			for (int i = 0; i < mInfo.VertexCount.x; ++i)
			{
				Float3 n = _calcuNormal(i, j);

				n.x = n.x * 0.5f + 0.5f;
				n.y = n.y * 0.5f + 0.5f;
				n.z = n.z * 0.5f + 0.5f;

				Color c;
				c.r = (unsigned char)(n.x * 255);
				c.g = (unsigned char)(n.y * 255);
				c.b = (unsigned char)(n.z * 255);
				c.a = 255;

				mNormals[index++] = c;
			}
		}
	}

}
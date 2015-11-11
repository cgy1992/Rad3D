#include "MTerrainMesh.h"
#include "MWorld.h"

namespace Rad {

	ImplementRTTI(TerrainMesh, Node);

	TerrainMesh::TerrainMesh()
		: mMorph(0)
		, mUnused(false)
		, mSection(NULL)
	{
		for (int i = 0; i < 4; ++i)
		{
			mNeighbor[i] = NULL;
		}

		for (int i = 0; i < Terrain::kMaxBlendLayers; ++i)
		{
			mLayer[i] = -1;
		}

		for (int i = 0; i < Terrain::kMaxDetailLevel; ++i)
		{
			mErrorMetric[i] = 0;
			mLevelDistSq[i] = MAX_FLOAT;
		}

		mLevel = 0;
		mLodKey.level = 0;
		mLodKey.north = 0;
		mLodKey.south = 0;
		mLodKey.west = 0;
		mLodKey.east = 0;

		mWeightMap = RenderHelper::Instance()->GetWhiteTexture();
		mLightingMap = RenderHelper::Instance()->GetDefaultLightingMap();
	}

	TerrainMesh::~TerrainMesh()
	{
		Unload();
	}

	void TerrainMesh::Init(const Int2 & index)
	{
		mIndex = index;

		const Terrain::Info * ti = Terrain::Instance()->GetInfo();

		if (mIndex.x > 0)
			mNeighbor[TN_WestIndex] = Terrain::Instance()->_getBlock(mIndex.x - 1, mIndex.y);

		if (mIndex.x + 1 < ti->BlockCount.x)
			mNeighbor[TN_EastIndex] = Terrain::Instance()->_getBlock(mIndex.x + 1, mIndex.y);

		if (mIndex.y > 0)
			mNeighbor[TN_SouthIndex] = Terrain::Instance()->_getBlock(mIndex.x, mIndex.y - 1);

		if (mIndex.y + 1 < ti->BlockCount.y)
			mNeighbor[TN_NorthIndex] = Terrain::Instance()->_getBlock(mIndex.x, mIndex.y + 1);

		mLocalAABBValid = false;
		mWorldAABB.minimum.x = ti->BlockSize * mIndex.x;
		mWorldAABB.minimum.z = ti->BlockSize * mIndex.y;
		mWorldAABB.maximum.x = mWorldAABB.minimum.x + ti->BlockSize;
		mWorldAABB.maximum.z = mWorldAABB.minimum.z + ti->BlockSize;
		mWorldAABB.minimum.y = mWorldAABB.maximum.y = Terrain::Instance()->GetInfo()->DefaultHeight;

		mRenderOp.vertexDeclarations[0] = Terrain::Instance()->GetSharedVetexDecl();
		mRenderOp.vertexBuffers[0] = Terrain::Instance()->GetSharedVetexBuffer();

		mRenderOp.primCount = 0;
		mRenderOp.primType = ePrimType::TRIANGLE_LIST;
	}

	void TerrainMesh::Build()
	{
		mRenderOp.vertexDeclarations[1].Clear();
		mRenderOp.vertexDeclarations[1].AddElement(eVertexSemantic::NORMAL, eVertexType::FLOAT4);

		int vertexStride = sizeof(TN_Vertex2);
		int vertexCount = Terrain::kMeshVertexCount * Terrain::kMeshVertexCount;
		mRenderOp.vertexBuffers[1] = HWBufferManager::Instance()->NewVertexBuffer(vertexStride, vertexCount);

		int xStartIndex = mIndex.x * Terrain::kMeshGridCount;
		int zStartIndex = mIndex.y * Terrain::kMeshGridCount;
		int xEndIndex = xStartIndex + Terrain::kMeshGridCount;
		int zEndIndex = zStartIndex + Terrain::kMeshGridCount;
		float minHeight = MAX_FLOAT;
		float maxHeight = MIN_FLOAT;

		TN_Vertex2 * v2 = (TN_Vertex2 *)mRenderOp.vertexBuffers[1]->Lock(eLockFlag::WRITE);
		for (int j = zEndIndex; j >= zStartIndex; --j)
		{
			for (int i = xStartIndex; i <= xEndIndex; ++i)
			{
				v2->normal = Terrain::Instance()->_getNormal(i, j);
				v2->height = Terrain::Instance()->_getHeight(i, j);

				minHeight = Min(minHeight, v2->height);
				maxHeight = Max(maxHeight, v2->height);

				++v2;
			}
		}
		mRenderOp.vertexBuffers[1]->Unlock();

		mRenderOp.primCount = 0;
		mRenderOp.primType = ePrimType::TRIANGLE_LIST;

		mMaterial.maps[eMapType::EXTERN0] = Terrain::Instance()->_getDetailMap(mLayer[0]);
		mMaterial.maps[eMapType::EXTERN1] = Terrain::Instance()->_getDetailMap(mLayer[1]);
		mMaterial.maps[eMapType::EXTERN2] = Terrain::Instance()->_getDetailMap(mLayer[2]);
		mMaterial.maps[eMapType::EXTERN3] = Terrain::Instance()->_getDetailMap(mLayer[3]);

		mMaterial.maps[eMapType::EXTERN4] = Terrain::Instance()->_getNormalMap(mLayer[0]);
		mMaterial.maps[eMapType::EXTERN5] = Terrain::Instance()->_getNormalMap(mLayer[1]);
		mMaterial.maps[eMapType::EXTERN6] = Terrain::Instance()->_getNormalMap(mLayer[2]);
		mMaterial.maps[eMapType::EXTERN7] = Terrain::Instance()->_getNormalMap(mLayer[3]);

		mLevel = 0;
		mLodKey.level = 0;
		mLodKey.north = 0;
		mLodKey.south = 0;
		mLodKey.west = 0;
		mLodKey.east = 0;

		mWorldAABB.minimum.y = minHeight;
		mWorldAABB.maximum.y = maxHeight;

		calcuMorphBuffer();
		calcuErrorMetrics();
		calcuLevelDistance();
	}

	void TerrainMesh::Destroy()
	{
		for (int i = 0; i < Terrain::kMaxDetailLevel; ++i)
		{
			mMorphBuffer[i] = NULL;
		}

		mRenderOp.vertexDeclarations[1].Clear();
		mRenderOp.vertexBuffers[1] = NULL;
		mRenderOp.indexBuffer = NULL;
		mRenderOp.primCount = 0;

		mWeightMap = RenderHelper::Instance()->GetWhiteTexture();
		mLightingMap = RenderHelper::Instance()->GetDefaultLightingMap();
	}

	bool TerrainMesh::IsValid()
	{
		return mRenderOp.vertexBuffers[1] != NULL;
	}

	void TerrainMesh::Serialize(Serializer & sl)
	{
		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			OS << 0x0001;
			OS << mUnused;
			OS << mLayer[0];
			OS << mLayer[1];
			OS << mLayer[2];
			OS << mLayer[3];

			if (!mUnused)
			{
				if (mWeightMap != RenderHelper::Instance()->GetWhiteTexture())
				{
					OS << 0x1001;

					int mapSize = mWeightMap->GetWidth();
					void * data = mWeightMap->Lock(eLockFlag::READ);

					static_memory buffer;
					buffer.query(M_STATIC_MEMORY_I_CHANEL, mapSize * mapSize * 4);

					int length = Math::RLE32_Encode((dword *)buffer.data(), mapSize * mapSize, (dword *)data, mapSize * mapSize);
					if (length != -1 && length < mapSize * mapSize)
					{
						OS << (0x80000000 | mapSize);
						OS << length;
						OS.Write(buffer.data(), length * 4);
					}
					else
					{
						OS << mapSize;
						OS.Write(data, mapSize * mapSize * 4);
					}

					mWeightMap->Unlock();
				}

				if (mLightingMap != RenderHelper::Instance()->GetDefaultLightingMap())
				{
					OS << 0x2001;

					int mapSize = mLightingMap->GetWidth();
					void * data = mLightingMap->Lock(eLockFlag::READ);

					static_memory buffer;
					buffer.query(M_STATIC_MEMORY_I_CHANEL, mapSize * mapSize * 4);

					int length = Math::RLE32_Encode((dword *)buffer.data(), mapSize * mapSize, (dword *)data, mapSize * mapSize);
					if (length != -1 && length < mapSize * mapSize)
					{
						OS << (0x80000000 | mapSize);
						OS << length;
						OS.Write(buffer.data(), length * 4);
					}
					else
					{
						OS << mapSize;
						OS.Write(data, mapSize * mapSize * 4);
					}

					mLightingMap->Unlock();
				}
			}

			/*
			OS << 0x3001;
			TN_Vertex2 * v = (TN_Vertex2 *)mRenderOp.vertexBuffers[1]->Lock(eLockFlag::READ);
			OS.Write(v, sizeof(TN_Vertex2) * Terrain::kMeshVertexCount * Terrain::kMeshVertexCount);
			mRenderOp.vertexBuffers[1]->Unlock();
			*/

			OS << 0; // end
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int ckId;
			while (IS.Read(&ckId, sizeof(ckId)) && ckId != 0)
			{
				if (ckId == 0x0001)
				{
					IS >> mUnused;
					IS >> mLayer[0];
					IS >> mLayer[1];
					IS >> mLayer[2];
					IS >> mLayer[3];
				}
				else if (ckId == 0x1001)
				{
					String name;
					_getWMapName(name);
					mWeightMap = HWBufferManager::Instance()->NewTexture(
						name, Terrain::Instance()->GetInfo()->WMapSize, Terrain::Instance()->GetInfo()->WMapSize);

					int mapSize = 0;
					IS >> mapSize;

					bool compressed = (mapSize & 0x80000000) != 0;
					mapSize &= 0x80000000 - 1;

					TexturePtr temp = mWeightMap; 
					if (mapSize != Terrain::Instance()->GetInfo()->WMapSize)
					{
						temp = HWBufferManager::Instance()->NewTexture("__TN_TempWM", mapSize, mapSize);
					}

					void * data = temp->Lock(eLockFlag::WRITE);
					if (compressed)
					{
						int length = 0;
						IS >> length;

						d_assert (length > 0 && length <= 1024 * 1024 * 4);

						static_memory buffer;
						buffer.query(M_STATIC_MEMORY_I_CHANEL, length * 4);
						IS.Read(buffer.data(), length * 4);

						Math::RLE32_Decode((dword *)data, mapSize * mapSize, (dword *)buffer.data(), length);
					}
					else
					{
						IS.Read(data, 4 * mapSize * mapSize);
					}
					temp->Unlock();

					if (temp != mWeightMap)
					{
						temp->StrechBlt(mWeightMap.c_ptr(), NULL, NULL);
					}
				}
				else if (ckId == 0x2001)
				{
					String name;
					_getLMapName(name);
					mLightingMap = HWBufferManager::Instance()->NewTexture(
						name, Terrain::Instance()->GetInfo()->LMapSize, Terrain::Instance()->GetInfo()->LMapSize);

					int mapSize;
					IS >> mapSize;

					bool compressed = (mapSize & 0x80000000) != 0;
					mapSize &= 0x80000000 - 1;

					TexturePtr temp = mLightingMap;
					if (mapSize != Terrain::Instance()->GetInfo()->LMapSize)
					{
						HWBufferManager::Instance()->NewTexture("__TN_TempLM", mapSize, mapSize);
					}

					void * data = temp->Lock(eLockFlag::WRITE);
					if (compressed)
					{
						int length = 0;
						IS >> length;

						d_assert (length > 0 && length <= 1024 * 1024 * 4);

						static_memory buffer;
						buffer.query(M_STATIC_MEMORY_I_CHANEL, length * 4);
						IS.Read(buffer.data(), length * 4);

						Math::RLE32_Decode((dword *)data, mapSize * mapSize, (dword *)buffer.data(), length);
					}
					else
					{
						IS.Read(data, 4 * mapSize * mapSize);
					}
					temp->Unlock();

					if (temp != mLightingMap)
					{
						temp->StrechBlt(mLightingMap.c_ptr(), NULL, NULL);
					}
				}
				/*
				else if (ckId == 0x3001)
				{
					TN_Vertex2 * v = (TN_Vertex2 *)mRenderOp.vertexBuffers[1]->Lock(eLockFlag::WRITE);
					IS.Read(v, sizeof(TN_Vertex2) * Terrain::kMeshVertexCount * Terrain::kMeshVertexCount);
					mRenderOp.vertexBuffers[1]->Unlock();
				}
				*/
				else
				{
					d_assert (0);
				}
			}
		}
	}

	void TerrainMesh::_preRendering()
	{
		mMaterial.maps[eMapType::DIFFUSE] = mWeightMap;
		mMaterial.maps[eMapType::LIGHTING_MAP] = mLightingMap;

		Float4 UVScale = Float4(0, 0, 0, 0);

		if (mLayer[0] >= 0)
		{
			UVScale.x = Terrain::Instance()->GetLayer(mLayer[0])->UVScale;
		}

		if (mLayer[1] >= 0)
		{
			UVScale.y = Terrain::Instance()->GetLayer(mLayer[1])->UVScale;
		}

		if (mLayer[2] >= 0)
		{
			UVScale.z = Terrain::Instance()->GetLayer(mLayer[2])->UVScale;
		}

		if (mLayer[3] >= 0)
		{
			UVScale.w = Terrain::Instance()->GetLayer(mLayer[3])->UVScale;
		}

		UVScale *= 1.0f / UNIT_METRES;

		mCurrentShaderFX->GetPass(0)->SetConst("u_UVScale", UVScale);
		mCurrentShaderFX->GetPass(0)->SetConst("u_XZTransform", GetXZTransform());
	}

	void TerrainMesh::_postRendering()
	{
		mMaterial.maps[eMapType::DIFFUSE] = NULL;
		mMaterial.maps[eMapType::LIGHTING_MAP] = NULL;
	}

	void TerrainMesh::Update(float elapsedTime)
	{
		Node::Update(elapsedTime);

		Node * cam = World::Instance()->MainCamera();
		float d = mWorldAABB.minimum.DistanceSq(cam->GetPosition());
		d = Min(d, mWorldAABB.maximum.DistanceSq(cam->GetPosition()));

		d -= Terrain::ErrorDistanceBias;

		bool finished = false;
		int max_level = Terrain::kMaxDetailLevel - 1;

		mLevel = 0;
		while (!finished && mLevel < max_level)
		{
			float d0 = mLevelDistSq[mLevel];
			float d1 = mLevelDistSq[mLevel + 1];

			if (d > d1)
			{
				++mLevel;
			}
			else
			{
				finished = true;
			}
		}

		if (Terrain::Instance()->GetInfo()->MorphEnable)
		{
			float start = Terrain::Instance()->GetInfo()->MorphStart;

			if (mLevel == max_level || d < start)
			{
				mMorph = 0.0f;
			}
			else
			{
				float d0 = mLevelDistSq[mLevel];
				float d1 = mLevelDistSq[mLevel + 1];
				mMorph = (d - d0) / (d1 - d0);
			}
		}
	}

	void TerrainMesh::AddRenderQueue(RenderQueue * rq)
	{
		int techId = 0;

		if (mLayer[1] >= 0)
			techId++;
		if (mLayer[2] >= 0)
			techId++;
		if (mLayer[3] >= 0)
			techId++;

		mShaderFX = Terrain::Instance()->_getShaderFX(techId, _hasLightingmap() || mUnused);
		mLightingShaderFX[eLightType::DIRECTION] = Terrain::Instance()->_getLightingShaderFX(eLightType::DIRECTION, techId);
		mLightingShaderFX[eLightType::POINT] = Terrain::Instance()->_getLightingShaderFX(eLightType::POINT, techId);
		mLightingShaderFX[eLightType::SPOT] = Terrain::Instance()->_getLightingShaderFX(eLightType::SPOT, techId);

		//
		Terrain::LodKey key;
		key.level = mLevel;
		key.north = mLevel;
		key.south = mLevel;
		key.west = mLevel;
		key.east = mLevel;

		if (mNeighbor[TN_NorthIndex] != NULL)
		{
			key.north = mNeighbor[TN_NorthIndex]->GetLevel();

			if (key.north < mLevel)
				key.north = mLevel;
		}

		if (mNeighbor[TN_SouthIndex] != NULL)
		{
			key.south = mNeighbor[TN_SouthIndex]->GetLevel();

			if (key.south < mLevel)
				key.south = mLevel;
		}

		if (mNeighbor[TN_WestIndex] != NULL)
		{
			key.west = mNeighbor[TN_WestIndex]->GetLevel();

			if (key.west < mLevel)
				key.west = mLevel;
		}

		if (mNeighbor[TN_EastIndex] != NULL)
		{
			key.east = mNeighbor[TN_EastIndex]->GetLevel();

			if (key.east < mLevel)
				key.east = mLevel;
		}

		if (key != mLodKey || mRenderOp.primCount == 0)
		{
			const Terrain::IndexData & data = Terrain::Instance()->GetIndexData(key);

			mRenderOp.indexBuffer = data.index_buffer;
			mRenderOp.primCount = data.prim_count;
			mRenderOp.primType = data.prim_type;

			mLodKey = key;
		}

		rq->AddRenderObject(this);
	}

	void TerrainMesh::calcuMorphBuffer()
	{
		if (Terrain::Instance()->GetInfo()->MorphEnable)
		{
			int size = Terrain::kMeshVertexCount * Terrain::kMeshVertexCount * sizeof(float);
			for (int i = 0; i < Terrain::kMaxDetailLevel - 1; ++i)
			{
				mMorphBuffer[i] = HWBufferManager::Instance()->NewVertexBuffer(size, sizeof(float));
				_calcuMorphBuffer(i);
			}
		}
	}

	void TerrainMesh::_calcuMorphBuffer(int level)
	{
		VertexBufferPtr buffer = mMorphBuffer[level++];

		int step = 1 << level;

		int xSectionVertSize = Terrain::kMeshVertexCount;
		int ySectionVertSize = Terrain::kMeshVertexCount;

		int xSides = (xSectionVertSize - 1) >> level;
		int ySides = (ySectionVertSize - 1) >> level;

		float err = 0.0f;

		float * data = (float *)buffer->Lock(eLockFlag::WRITE);

		memset(data, 0, buffer->GetCount() * buffer->GetStride());

		// 求水平方向的误差值
		for (int y = step; y < ySectionVertSize - step; y += step)
		{
			for (int x = 0; x < xSides; ++x)
			{
				int x0 = x * step;
				int x1 = x0 + step;

				int xm = (x1 + x0) / 2;

				float h0 = GetHeight(x0, y);
				float h1 = GetHeight(x1, y);
				float hm = GetHeight(xm, y);
				float hmi = (h0 + h1) / 2;

				float delta = hmi - hm;

				data[TN_MAP(xm, y, xSectionVertSize)] = delta;
			}
		}

		//垂直方向
		for (int x = step; x < xSectionVertSize - step; x += step)
		{
			for (int y = 0; y < ySides; ++y)
			{
				int y0 = y * step;
				int y1 = y0 + step;

				int ym = (y0 + y1) / 2;

				float h0 = GetHeight(x, y0);
				float h1 = GetHeight(x, y1);
				float hm = GetHeight(x, ym);
				float hmi = (h0 + h1) / 2;

				float delta = hmi - hm;

				data[TN_MAP(x, ym, xSectionVertSize)] = delta;
			}
		}

		//中点
		for (int y = 0; y < ySides; ++y)
		{
			int y0 = y * step;
			int y1 = y0 + step;
			int ym = (y0 + y1) / 2;

			for (int x = 0; x < xSides; ++x)
			{
				int x0 = x * step;
				int x1 = x0 + step;
				int xm = (x0 + x1) / 2;

				float h0 = GetHeight(x0, y0);
				float h1 = GetHeight(x1, y1);
				float hm = GetHeight(xm, ym);
				float hmi = (h0 + h1) / 2;

				float delta = hmi - hm;

				data[TN_MAP(xm, ym, xSectionVertSize)] = delta;
			}
		}

		buffer->Unlock();
	}

	void TerrainMesh::calcuErrorMetrics()
	{
		mErrorMetric[0] = 0;

		for (int i = 1; i < Terrain::kMaxDetailLevel; ++i)
		{
			mErrorMetric[i] = _calcuErrorMetric(i);
		}

		//lod大的必须误差值必须大
		for (int i = 2; i < Terrain::kMaxDetailLevel; ++i)
		{
			mErrorMetric[i] = Max(mErrorMetric[i], mErrorMetric[i - 1]);
		}
	}

	float TerrainMesh::_calcuErrorMetric(int level)
	{
		///////////////////////////////////////////////////////////////////////////
		// 求每个级别的最大误差值
		// 对于该栅格求误差值, 取最大值.
		// 
		//
		int step = 1 << level;

		int xSectionVertSize = Terrain::kMeshVertexCount;
		int ySectionVertSize = Terrain::kMeshVertexCount;

		int xSides = (xSectionVertSize - 1) >> level;
		int ySides = (ySectionVertSize - 1) >> level;

		float err = 0.0f;

		// 求水平方向的误差值
		for (int y = 0; y < ySectionVertSize; y += step)
		{
			for (int x = 0; x < xSides; ++x)
			{
				int x0 = x * step;
				int x1 = x0 + step;

				int xm = (x1 + x0) / 2;

				float h0 = GetHeight(x0, y);
				float h1 = GetHeight(x1, y);
				float hm = GetHeight(xm, y);
				float hmi = (h0 + h1) / 2;

				float delta = Math::Abs(hm - hmi);

				err = Max(err, delta);
			}
		}

		//垂直方向
		for (int x = 0; x < xSectionVertSize; x += step)
		{
			for (int y = 0; y < ySides; ++y)
			{
				int y0 = y * step;
				int y1 = y0 + step;

				int ym = (y0 + y1) / 2;

				float h0 = GetHeight(x, y0);
				float h1 = GetHeight(x, y1);
				float hm = GetHeight(x, ym);
				float hmi = (h0 + h1) / 2;

				float delta = Math::Abs(hm - hmi);

				err = Max(err, delta);
			}
		}

		//中点
		for (int y = 0; y < ySides; ++y)
		{
			int y0 = y * step;
			int y1 = y0 + step;
			int ym = (y0 + y1) / 2;

			for (int x = 0; x < xSides; ++x)
			{
				int x0 = x * step;
				int x1 = x0 + step;
				int xm = (x0 + x1) / 2;

				float h0 = GetHeight(x0, y0);
				float h1 = GetHeight(x1, y1);
				float hm = GetHeight(xm, ym);
				float hmi = (h0 + h1) / 2;

				float delta = Math::Abs(hm - hmi);

				err = Max(err, delta);
			}
		}

		return err;
	}

	void TerrainMesh::calcuLevelDistance()
	{
		const float pixelerr = Terrain::PixelError;
		const float resolution = 768;
;
		float c = 1.0f / (2 * pixelerr / resolution);

		for (int i = 0; i < Terrain::kMaxDetailLevel; ++i)
		{
			float e = mErrorMetric[i];
			float d = e * c;
			mLevelDistSq[i] = d * d;
		}

		if (Terrain::Instance()->GetInfo()->MorphEnable)
		{
			for (int i = 1; i < Terrain::kMaxDetailLevel - 1; ++i)
			{
				float d0 = mLevelDistSq[i - 1];
				float d1 = mLevelDistSq[i];
				float d2 = mLevelDistSq[i + 1];

				if (d2 - d1 < EPSILON_E4)
				{
					d1 =  d0 + 0.5f * (d2 - d0);
					mLevelDistSq[i] = d1;
				}
			}
		}
	}

	void TerrainMesh::_getWMapName(String & name)
	{
		name.Format("TN_WM_%d_%d", mIndex.x, mIndex.y);
	}

	void TerrainMesh::_getLMapName(String & name)
	{
		name.Format("TN_LM_%d_%d", mIndex.x, mIndex.y);
	}

	void TerrainMesh::SetUnused(bool unused)
	{
		mUnused = unused;
		if (unused)
		{
			mWeightMap = RenderHelper::Instance()->GetWhiteTexture();
			mLightingMap = RenderHelper::Instance()->GetDefaultLightingMap();

			mLayer[0] = 0;
			mLayer[1] = -1;
			mLayer[2] = -1;
			mLayer[3] = -1;
		}
	}

	Float4 TerrainMesh::GetXZTransform()
	{
		Float4 tm;
		tm.x = mWorldAABB.minimum.x;
		tm.y = mWorldAABB.minimum.z;
		tm.w = Terrain::Instance()->GetInfo()->GridSize;

		return tm;
	}

	float TerrainMesh::GetHeight(int x, int z)
	{
		d_assert (x <= Terrain::kMeshGridCount && z <= Terrain::kMeshGridCount);

		float height = 0;

		if (IsValid())
		{
			TN_Vertex2 * v = (TN_Vertex2 *)mRenderOp.vertexBuffers[1]->Lock(eLockFlag::READ);
			height = v[TN_MAP(x, z, Terrain::kMeshVertexCount)].height;
			mRenderOp.vertexBuffers[1]->Unlock();
		}

		return height;
	}

	Float3 TerrainMesh::GetNormal(int x, int z)
	{
		d_assert (x <= Terrain::kMeshGridCount && z <= Terrain::kMeshGridCount);

		Float3 normal = Float3(0, 1, 0);

		if (IsValid())
		{
			TN_Vertex2 * v = (TN_Vertex2 *)mRenderOp.vertexBuffers[1]->Lock(eLockFlag::READ);
			normal = v[TN_MAP(x, z, Terrain::kMeshVertexCount)].normal;
			mRenderOp.vertexBuffers[1]->Unlock();
		}
		
		return normal;
	}

	Rgba32 TerrainMesh::GetWeight(int x, int z)
	{
		Rgba32 color = Rgba32(255, 0, 0, 0);

		if (mWeightMap != RenderHelper::Instance()->GetWhiteTexture())
		{
			int mapSize = Terrain::Instance()->GetInfo()->WMapSize;

			d_assert (x < mapSize && z < mapSize);

			Rgba32 * data = (Rgba32 *)mWeightMap->Lock(eLockFlag::READ);

			color = data[TN_MAP(x, z, mapSize)];

			mWeightMap->Unlock();
		}

		return color;
	}

	Rgba32 TerrainMesh::GetLightingColor(int x, int z)
	{
		Rgba32 color = Rgba32(255, 255, 255, 0);

		if (mLightingMap != RenderHelper::Instance()->GetDefaultLightingMap())
		{
			int mapSize = Terrain::Instance()->GetInfo()->LMapSize;

			d_assert (x < mapSize && z < mapSize);

			Rgba32 * data = (Rgba32 *)mLightingMap->Lock(eLockFlag::READ);

			color = data[TN_MAP(x, z, mapSize)];

			mLightingMap->Unlock();
		}

		return color;
	}

	void TerrainMesh::_updateHeight(const RectI & rc)
	{
		if (!IsValid())
			return ;

		int xtile = Terrain::kMeshGridCount;
		int ztile = Terrain::kMeshGridCount;

		RectI myRect;
		myRect.x1 = mIndex.x * xtile;
		myRect.y1 = mIndex.y * ztile;
		myRect.x2 = mIndex.x * xtile + xtile;
		myRect.y2 = mIndex.y * ztile + ztile;

		if (rc.x1 > myRect.x2 || rc.x2 < myRect.x1 ||
			rc.y1 > myRect.y2 || rc.y2 < myRect.y1)
			return ;

		for (int i = 0; i < Terrain::kMaxDetailLevel; ++i)
		{
			mMorphBuffer[i] = NULL;
		}

		mRenderOp.vertexDeclarations[1].Clear();
		mRenderOp.vertexBuffers[1] = NULL;
		mRenderOp.indexBuffer = NULL;
		mRenderOp.primCount = 0;

		Build();
	}

	void TerrainMesh::_updateLayers()
	{
		mMaterial.maps[eMapType::EXTERN0] = Terrain::Instance()->_getDetailMap(mLayer[0]);
		mMaterial.maps[eMapType::EXTERN1] = Terrain::Instance()->_getDetailMap(mLayer[1]);
		mMaterial.maps[eMapType::EXTERN2] = Terrain::Instance()->_getDetailMap(mLayer[2]);
		mMaterial.maps[eMapType::EXTERN3] = Terrain::Instance()->_getDetailMap(mLayer[3]);

		mMaterial.maps[eMapType::EXTERN4] = Terrain::Instance()->_getNormalMap(mLayer[0]);
		mMaterial.maps[eMapType::EXTERN5] = Terrain::Instance()->_getNormalMap(mLayer[1]);
		mMaterial.maps[eMapType::EXTERN6] = Terrain::Instance()->_getNormalMap(mLayer[2]);
		mMaterial.maps[eMapType::EXTERN7] = Terrain::Instance()->_getNormalMap(mLayer[3]);
	}

	void TerrainMesh::_updateWeightMap(const RectI & rc, const Array<Rgba32> & data)
	{
		if (!IsValid() || mUnused)
			return ;

		int mapSize = Terrain::Instance()->GetInfo()->WMapSize;

		RectI myRect;
		myRect.x1 = mIndex.x * mapSize;
		myRect.y1 = mIndex.y * mapSize;
		myRect.x2 = mIndex.x * mapSize + mapSize - 1;
		myRect.y2 = mIndex.y * mapSize + mapSize - 1;

		if (rc.x1 > myRect.x2 || rc.x2 < myRect.x1 ||
			rc.y1 > myRect.y2 || rc.y2 < myRect.y1)
			return ;

		if (mWeightMap == RenderHelper::Instance()->GetWhiteTexture())
		{
			String name;
			_getWMapName(name);
			mWeightMap = HWBufferManager::Instance()->NewTexture(name, mapSize, mapSize);

			Rgba32 * mapData = (Rgba32 *)mWeightMap->Lock(eLockFlag::WRITE);
			for (int i = 0; i < mapSize * mapSize; ++i)
			{
				mapData[i] = Rgba32(255, 0, 0, 0);
			}
			mWeightMap->Unlock();
		}

		int sx = Max(myRect.x1, rc.x1);
		int ex = Min(myRect.x2, rc.x2);
		int sy = Max(myRect.y1, rc.y1);
		int ey = Min(myRect.y2, rc.y2);

		Rgba32 * mapData = (Rgba32 *)mWeightMap->Lock(eLockFlag::READ_WRITE);
		for (int j = sy; j <= ey; ++j)
		{
			for (int i = sx; i <= ex; ++i)
			{
				int x = i - myRect.x1;
				int y = j - myRect.y1;
				int x1 = i - rc.x1;
				int y1 = j - rc.y1;

				d_assert (x < mapSize && y < mapSize);

				mapData[TN_MAP(x, y, mapSize)] = data[y1 * (rc.Width() + 1) + x1];
			}
		}
		mWeightMap->Unlock();

		_updateLayers();
	}

	void TerrainMesh::_updateLightingMap(const Array<Rgba32> & lightingColor)
	{
		if (!IsValid() || mUnused)
			return ;

		mLightingMap = RenderHelper::Instance()->GetDefaultLightingMap();

		int mapSize = Terrain::Instance()->GetInfo()->LMapSize;
		int dataRows = mapSize * Terrain::Instance()->GetInfo()->BlockCount.x;

		String name;
		_getLMapName(name);
		mLightingMap = HWBufferManager::Instance()->NewTexture(name, mapSize, mapSize);

		const Rgba32 * data = &lightingColor[mapSize * mIndex.y * dataRows + mapSize * mIndex.x];
		Rgba32 * color = (Rgba32 *)mLightingMap->Lock(eLockFlag::WRITE);
		for (int j = 0; j < mapSize; ++j)
		{
			memcpy(&color[TN_MAP(0, j, mapSize)], data, mapSize * sizeof(Rgba32));

			data += dataRows;
		}
		mLightingMap->Unlock();
	}

	void TerrainMesh::_resetLighting()
	{
		mLightingMap = RenderHelper::Instance()->GetDefaultLightingMap();
	}

}
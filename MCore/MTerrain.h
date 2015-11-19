/*
*	Terrain
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderSystem.h"
#include "MTerrainSection.h"

namespace Rad {

#define TN_NorthIndex    0
#define TN_SouthIndex    1
#define TN_WestIndex     2
#define TN_EastIndex     3

	struct TN_Vertex1 {
		Float2 xz;
		Float2 uv;
	};

	struct TN_Vertex2 {
		Float3 normal;
		float height;
	};

	class M_ENTRY Terrain : public Singleton<Terrain>
	{
		DECLARE_ALLOC();

	public:
		static float PixelError;
		static float ErrorDistanceBias;

	public:
		enum {
			kMaxBlendLayers = 4,
			kMeshGridCount = 32,
			kMeshVertexCount = kMeshGridCount + 1,
			kMaxDetailLevel = 4,
			kMaxWaters = 8,
		};

		enum {
			K_FILE_MAGIC = 0x10203040,
			K_VERSION = 0x00001001,
		};

		struct Info : public IObject
		{
			DECLARE_PROPERTY(IObject);

			bool MorphEnable;
			float MorphStart;

			Int2 BlocksPerSection;
			float DefaultHeight;

			int WMapSize;
			int LMapSize;

			Int2 VertexCount;
			Int2 GridCount;
			Int2 BlockCount;

			float GridSize;
			float InvGridSize;
			float BlockSize;
			float InvBlockSize;
			Float2 Size;
			Float2 InvSize;

			Info()
			{
				MorphEnable = false;
				MorphStart = 100 * UNIT_METRES;

				BlocksPerSection = Int2(4, 4);
				DefaultHeight = 100;

				WMapSize = 128;
				LMapSize = 128;

				GridSize = UNIT_METRES;
				InvGridSize = 1 / GridSize;
				BlockSize = GridSize * kMeshGridCount;
				InvBlockSize = 1 / BlockSize;
			}
		};

		struct Layer : public IObject
		{
			DECLARE_PROPERTY(IObject);

			String DetailMap;
			String NormalMap;
			float UVScale;
			int MaterialId;

			Layer() : UVScale(0.5f), MaterialId(-1) {}
		};

	public:
		struct IndexPool
		{
			int size;
			short * indeces;

			IndexPool() : size(0), indeces(0) {}
		};

		struct IndexData
		{
			int prim_count;
			ePrimType prim_type;
			int start;
			int index_count;
			IndexBufferPtr index_buffer;

			IndexData() : prim_count(0), prim_type(ePrimType::TRIANGLE_STRIP),
				start(0), index_count(0) {}
		};

		struct LodKey
		{
			union
			{
				struct {
					unsigned char level;
					unsigned char north;
					unsigned char south;
					unsigned char west;
					unsigned char east;
					unsigned char unused[3];
				};

				uint64_t value;
			};

			LodKey() : value(0) {}
			~LodKey() {}

			LodKey(const LodKey & k) : value(k.value) {}

			LodKey & operator = (const LodKey & r)
			{
				value = r.value;
				return *this;
			}

			bool operator < (const LodKey & r) const
			{
				return value < r.value;
			}

			bool operator <= (const LodKey & r) const
			{
				return value <= r.value;
			}

			bool operator > (const LodKey & r) const
			{
				return value > r.value;
			}

			bool operator >= (const LodKey & r) const
			{
				return value >= r.value;
			}

			bool operator == (const LodKey & r) const
			{
				return value == r.value;
			}

			bool operator != (const LodKey & r) const
			{
				return value != r.value;
			}
		};

	public:
		Terrain();
		virtual ~Terrain();

		void
			SetMorphEnable(bool enable) { mInfo.MorphEnable = enable; }
		void
			SetMorphStart(float dist) { mInfo.MorphStart = dist; }

		const Info *		
			GetInfo() const { return &mInfo; }

		const Layer *		
			GetLayer(int index) { return &mLayers[index]; }
		int					
			GetLayerCount() { return mLayers.Size(); }

		bool				
			IsValid();
		bool				
			Load(const String & filename);
		void
			Unload();
		void				
			Save(const String & filename);
		void
			EnsureBuild();

		TexturePtr			
			_getDetailMap(int layer);
		TexturePtr			
			_getNormalMap(int layer);
		ShaderFX *			
			_getShaderFX(int maxlayer, bool lightingmap);
		ShaderFX *			
			_getLightingShaderFX(eLightType type, int maxlayer);

		TerrainMesh *
			_getBlock(int i, int j);
		void
			_getActiveBlocks(Array<TerrainMesh *> & arr);
		TerrainSection *
			_getCurrentSection();

		Float3
			_getPosition(int i, int j);
		float
			_getHeight(int i, int j);
		float
			_getHeightClamp(int i, int j);
		Float3
			_getNormal(int i, int j);
		Rgba32
			_getWeight(int i, int j);
		Rgba32
			_getLightingColor(int i, int j);

		bool
			MapBlockIndex(int & blockX, int & blockZ, int & localX, int & localZ, int x, int z, int mapSize);

		bool				
			GetHeightAt(float & h, float x, float y);
		bool			
			GetNormalAt(Float3 & n, float x, float y);
		bool			
			GetLightingColorAt(Float3 & c, float x, float z);

		bool
			RayCheck(Float3 * p, const Ray & ray, float step = UNIT_METRES * 0.1f);

		float *
			LockHeight(const RectI & rect);
		void
			UnlockHeight();

		float *				
			LockWeightMap(const RectI & rc);
		void				
			UnlockWeightMap(int layer);

		void				
			SetLightingMap(Array<Rgba32> & color);
		void				
			ResetLighting();

		const IndexData &  
			GetIndexData(const LodKey & k);
		const VertexDeclaration &
			GetSharedVetexDecl() { return mShaderVertexDecl; }
		VertexBufferPtr
			GetSharedVetexBuffer() { return mSharedVertexBuffer; }

	protected:
		void
			_OnLoadSetting();

		void
			genXZUVBuffer();
		void                
			genBodyIndex();
		void               
			_genBodyIndex(int level);
		void               
			genConecterIndex();
		void                
			_genConecterIndexNorth(int level, int conecter);
		void                
			_genConecterIndexSouth(int level, int conecter);
		void                
			_genConecterIndexWest(int level, int conecter);
		void                
			_genConecterIndexEast(int level, int conecter);

		Float3				
			_calcuNormal(int x, int z);
		void				
			_calcuNormals();

	protected:
		Info mInfo;
		Array<Layer> mLayers;

		VertexDeclaration mShaderVertexDecl;
		VertexBufferPtr mSharedVertexBuffer;
		IndexPool mBodyIndex[Terrain::kMaxDetailLevel];
		IndexPool mConecterIndex[Terrain::kMaxDetailLevel][Terrain::kMaxDetailLevel][4];
		Map<LodKey, IndexData> mIndexBufferMap;

		Field<ShaderFX *, kMaxBlendLayers> mShaderFX;
		Field<ShaderFX *, kMaxBlendLayers> mLightingMapShaderFX;
		Field<ShaderFX *, kMaxBlendLayers> mLightingShaderFX[eLightType::MAX];

		Array<float> mHeights;
		Array<Rgb24> mNormals;

		TerrainMesh * mMeshes;

		// for editor
		RectI mLockedRect;
		Array<float> mLockedData;

		RectI mLockedWeightRect;
		Array<float> mLockedWeightData;
	};

}

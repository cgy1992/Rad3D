/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LightFX_AOGenerator.h"
#include "LightFX_Rasterizer.h"
#include "LightFX_Light.h"
#include "LightFX_Mesh.h"
#include "LightFX_Terrain.h"
#include "LightFX_Grass.h"

namespace Rad {

	class FX_Thread : public Thread
	{
	public:
		FX_Thread() {}
		virtual ~FX_Thread() {}

		virtual void
			Run();
	};

	class LIGHTFX_ENTRY FX_World : public Singleton<FX_World>
	{
	public:
		enum {
			STAGE_START,
			STAGE_VERTEX_COLOR,

			STAGE_DIRECT_LIGHTING_MESH,
			STAGE_DIRECT_LIGHTING_TERRAIN,
			STAGE_DIRECT_LIGHTING_GRASS,

			STAGE_INDIRECT_LIGHTING_MESH,
			STAGE_INDIRECT_LIGHTING_TERRAIN,
			STAGE_INDIRECT_LIGHTING_GRASS,

			STAGE_AMBIENT_OCCLUSION_MESH,
			STAGE_AMBIENT_OCCLUSION_TERRAIN,
			STAGE_AMBIENT_OCCLUSION_GRASS,
			
			STAGE_FINISH,
			STAGE_END,
		};

	public:
		FX_World();
		~FX_World();

		FX_Setting * 
			GetSetting() { return &mSetting; }

		void 
			Clear();

		FX_Light * 
			NewLight();
		FX_Light *
			GetLight(int i) { return mLights[i]; }
		int
			GetLightCount() { return mLights.Size(); }

		FX_Mesh * 
			NewMesh();
		FX_Mesh * 
			GetMesh(int i) { return mMeshes[i]; }
		int
			GetMeshCount() { return mMeshes.Size(); }

		FX_Terrain * 
			NewTerrain(Terrain * terrain);
		FX_Terrain * 
			GetTerrain() { return mTerrain; }

		FX_GrassManager * 
			NewGrassManager();
		FX_GrassManager * 
			GetGrassManager() { return mGrassManager; }

		void
			Run();
		void
			_threadProc();
		int
			GetStage() { return mStage; }
		void
			SetProgress(int p) { mProgress = p; }
		int
			GetProgress() { return mProgress; }

		bool 
			RayCheck(FX_Contract & contract, const Ray & ray, float len, int flags);

	protected:
		void 
			_build();
		
		void 
			_calcuVertexColor();
		void 
			_calcuDirectLighting();
		void 
			_calcuIndirectLighting();
		void 
			_calcuAmbientOcclusion();
		void 
			_finish();

	protected:
		FX_Setting mSetting;
		FX_AOGenerator mAOGenerator;
		
		Array<FX_Light *> mLights;
		Array<FX_Mesh *> mMeshes;
		FX_Terrain * mTerrain;
		FX_GrassManager * mGrassManager;

		KDTree<FX_Mesh *> mKDTree;

		FX_Thread * mThread;
		int mStage;
		int mProgress;
	};

}
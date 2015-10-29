#include "LightFX_World.h"

namespace Rad {

	void FX_Thread::Run()
	{
		FX_World::Instance()->_threadProc();
	}

	//
	ImplementSingleton(FX_World);

	FX_World::FX_World()
		: mTerrain(NULL)
		, mGrassManager(NULL)
	{
		mThread = NULL;
	}

	FX_World::~FX_World()
	{
		Clear();
	}

	void FX_World::Clear()
	{
		safe_delete (mThread);

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			delete mMeshes[i];
		}
		mMeshes.Clear();

		for (int i = 0; i < mLights.Size(); ++i)
		{
			delete mLights[i];
		}
		mLights.Clear();

		safe_delete (mTerrain);

		safe_delete (mGrassManager);
	}

	FX_Light * FX_World::NewLight()
	{
		FX_Light * pLight = new FX_Light;

		mLights.PushBack(pLight);

		return pLight;
	}

	FX_Mesh * FX_World::NewMesh()
	{
		FX_Mesh * pMesh = new FX_Mesh;

		mMeshes.PushBack(pMesh);

		return pMesh;
	}

	FX_Terrain * FX_World::NewTerrain(Terrain * terrain)
	{
		d_assert (mTerrain == NULL);

		mTerrain = new FX_Terrain(terrain);

		return mTerrain;
	}

	FX_GrassManager * FX_World::NewGrassManager()
	{
		d_assert (mGrassManager == NULL);

		mGrassManager = new FX_GrassManager;

		return mGrassManager;
	}

	void FX_World::Run()
	{
		_build();

		mStage = STAGE_START;
		mProgress = 0;

		safe_delete (mThread);
		mThread = new FX_Thread;
	}

	void FX_World::_threadProc()
	{
		_calcuVertexColor();
		_calcuDirectLighting();
		_calcuIndirectLighting();

		if (mSetting.AOEnable)
			_calcuAmbientOcclusion();

		_finish();

		mStage = STAGE_END;
	}

	bool _addMesh(KDTree<FX_Mesh *>::Node * node, FX_Mesh * mesh)
	{
		Aabb bound = mesh->GetBound();

		if (node->aabb.Contain(bound))
		{
			if (node->child[0] != NULL && _addMesh(node->child[0], mesh))
				return true;

			if (node->child[1] != NULL && _addMesh(node->child[1], mesh))
				return true;

			node->elems.PushBack(mesh);

			return true;
		}

		return false;
	}

	void FX_World::_build()
	{
		// Build collision.
		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			mMeshes[i]->BuildKDTree();
		}

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			if (!mMeshes[i]->Valid())
			{
				delete mMeshes[i];
				mMeshes.Erase(i--);
			}
		}
		
		Aabb worldBound = Aabb::Invalid;

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			Aabb bound = mMeshes[i]->GetBound();

			worldBound.Merge(bound);
		}
		
		if (!worldBound.Valid())
			return ;

		mKDTree.Build(worldBound, 4);

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			_addMesh(mKDTree.GetRoot(), mMeshes[i]);
		}

		if (mTerrain != NULL)
		{
			mTerrain->BuildKDTree();
		}
	}

	void FX_World::_calcuVertexColor()
	{
		mStage = STAGE_VERTEX_COLOR;
		mProgress = 0;

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			FX_Mesh * pMesh = mMeshes[i];

			if (pMesh->GetLightingMode() == eStaticLightingMode::LIGHTING_COLOR)
			{
				pMesh->CalcuVertexColor();
			}

			mProgress = (int)(i / (float)mMeshes.Size() * 100.0f);
		}
	}

	void FX_World::_calcuDirectLighting()
	{
		mStage = STAGE_DIRECT_LIGHTING_MESH;
		mProgress = 0;

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			FX_Mesh * pMesh = mMeshes[i];

			if (pMesh->GetLightingMode() == eStaticLightingMode::LIGHTING_MAP)
			{
				pMesh->CalcuLightingMap();
			}

			mProgress = (int)(i / (float)mMeshes.Size() * 100.0f);
		}

		mStage = STAGE_DIRECT_LIGHTING_TERRAIN;
		mProgress = 0;

		if (mTerrain != NULL)
			mTerrain->CalcuLightingMap();

		mStage = STAGE_DIRECT_LIGHTING_GRASS;
		mProgress = 0;

		if (mGrassManager != NULL)
			mGrassManager->CalcuVertexColor();
	}

	void FX_World::_calcuIndirectLighting()
	{
	}

	void FX_World::_calcuAmbientOcclusion()
	{
		mStage = STAGE_AMBIENT_OCCLUSION_MESH;
		mProgress = 0;

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			FX_Mesh * pMesh = mMeshes[i];

			if (pMesh->GetLightingMode() == eStaticLightingMode::LIGHTING_MAP)
			{
				pMesh->CalcuAmbientOcclusion();
			}

			mProgress = (int)(i / (float)mMeshes.Size() * 100.0f);
		}

		mStage = STAGE_AMBIENT_OCCLUSION_TERRAIN;
		mProgress = 0;

		if (mTerrain != NULL)
			mTerrain->CalcuAmbientOcclusion();

		mStage = STAGE_AMBIENT_OCCLUSION_GRASS;

		if (mGrassManager != NULL)
			mGrassManager->CalcuAmbientOcclusion();
	}

	void FX_World::_finish()
	{
		mStage = STAGE_FINISH;
		mProgress = 0;

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			FX_Mesh * pMesh = mMeshes[i];

			pMesh->Finish();

			mProgress = (int)(i / (float)mMeshes.Size() * 100.0f);
		}

		if (mTerrain != NULL)
			mTerrain->Finish();
	}

	void _rayCheck(FX_Contract & contract, KDTree<FX_Mesh *>::Node * node, const Ray & ray, float len)
	{
		float dist = 0;

		if (!ray.Intersect(&dist, node->aabb) || contract.td < dist)
			return ;

		for (int i = 0; i < node->elems.Size(); ++i)
		{
			node->elems[i]->RayCheck(contract, ray, len);
		}

		if (node->child[0] != NULL)
		{
			_rayCheck(contract, node->child[0], ray, len);
		}

		if (node->child[1] != NULL)
		{
			_rayCheck(contract, node->child[1], ray, len);
		}
	}

	bool FX_World::RayCheck(FX_Contract & contract, const Ray & ray, float len, int flags)
	{
		contract.td = MAX_FLOAT;
		contract.tu = 0;
		contract.tv = 0;
		contract.triIndex = -1;
		contract.entity = NULL;

		if ((flags & FX_Flag::MESH) && mKDTree.GetRoot() != NULL)
			_rayCheck(contract, mKDTree.GetRoot(), ray, len);

		return contract.entity != NULL;
	}

}
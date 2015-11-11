/*
*	Word
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRoot.h"
#include "MUtil.h"
#include "MCodeTimer.h"
#include "MNode.h"
#include "MCamera.h"
#include "MMesh.h"
#include "MLight.h"
#include "MBillboard.h"
#include "MParticleSystem.h"
#include "MSound.h"
#include "MTerrain.h"
#include "MWater.h"
#include "MPrefab.h"
#include "MZone.h"
#include "MRenderPipeline.h"
#include "MRenderContext.h"
#include "MNodeTracker.h"
#include "MNodeDisplayer.h"
#include "MSimpleCollision.h"
#include "MGrassManager.h"
#include "MResourceLoaderMT.h"
#include "MLoadRule.h"
#include "NavData.h"
#include "NavPathFinder.h"
#include "PSManager.h"
#include "MMeshManager.h"
#include "MPrefabManager.h"
#include "MWorldSection.h"
#include "MConfigFile.h"

namespace Rad {

	class M_ENTRY World : public Singleton<World>
	{
		friend class Node;

	public:
		tEvent0 E_Init;
		tEvent0 E_Shutdown;
		tEvent1<float> E_Update;

		tEvent0 E_VisibleCullBegin;
		tEvent0 E_VisibleCullEnd;

		tEvent0 E_RenderingBegin;
		tEvent0 E_RenderingEnd;

		tEvent0 E_RenderSolidBegin;
		tEvent0 E_RenderSolid;
		tEvent0 E_RenderSolidEnd;

		tEvent0 E_RenderLighting;
		;
		tEvent0 E_RenderAlphaBegin;
		tEvent0 E_RenderAlpha;
		tEvent0 E_RenderAlphaEnd;

		tEvent0 E_Load;
		tEvent0 E_Save;
		tEvent0 E_Unload;
		tEvent0 E_LoadSetting;

		tEvent2<WorldSection *, Node *> E_SectionAddNode;
		tEvent2<WorldSection *, Node *> E_SectionRemoveNode;
		tEvent1<WorldSection *> E_SectionLoaded;
		tEvent1<WorldSection *> E_SectionUnload;
		tEvent1<WorldSection *> E_SectionCurrentChanged;

	public:
		struct Info : public IObject
		{
			DECLARE_PROPERTY(IObject);

			Int2 SectionCount;
			Float3 SectionSize;
			Int3 SectionZoneTile;
			int SectionZoneDepth;
		};

		struct EvInfo : public IObject
		{
			DECLARE_PROPERTY(IObject);

			Float3 MainLightRotation;
			Float3 MainLightAmbient;
			Float3 MainLightDiffuse;
			Float3 MainLightSpecular;
			float MainLightStrength;

			float FogStart;
			float FogEnd;
			Float3 FogColor;

			Float2 GrassWaveDir;
			float GrassWaveSpeed;
			float GrassWaveStrength;
			float GrassVisibleRadius;

			EvInfo();

			void
				Reset();
			virtual void
				OnPropertyChanged(const Property * p);
		};

	public:
		World();
		~World();

		int 
			FrameId() { return mFrameId; }

		Camera * 
			MainCamera() { return mMainCamera; }
		Zone * 
			MainZone() { return mMainZone; }
		Light * 
			MainLight() { return mMainLight; }
		RenderContext * 
			MainRenderContext() { return mMainRenderContext; }

		NavData *
			GetNavData() { return mNavData; }

		void
			EnableMultiThreadLoad();
		void
			DisableMultiThreadLoad();
		bool
			IsMultiThreadLoad();

		void
			SetLoadRule(ILoadRule * rule);
		ILoadRule *
			GetLoadRule() { return mLoadRule; }
		
		void
			Load(const String & filename);
		void
			Unload();
		void
			Save(const String & filename);
		void
			_updateEvInfo();

		const Info *
			GetInfo() { return &mInfo; }
		EvInfo *
			GetEvInfo() { return &mEvInfo; }
		ConfigFile *
			GetSetting() { return &mSetting; }
		const String &
			GetFilename() { return mFilename; }

		WorldSection *
			GetSection(const Float3 & position);
		WorldSection *
			GetSection(int x, int y);
		WorldSection *
			GetBestSection(const Float3 & position);
		WorldSection *
			GetCurrentSection() { return mCurrentSection; }
		const FixedArray<WorldSection *, 9> &
			GetActiveSections() { return mActiveSections; }
		void
			GetActiveSectionNodes(Array<Node *> & nodes);
		void
			GetSectionNodes(Array<Node *> & nodes);

		void 
			ImpCullLights(Array<Light *> & lightArray, Camera * camera);

		RenderContext * 
			NewRenderContext(int id, int order, const String & name);
		void 
			DeleteRenderContext(RenderContext * context);
		void
			BeginRenderContext(RenderContext * context);
		void
			EndRenderContext();
		RenderContext * 
			GetCurrentRenderContext();
		
		void 
			Update(float elapsedTime);
		void 
			Render();

		Node *
			RayCheck(float * dist, const Ray & ray, float length, int flags);
		bool 
			RayCheckEx(float * dist, const Ray & ray, float length, int flags);

	protected:
		void
			_updateSections(float elapsedTime);

	protected:
		int mFrameId;

		PS_Manager * mPSManager;
		MeshManager * mMeshManager;
		PrefabManager * mPrefabManager;
		ResourceLoaderMT * mResourceLoaderMT;
		ILoadRule * mLoadRule;

		Array<RenderContext *> mRenderContexts;
		RenderContext * mMainRenderContext;
		RenderContext * mCurrentRenderContext;

		Node * mNodeLinker;
		Zone * mMainZone;
		Camera * mMainCamera;
		Light * mMainLight;
		Terrain * mTerrain;
		GrassManager * mGrassManager;
		NavData * mNavData;

		Info mInfo;
		EvInfo mEvInfo;
		String mFilename;
		ConfigFile mSetting;

		WorldSection * mSections;
		WorldSection * mCurrentSection;
		FixedArray<WorldSection *, 9> mActiveSections;
	};

	struct __ContextEntry
	{
		__ContextEntry(RenderContext * cn) { World::Instance()->BeginRenderContext(cn); }
		~__ContextEntry() { World::Instance()->EndRenderContext(); }
	};
}

#define ENTER_RENDER_CONTEXT(_cn) Rad3::_ContextEntry __context_entry(_cn)

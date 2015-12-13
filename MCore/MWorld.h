/*
*	Word
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRoot.h"
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
#include "MWorldComponent.h"
#include "MWorldSection.h"
#include "MConfigFile.h"

#include "MFogController.h"
#include "MGrassController.h"
#include "MMainLightController.h"

namespace Rad {

#define MAIN_CONETEX_ORDER 2048

	class M_ENTRY World : public Singleton<World>
	{
		friend class Node;

	public:
		tEvent0 E_Init;
		tEvent0 E_Shutdown;
		tEvent1<float> E_Update;

		tEvent0 E_RenderBegin;
		tEvent0 E_RenderEnd;

		tEvent0 E_VisibleCullBegin;
		tEvent0 E_VisibleCullEnd;
		tEvent0 E_RenderContextBegin;
		tEvent0 E_RenderContextEnd;

		tEvent0 E_RenderPrepare;

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

		const Info *
			GetInfo() { return &mInfo; }
		ConfigFile *
			GetSetting() { return &mSetting; }
		const String &
			GetFilename() { return mFilename; }

		void
			AddComponent(WorldComponent * cp);
		void
			RemoveComponent(WorldComponent * cp, bool _delete = true);
		void
			RemoveComponent(int i, bool _delete = true);
		void
			RemoveAllComponent();
		void
			ResortComponent(WorldComponent * cp);
		WorldComponent *
			GetComponent(int i) { return mComponents[i]; }
		int
			GetComponentCount() { return mComponents.Size(); }

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
			NewRenderContext(int order, const String & name, int id = -1);
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
		String mFilename;
		ConfigFile mSetting;

		Array<WorldComponent *> mComponents;

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

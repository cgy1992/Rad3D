#include "MWorld.h"
#include "MFunctional.h"
#include "MRoot.h"
#include "MMeshManager.h"
#include "MResourceManager.h"

namespace Rad {

	DF_PROPERTY_BEGIN(World::Info)
		DF_PROPERTY(World::Info, SectionCount, "", "", PT_Int2)
		DF_PROPERTY(World::Info, SectionSize, "", "", PT_Float3)
		DF_PROPERTY(World::Info, SectionZoneTile, "", "", PT_Int3)
		DF_PROPERTY(World::Info, SectionZoneDepth, "", "", PT_Int)
	DF_PROPERTY_END()

	ImplementSingleton(World);

	World::World()
	{
		mFrameId = 0;
		mNodeLinker = NULL;
		mResourceLoaderMT = NULL;
		mLoadRule = new LoadRuleStandard;

		mCurrentRenderContext = NULL;

		mPSManager = new PS_Manager;
		mMeshManager = new MeshManager;
		mPrefabManager = new PrefabManager;

		mMainZone = new Zone(NULL);
		mMainCamera = new Camera;
		mMainLight = new Light(eLightType::DIRECTION);
		mTerrain = new Terrain;
		mGrassManager = new GrassManager;
		mNavData = new NavData;

		mMainLight->SetRotationEx(Float3(45.0f, -45.0f, 0));
		mMainLight->SetAmbient(Float3(0.5f, 0.5f, 0.5f));
		mMainLight->SetDiffuse(Float3(1.0f, 1.0f, 1.0f));
		mMainLight->SetSpecular(Float3(0.0f, 0.0f, 0.0f));

		mMainRenderContext = NewRenderContext(MAIN_CONETEX_ORDER, "Main");
		mMainRenderContext->SetCamera(mMainCamera);
		mMainRenderContext->SetVisibleCuller(new VisibleCullerStandard);
		mMainRenderContext->SetShaderProvider(new ShaderProviderStandard);
		mMainRenderContext->SetRenderPipeline(new RenderPipelineStandard);

		mInfo.SectionSize = Float3(128, 64, 128);
		mInfo.SectionZoneTile = Int3(4, 1, 4);
		mInfo.SectionZoneDepth = 1;
		mSections = NULL;
		mCurrentSection = NULL;

		//
		Root::REG_OBJECT_T<Mesh>();
		Root::REG_OBJECT_T<MeshShaderNull>();
		Root::REG_OBJECT_T<MeshShaderStandard>();

		Root::REG_OBJECT_T<ParticleSystem>();
		Root::REG_OBJECT_T<Billboard>();
		Root::REG_OBJECT_T<Camera>();
		Root::REG_OBJECT_T<Light>();
		Root::REG_OBJECT_T<Node>();
		Root::REG_OBJECT_T<Sound>();

		Root::REG_OBJECT_T<Water>();
		Root::REG_OBJECT_T<WaterShaderStandard>();

		Root::REG_OBJECT_T<Prefab>();

		Root::REG_OBJECT_T<NodeTracker>();
		Root::REG_OBJECT_T<SimpleCollision>();

		Root::REG_OBJECT_T<TerrainSection>();
		Root::REG_OBJECT_T<GrassSection>();

		Root::REG_OBJECT_T<FogController>();
		Root::REG_OBJECT_T<GrassController>();
		Root::REG_OBJECT_T<MainLightController>();

		//
		E_Init();
	}

	World::~World()
	{
		E_Shutdown();

		RemoveAllComponent();

		DisableMultiThreadLoad();

		SetLoadRule(NULL);

		delete mNavData;
		delete mGrassManager;
		delete mTerrain;
		delete mMainLight;
		delete mMainCamera;
		delete mMainZone;

		delete mPSManager;
		delete mMeshManager;
		delete mPrefabManager;

		for (int i = 0; i < mRenderContexts.Size(); ++i)
		{
			delete mRenderContexts[i];
		}

		d_assert (mNodeLinker == NULL);
	}

	void World::Unload()
	{
		E_Unload();

		mCurrentSection = NULL;
		mActiveSections.Clear();

		RemoveAllComponent();

		safe_delete_array (mSections);

		mTerrain->Unload();

		mNavData->Clear();

		mSetting.Clear();

		mFilename = "";

		mInfo.SectionCount.x = mInfo.SectionCount.y = 0;
	}

#define WORLD_MAGIC MAKE_DWORD('W', 'M', '0', '1')
#define WORLD_COMPONENT_CHUNK 0x0001

	void World::Load(const String & filename)
	{
		Unload();

		mFilename = filename;

		String basename = FileHelper::RemoveExternName(filename);

		// load component
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(basename);
		if (stream != NULL)
		{
			ISerializerD IS(stream);

			IS.Check(WORLD_MAGIC);

			int ckId = 0;
			while (IS.Read(&ckId, 4) && ckId != 0)
			{
				if (ckId == WORLD_COMPONENT_CHUNK)
				{
					int typeId;
					int length;

					IS >> typeId;
					IS >> length;

					WorldComponent * p = Root::NEW_OBJECT_T<WorldComponent>(typeId);
					if (p != NULL)
					{
						p->Serialize(IS);

						AddComponent(p);
					}
					else
					{
						d_log("?: WorldComponent '%d' new failed.", typeId);
					}
				}
			}
		}

		// load section
		stream = ResourceManager::Instance()->OpenResource(basename + ".setting");
		if (stream != NULL)
		{
			mSetting.LoadStream(stream);

			rml_node * node = mSetting.first_node("World");
			if (node != NULL)
			{
				mInfo.Serialize(node, false);

				d_assert (mInfo.SectionCount.x > 0 && mInfo.SectionCount.y > 0);

				mSections = new WorldSection[mInfo.SectionCount.x * mInfo.SectionCount.y];

				node = node->first_node("Section");
				while (node != NULL)
				{
					const char * str = node->value();
					int x = -1, y = -1;

					str = str_getint(x, str);
					str = str_getint(y, str);

					WorldSection * section = GetSection(x, y);
					d_assert (section != NULL);

					String name;
					name.Format("%s_%04d_%04d", basename.c_str(), x, y);
					mSections[y * mInfo.SectionCount.x + x].Init(name, Int2(x, y), node);

					node = node->next_sibling("Section");
				}
			}
			else
			{
				d_log("X: World setting '%s' error, no world info.", filename.c_str());
			}

			E_LoadSetting();
		}
		else
		{
			d_log ("X: Load world setting '%s' failed.", filename.c_str());
		}

		//
		mTerrain->Load(basename + ".terrain");
		mNavData->Load(basename + ".path");

		//
		if (!IsMultiThreadLoad())
		{
			for (int i = 0; i < mInfo.SectionCount.x * mInfo.SectionCount.y; ++i)
			{
				mSections[i].Load();
			}

			mTerrain->EnsureBuild();
		}

		Update(0);

		E_Load();
	}

	void World::Save(const String & filename)
	{
		if (mSections == NULL && IsMultiThreadLoad())
			return ;

		String basename = FileHelper::RemoveExternName(filename);
		{
			FILE * fp = fopen(basename.c_str(), "wb");
			if (fp)
			{
				OSerializerF OS(fp, true);
				OSerializerTester OT;

				OS << (int)WORLD_MAGIC;
				
				for (int i = 0; i < mComponents.Size(); ++i)
				{
					WorldComponent * cp = mComponents[i];
					if (cp->Serializable())
					{
						OT.Clear();
						cp->Serialize(OT);

						int typeId = cp->GetRTTI()->TypeId();
						int length = OT.Size();

						OS << (int)WORLD_COMPONENT_CHUNK;
						OS << typeId;
						OS << length;

						cp->Serialize(OS);
					}
				}
				
				OS << 0;
			}
		}
		
		for (int i = 0; i < mInfo.SectionCount.x * mInfo.SectionCount.y; ++i)
		{
			const Int2 & index = mSections[i].GetIndex();

			String name;
			name.Format("%s_%04d_%04d", basename.c_str(), index.x, index.y);

			FILE * fp = fopen(name.c_str(), "wb");
			if (fp)
			{
				OSerializerF OS(fp, true);
				mSections[i].Serialize(OS);
			}
			else
			{
				d_log ("X: Save section failed, '%s'.", name.c_str());
			}
		}

		mTerrain->Save(basename + ".terrain");
		mNavData->Save(basename + ".path");

		E_Save();
	}

	void World::AddComponent(WorldComponent * p)
	{
		d_assert (!p->_inworld);

		int i;
		for (i = mComponents.Size(); i > 0 ; ++i)
		{
			if (mComponents[i - 1]->GetOrder() <= p->GetOrder())
				break;
		}

		mComponents.Insert(i, p);

		p->_inworld = true;
	}

	void World::RemoveComponent(WorldComponent * p, bool _delete)
	{
		int i;
		for (i = 0; i < mComponents.Size(); ++i)
		{
			if (mComponents[i] == p)
				break;
		}

		d_assert (i < mComponents.Size());

		RemoveComponent(i, _delete);
	}

	void World::RemoveComponent(int i, bool _delete)
	{
		mComponents[i]->_inworld = false;

		if (_delete)
			delete mComponents[i];

		mComponents.Erase(i);
	}

	void World::RemoveAllComponent()
	{
		for (int i = 0; i < mComponents.Size(); ++i)
		{
			delete mComponents[i];
		}

		mComponents.Clear();
	}

	WorldSection * World::GetSection(const Float3 & position)
	{
		int x = (int)(position.x / mInfo.SectionSize.x);
		int y = (int)(position.z / mInfo.SectionSize.z);

		if (x >= 0 && x < mInfo.SectionCount.x &&
			y >= 0 && y < mInfo.SectionCount.y)
		{
			return &mSections[y * mInfo.SectionCount.x + x];
		}

		return NULL;
	}

	WorldSection * World::GetSection(int x, int y)
	{
		d_assert (
			x >= 0 && x < mInfo.SectionCount.x &&
			y >= 0 && y < mInfo.SectionCount.y);

		return &mSections[y * mInfo.SectionCount.x + x];
	}

	WorldSection * World::GetBestSection(const Float3 & position)
	{
		WorldSection * pSection = GetSection(position);
		if (pSection != NULL)
			return pSection;

		Float2 p2 = Float2(position.x, position.z);

		int choose = -1;
		float length = MAX_FLOAT;

		for (int i = 0; i < mInfo.SectionCount.x * mInfo.SectionCount.y; ++i)
		{
			Float2 o;
			o.x = mSections[i].GetIndex().x * mInfo.SectionSize.x;
			o.y = mSections[i].GetIndex().y * mInfo.SectionSize.z;

			float d = o.DistanceSq(p2);
			if (d < length)
			{
				length = d;
				choose = i;
			}
		}

		return choose != -1 ? &mSections[choose] : NULL;
	}

	void World::GetActiveSectionNodes(Array<Node *> & nodes)
	{
		for (int i = 0; i < mActiveSections.Size(); ++i)
		{
			const Array<Node *> & arr = mActiveSections[i]->GetNodes();
			for (int j = 0; j < arr.Size(); ++j)
			{
				nodes.PushBack(arr[j]);
			}
		}
	}

	void World::GetSectionNodes(Array<Node *> & nodes)
	{
		for (int i = 0; i < mInfo.SectionCount.x * mInfo.SectionCount.y; ++i)
		{
			const Array<Node *> & arr = mSections[i].GetNodes();
			for (int j = 0; j < arr.Size(); ++j)
			{
				nodes.PushBack(arr[j]);
			}
		}
	}

	void World::EnableMultiThreadLoad()
	{
		if (mResourceLoaderMT == NULL)
		{
			mResourceLoaderMT = new ResourceLoaderMT;
			ResourceManager::Instance()->SetResourceLoader(mResourceLoaderMT);
		}
	}

	void World::DisableMultiThreadLoad()
	{
		if (mResourceLoaderMT != NULL)
		{
			ResourceManager::Instance()->SetResourceLoader(NULL);
			mResourceLoaderMT = NULL;
		}
	}

	bool World::IsMultiThreadLoad()
	{
		return ResourceManager::Instance()->GetResourceLoader()->IsMultiThread();
	}

	void World::SetLoadRule(ILoadRule * rule)
	{
		safe_delete (mLoadRule);

		mLoadRule = rule;
	}

	void World::ImpCullLights(Array<Light *> & lightArray, Camera * cam)
	{
		for (Node * node = mNodeLinker; node != NULL; node = LINKER_NEXT(node))
		{
			Light * light = DYNAMIC_CAST(Light, node);

			if (light != NULL && light->IsLighting() && light != mMainLight)
			{
				switch (light->GetType())
				{
				case eLightType::DIRECTION:
					lightArray.PushBack(light);
					break;

				case eLightType::POINT:
				case eLightType::SPOT:
					{
						Sphere sph = Sphere(light->GetWorldPosition(), light->GetAttenEnd());

						if (cam->InVisible(sph))
							lightArray.PushBack(light);
					}
					break;
				}
			}
		}
	}

	RenderContext * World::NewRenderContext(int order, const String & name, int id)
	{
		int i = 0;
		for (i = 0; i < mRenderContexts.Size(); ++i)
		{
			if (order < mRenderContexts[i]->GetOrder())
				break;
		}

		RenderContext * context = new RenderContext(order, name, id);

		mRenderContexts.Insert(i, context);

		return context;
	}

	void World::DeleteRenderContext(RenderContext * context)
	{
		for (int i = 0; i < mRenderContexts.Size(); ++i)
		{
			if (mRenderContexts[i] == context)
			{
				delete mRenderContexts[i];
				mRenderContexts.Erase(i);

				return ;
			}
		}

		d_assert (0);
	}

	void World::BeginRenderContext(RenderContext * context)
	{
		mCurrentRenderContext = context;
	}

	void World::EndRenderContext()
	{
		mCurrentRenderContext = NULL;
	}

	RenderContext * World::GetCurrentRenderContext()
	{
		return mCurrentRenderContext;
	}

	void World::Update(float elapsedTime)
	{
		profile_code();

		E_Update(elapsedTime);

		int i = 0;
		float time[5] = {0, 0, 0, 0, 0};

		//
		if (mResourceLoaderMT)
		{
			mResourceLoaderMT->Update();
		}

		// 
		for (int i = 0; i < mComponents.Size(); ++i)
		{
			WorldComponent * p = mComponents[i];
			if (p->IsEnable())
			{
				if (p->Update(elapsedTime) < 0)
					RemoveComponent(i--);
			}
		}

		// 
		if (mSections != NULL)
		{
			const Float3 & center = mMainCamera->GetWorldPosition();
			WorldSection * pNewSection = GetBestSection(center);
			WorldSection * pOldSection = mCurrentSection;

			if (pNewSection != pOldSection)
			{
				FixedSet<WorldSection *, 9> omap;
				for (int i = 0; i < mActiveSections.Size(); ++i)
				{
					omap.Insert(mActiveSections[i]);
				}

				FixedSet<WorldSection *, 9> nmap;
				if (pNewSection != NULL)
				{
					const Int2 & index = pNewSection->GetIndex();

					if (index.x > 0)
					{
						if (index.y > 0)
							nmap.Insert(GetSection(index.x - 1, index.y - 1));

						nmap.Insert(GetSection(index.x - 1, index.y));

						if (index.y < mInfo.SectionCount.y - 1)
							nmap.Insert(GetSection(index.x - 1, index.y + 1));
					}

					if (index.y > 0)
						nmap.Insert(GetSection(index.x, index.y - 1));

					nmap.Insert(GetSection(index.x, index.y));

					if (index.y < mInfo.SectionCount.y - 1)
						nmap.Insert(GetSection(index.x, index.y + 1));

					if (index.x < mInfo.SectionCount.x - 1)
					{
						if (index.y > 0)
							nmap.Insert(GetSection(index.x + 1, index.y - 1));

						nmap.Insert(GetSection(index.x + 1, index.y));

						if (index.y < mInfo.SectionCount.y - 1)
							nmap.Insert(GetSection(index.x + 1, index.y + 1));
					}
				}

				mCurrentSection = pNewSection;

				if (IsMultiThreadLoad())
				{
					for (int i = 0; i < omap.Size(); ++i)
					{
						if (nmap.Find(omap[i]) == -1)
						{
							omap[i]->Unload();
						}
					}

					for (int i = 0; i < nmap.Size(); ++i)
					{
						if (omap.Find(nmap[i]) == -1)
						{
							nmap[i]->Load();
						}
					}
				}

				mActiveSections.Clear();
				for (int i = 0; i < nmap.Size(); ++i)
				{
					const Int2 & i1 = nmap[i]->GetIndex();

					int j = 0;
					for (j = 0; j < mActiveSections.Size(); ++j)
					{
						const Int2 & i2 = mActiveSections[j]->GetIndex();
						if (MAKE_DWORD2(i1.x, i1.y) < MAKE_DWORD2(i2.x, i2.y))
							break;
					}

					mActiveSections.Insert(j, nmap[i]);
				}

				E_SectionCurrentChanged(mCurrentSection);
			}
		}

		//
		for (Node * node = mNodeLinker; node != NULL; node = LINKER_NEXT(node))
		{
			if (node->IsVisible())
				node->Update(elapsedTime);
		}
	}

	void World::Render()
	{
		profile_code();

		E_RenderBegin();

		for (int i = 0; i < mRenderContexts.Size(); ++i)
		{
			if (mRenderContexts[i]->IsEnable())
			{
				BeginRenderContext(mRenderContexts[i]);

				E_RenderContextBegin();

				mCurrentRenderContext->DoRender(mFrameId);

				E_RenderContextEnd();

				EndRenderContext();
			}
		}

		RenderSystem::Instance()->SetRenderTarget(0, NULL);
		RenderSystem::Instance()->SetRenderTarget(1, NULL);
		RenderSystem::Instance()->SetRenderTarget(2, NULL);
		RenderSystem::Instance()->SetRenderTarget(3, NULL);
		RenderSystem::Instance()->SetDepthBuffer(NULL);

		E_RenderEnd();

		mFrameId = Max(0, mFrameId + 1);
	}

	Node * World::RayCheck(float * dist, const Ray & ray, float length, int flags)
	{
		Array<RayCheckContract> contractArray;
		World::Instance()->MainZone()->RayCheck(contractArray, ray, length, flags);

		if (contractArray.Size() == 0)
			return NULL;

		RayCheckContract::SortOp sortOp;
		Sort(&contractArray[0], contractArray.Size(), sortOp);

		Node * pSelectedNode = NULL;
		float k_len = MAX_FLOAT;

		for (int i = 0; i < contractArray.Size(); ++i)
		{
			Node * pNode = contractArray[i].node;

			if (pNode == NULL)
				continue;

			if (k_len < contractArray[i].contract_dist)
				continue;

			float c_len = MAX_FLOAT;
			if (pNode->GetShape() != NULL)
			{
				pNode->GetShape()->OnRayCheck(c_len, ray, length);
			}
			else
			{
				c_len = contractArray[i].contract_dist;
			}

			if (c_len < k_len)
			{
				pSelectedNode = pNode;
				k_len = c_len;
			}
		}

		if (dist)
		{
			*dist = k_len;
		}

		return pSelectedNode;
	}

	bool World::RayCheckEx(float * dist, const Ray & ray, float length, int flags)
	{
		if (RayCheck(dist, ray, length, flags) != NULL)
		{
			return true;
		}
		else
		{
			float d = 0;
			Float3 point;
			if (mTerrain->RayCheck(&point, ray))
			{
				d = point.Distance(ray.orig);

				if (dist)
					*dist = d;

				return d <= length;
			}
		}

		return false;
	}
}
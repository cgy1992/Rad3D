#include "MRenderPipeline.h"
#include "MFunctional.h"
#include "MWorld.h"
#include "MRoot.h"
#include "MRenderSystem.h"
#include "MShaderFXManager.h"

namespace Rad {

	ImplementRootRTTI(RenderPipeline);

	RenderPipeline::RenderPipeline()
	{
	}

	RenderPipeline::~RenderPipeline()
	{
	}

	//
	ImplementRTTI(RenderPipelineStandard, RenderPipeline);

	RenderPipelineStandard::RenderPipelineStandard()
	{
	}

	RenderPipelineStandard::~RenderPipelineStandard()
	{
	}

	void RenderPipelineStandard::DoRender()
	{
		mRenderQueue.Clear();

		RenderContext * pRenderContext = World::Instance()->GetCurrentRenderContext();
		Camera * pCamera = pRenderContext->GetCamera();
		VisibleCullerPtr pVisibleCuller = pRenderContext->GetVisibleCuller();
		ShaderProviderPtr pShaderProvider = pRenderContext->GetShaderProvider();

		if (pVisibleCuller == NULL || pShaderProvider == NULL || pCamera == NULL)
			return ;

		Array<Node*> & visibleArray = pRenderContext->GetVisibleCuller()->GetNodeArray();
		for (int i = 0; i < visibleArray.Size(); ++i)
		{
			Node * n = visibleArray[i];
			if (n->GetRenderContextId() == pRenderContext->GetId())
			{
				visibleArray[i]->AddRenderQueue(&mRenderQueue);
			}
		}

		RenderSystem::Instance()->SetViewTM(pCamera->GetViewTM());
		RenderSystem::Instance()->SetProjTM(pCamera->GetProjTM());

		RenderSystem::Instance()->SetClipPlane(pCamera->GetNearClip(), pCamera->GetFarClip());
		RenderSystem::Instance()->SetTime(Root::Instance()->GetTime());

		RenderSystem::Instance()->SetLight(World::Instance()->MainLight());

		World::Instance()->E_RenderPrepare();

		World::Instance()->E_RenderSolidBegin();

		do 
		{
			Array<RenderObject *> & arr = mRenderQueue.GetSolidObjects();

			for (int i = 0; i < arr.Size(); ++i)
			{
				pShaderProvider->ApplyShaderFX(arr[i]);
			}

			if (arr.Size() > 0)
			{
				SolidSorter sorter;
				Sort(&arr[0], arr.Size(), sorter);
			}

			for (int i = 0; i < arr.Size(); ++i)
			{
				RenderSystem::Instance()->Render(arr[i]->GetCurrentShaderFX(), arr[i]);
			}

		} while (0);

		World::Instance()->E_RenderSolid();

		DoLighting();

		World::Instance()->E_RenderSolidEnd();

		World::Instance()->E_RenderAlphaBegin();

		do 
		{
			Array<RenderObject *> & arr = mRenderQueue.GetAlphaObjects();

			if (arr.Size() > 0)
			{
				AlphaSorter sorter(pCamera);
				Sort(&arr[0], arr.Size(), sorter);
			}

			for (int i = 0; i < arr.Size(); ++i)
			{
				pShaderProvider->ApplyShaderFX(arr[i]);

				eBlendMode bm = arr[i]->GetMaterial()->blendMode;
				if (bm == eBlendMode::OPACITY || bm == eBlendMode::ALPHA_TEST)
				{
					RenderSystem::Instance()->SetColorWriteEnable(false);
					RenderSystem::Instance()->Render(arr[i]->GetCurrentShaderFX(), arr[i]);
					RenderSystem::Instance()->SetColorWriteEnable(true);
				}

				RenderSystem::Instance()->Render(arr[i]->GetCurrentShaderFX(), arr[i]);
			}

		} while(0);

		World::Instance()->E_RenderAlpha();

		World::Instance()->E_RenderAlphaEnd();
	}

	void RenderPipelineStandard::DoLighting()
	{
		RenderContext * pRenderContext = World::Instance()->GetCurrentRenderContext();
		ShaderProviderPtr pShaderProvider = pRenderContext->GetShaderProvider();

		Array<Light *> & lightArray = pRenderContext->GetVisibleCuller()->GetLightArray();
		Array<RenderObject *> & objArray = mRenderQueue.GetSolidObjects();

		for (int i = 0; i < lightArray.Size(); ++i)
		{
			Light * light = lightArray[i];
			Float3 lightExtend = Float3(light->GetAttenEnd(), light->GetAttenEnd(), light->GetAttenEnd());

			Aabb lightBound;
			lightBound.minimum = light->GetWorldPosition() - lightExtend;;
			lightBound.maximum = light->GetWorldPosition() + lightExtend;;

			RenderSystem::Instance()->SetLight(light);

			switch (light->GetType())
			{
			case eLightType::POINT:
			case eLightType::SPOT:
				for (int j = 0; j < objArray.Size(); ++j)
				{
					RenderObject * obj = objArray[j];

					if (obj->_getNode() && obj->_getNode()->IsLighting())
					{
						Aabb bound;
						obj->_getWorldBound(bound);
						if (lightBound.Intersect(bound))
						{
							pShaderProvider->ApplyLightingFX(light, obj);
							RenderSystem::Instance()->Render(obj->GetCurrentShaderFX(), obj);
						}
					}
				}
				break;

			case eLightType::DIRECTION:
				for (int j = 0; j < objArray.Size(); ++j)
				{
					RenderObject * obj = objArray[j];
					if (obj->_getNode() && obj->_getNode()->IsLighting())
					{
						pShaderProvider->ApplyLightingFX(light, obj);
						RenderSystem::Instance()->Render(obj->GetCurrentShaderFX(), obj);
					}
				}
				break;
			}
		}
	}

	//
	ImplementRTTI(RenderPipelineNodeArray, RenderPipeline);

	RenderPipelineNodeArray::RenderPipelineNodeArray(bool childable)
		: mChildable(childable)
	{
	}

	RenderPipelineNodeArray::~RenderPipelineNodeArray()
	{
	}

	void _addRenderQueue(Node * node, RenderQueue * rq)
	{
		if (node->IsVisible())
		{
			node->AddRenderQueue(rq);
		}

		for (int i = 0; i < node->GetChildCount(); ++i)
		{
			_addRenderQueue(node->GetChild(i), rq);
		}
	}

	void RenderPipelineNodeArray::DoRender()
	{
		mRenderQueue.Clear();

		RenderContext * pRenderContext = World::Instance()->GetCurrentRenderContext();
		Camera * pCamera = pRenderContext->GetCamera();
		ShaderProviderPtr pShaderProvider = pRenderContext->GetShaderProvider();

		if (mChildable)
		{
			for (int i = 0; i < NodeArray.Size(); ++i)
			{
				_addRenderQueue(NodeArray[i], &mRenderQueue);
			}
		}
		else
		{
			for (int i = 0; i < NodeArray.Size(); ++i)
			{
				if (NodeArray[i]->IsVisible())
				{
					NodeArray[i]->AddRenderQueue(&mRenderQueue);
				}
			}
		}
		
		RenderSystem::Instance()->SetViewTM(pCamera->GetViewTM());
		RenderSystem::Instance()->SetProjTM(pCamera->GetProjTM());

		RenderSystem::Instance()->SetClipPlane(pCamera->GetNearClip(), pCamera->GetFarClip());
		RenderSystem::Instance()->SetTime(Root::Instance()->GetTime());

		RenderSystem::Instance()->SetLight(World::Instance()->MainLight());

		do 
		{
			Array<RenderObject *> & arr = mRenderQueue.GetSolidObjects();

			for (int i = 0; i < arr.Size(); ++i)
			{
				pShaderProvider->ApplyShaderFX(arr[i]);
			}

			if (arr.Size() > 0)
			{
				SolidSorter sorter;
				Sort(&arr[0], arr.Size(), sorter);
			}

			for (int i = 0; i < arr.Size(); ++i)
			{
				RenderSystem::Instance()->Render(arr[i]->GetCurrentShaderFX(), arr[i]);
			}

		} while (0);

		do 
		{
			Array<RenderObject *> & arr = mRenderQueue.GetAlphaObjects();

			if (arr.Size() > 0)
			{
				AlphaSorter sorter(pCamera);
				Sort(&arr[0], arr.Size(), sorter);
			}

			for (int i = 0; i < arr.Size(); ++i)
			{
				pShaderProvider->ApplyShaderFX(arr[i]);

				eBlendMode bm = arr[i]->GetMaterial()->blendMode;
				if (bm == eBlendMode::OPACITY || bm == eBlendMode::ALPHA_TEST)
				{
					RenderSystem::Instance()->SetColorWriteEnable(false);
					RenderSystem::Instance()->Render(arr[i]->GetCurrentShaderFX(), arr[i]);
					RenderSystem::Instance()->SetColorWriteEnable(true);
				}

				RenderSystem::Instance()->Render(arr[i]->GetCurrentShaderFX(), arr[i]);
			}

		} while(0);
	}

}



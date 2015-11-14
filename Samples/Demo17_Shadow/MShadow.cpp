#include "MShadow.h"
#include "MWorld.h"
#include "MRoot.h"
#include "MFunctional.h"
#include "MRenderSystem.h"

namespace Rad {

	Shadow::Shadow(RenderContext * context, int mapSize)
		: mContext(context)
		, mShadowCamera(NULL)
		, mShadowContext(NULL)
	{
		mMapSize = mapSize;
		mDistance = 15 * UNIT_METRES;
		mFadeRatio = 0.8f;
		mOffset = 30 * UNIT_METRES;
		mColor = Float4(0.5f, 0.5f, 0.5f);

		mShadowCamera = new Camera;
		mShadowContext = World::Instance()->NewRenderContext(0, 0, "ShadowContext");

		mShadowContext->SetCamera(mShadowCamera);
		mShadowContext->SetVisibleCuller(new ShadowVisibleCuller);
		mShadowContext->SetShaderProvider(new ShadowShaderProvider);
		mShadowContext->SetRenderPipeline(new ShadowRenderPipline(this));
		mShadowContext->SetColorClear(eClearMode::ALL, Float4(1, 1, 1, 1));

		RenderTargetPtr pRenderTarget = HWBufferManager::Instance()->NewRenderTarget(mMapSize, mMapSize, ePixelFormat::R5G6B5);
		DepthBufferPtr pDepthBuffer = HWBufferManager::Instance()->NewDepthBuffer(mMapSize, mMapSize, ePixelFormat::D16);

		mShadowContext->SetRenderTarget(0, pRenderTarget);
		mShadowContext->SetDepthBuffer(pDepthBuffer);

		Viewport vp;
		vp.x = 1;
		vp.y =1;
		vp.w = mMapSize - 1;
		vp.h = mMapSize - 1;
		mShadowContext->SetViewport(vp);

		World::Instance()->E_RenderingBegin += L_OnUpdateCamera(this, &Shadow::_updateCamera);
		World::Instance()->E_RenderSolidEnd += L_OnRenderShadow(this, &Shadow::_renderShadow);
	}

	Shadow::~Shadow()
	{
		World::Instance()->DeleteRenderContext(mShadowContext);
		safe_delete (mShadowCamera);

		World::Instance()->E_RenderSolidEnd -= L_OnRenderShadow(this, &Shadow::_renderShadow);
		World::Instance()->E_RenderingBegin -= L_OnUpdateCamera(this, &Shadow::_updateCamera);
	}

	void Shadow::SetDistance(float distance)
	{
		mDistance = distance;
	}

	void Shadow::SetFadeRatio(float fadeRatio)
	{
		mFadeRatio = fadeRatio;
	}

	void Shadow::SetOffset(float offset)
	{
		mOffset = offset;
	}

	void Shadow::SetColor(const Float4 & color)
	{
		mColor = color;
	}

	void Shadow::_updateCamera()
	{
		Camera * worldCam = mContext->GetCamera();
		Light * light = World::Instance()->MainLight();

		float nearClip = worldCam->GetNearClip();
		float farClip = mDistance;

		Float3 xAixs = worldCam->GetWorldRotation().GetRightVector();
		Float3 yAixs = worldCam->GetWorldRotation().GetUpVector();
		Float3 zAixs = light->GetWorldRotation().GetDirVector();

		if (Math::Abs(zAixs.Dot(yAixs)) > 0.99f)
		{
			yAixs = Float3::Cross(zAixs, xAixs);
			xAixs = Float3::Cross(yAixs, zAixs);
		}
		else
		{
			xAixs = Float3::Cross(yAixs, zAixs);
			yAixs = Float3::Cross(zAixs, xAixs);
		}

		if (xAixs.Dot(worldCam->GetWorldRotation().GetDirVector()) < 0)
			xAixs = -xAixs;

		yAixs = Float3::Cross(zAixs, xAixs);

		xAixs.Normalize();
		yAixs.Normalize();
		zAixs.Normalize();

		Mat4 matView, matProj;
		Quat qOrient;
		
		qOrient.FromAxis(xAixs, yAixs, zAixs);

		matView.MakeViewLH(worldCam->GetPosition(), qOrient);

		Float3 corner[8], t_corner[8];

		worldCam->GetWorldCorner(t_corner, nearClip, farClip);

		float dist = t_corner[4].Distance(t_corner[5]);

		for (int i = 0; i < 8; ++i)
		{
			corner[i] = t_corner[i] * matView;
		}

		Aabb aabb = Aabb::Invalid;

		for (int i = 0; i < 8; ++i)
		{
			aabb.minimum = Float3::Minimum(aabb.minimum, corner[i]);
			aabb.maximum = Float3::Maximum(aabb.maximum, corner[i]);
		}

		Float3 center = aabb.GetCenter();
		float width = aabb.GetSize().x;
		float height = aabb.GetSize().y;
		float depth = aabb.GetSize().z;

		Mat4 matInvView = matView;
		matInvView.Inverse();
		center *= matInvView;

		mShadowCamera->SetPosition(center - zAixs * mOffset);
		mShadowCamera->SetRotation(qOrient);
		mShadowCamera->SetClipPlane(nearClip, mOffset + depth);
		mShadowCamera->SetOrthoParam(width, height, true);
	}

	void Shadow::_renderShadow()
	{
		if (mContext != World::Instance()->GetCurrentRenderContext())
			return ;

		RenderQueue * rq = mContext->GetRenderPipeline()->GetRenderQueue();

		do 
		{
			Mat4 matShadow = mShadowCamera->GetViewProjMatrix();
			TexturePtr depthMap = mShadowContext->GetRenderTarget(0)->GetTexture();
			
			Float4 fadeParam;
			fadeParam.x = mFadeRatio * mDistance;
			fadeParam.y = 1 / (mDistance - fadeParam.x);

			Float4 invMapSize;
			invMapSize.x = 1.0f / mMapSize;
			invMapSize.y = 1.0f / mMapSize;

			FixedArray<RenderObject *, 1024> arr;

			for (int i = 0; i < rq->GetSolidObjects().Size(); ++i)
			{
				RenderObject * able = rq->GetSolidObjects().At(i);
				if (able->_getNode() != NULL && able->_getNode()->IsReceiveShadow())
				{
					mShadowContext->GetShaderProvider()->ApplyShaderFX(able, 1);

					arr.PushBack(able);
				}

				if (arr.Size() == arr.Capacity())
					break;
			}

			if (arr.Size() > 0)
			{
				RenderPipeline::SolidSorter sorter;
				Sort(&arr[0], arr.Size(), sorter);

				RenderSystem::Instance()->SetTexture(0, depthMap.c_ptr());
			}

			for (int i = 0; i < arr.Size(); ++i)
			{
				ShaderFX * fx = arr[i]->GetCurrentShaderFX();
				if (fx != NULL && fx->GetPassCount() > 0)
				{
					Mat4 matShadow = mShadowCamera->GetViewProjMatrix();
					if (!arr[i]->IsSkined())
					{
						Mat4 worldTm;
						arr[i]->_getWorldTM(worldTm);

						matShadow = worldTm * matShadow;
					}

					fx->GetPass(0)->SetConst("u_ShadowTM", matShadow);
					fx->GetPass(0)->SetConst("u_InvTexSize", invMapSize);
					fx->GetPass(0)->SetConst("u_ShadowColor", mColor);
					fx->GetPass(0)->SetConst("u_FadeParam", fadeParam);

					RenderSystem::Instance()->Render(fx, arr[i]);
				}
			}

		} while (0);

		E_RenderShadow(this);
	}

	//
	ShadowVisibleCuller::ShadowVisibleCuller()
	{
	}

	void ShadowVisibleCuller::DoCull(int frameId, Camera * camera)
	{
		if (mFrameId == frameId)
			return ;

		mFrameId = frameId;

		mNodeArray.Clear();
		mLightArray.Clear();

		World::Instance()->MainZone()->ImpVisiblityCull(mNodeArray, camera);
	}

	//
	ShadowShaderProvider::ShadowShaderProvider()
	{
		mCasterFX = ShaderFXManager::Instance()->Load("ShadowCaster", "Shadow/MShadowCaster.mfx");
		mCasterFXSkined = ShaderFXManager::Instance()->Load("ShadowCasterSkined", "Shadow/MShadowCaster.mfx", "#define D_SKINED\n");

		mReceiverFX = ShaderFXManager::Instance()->Load("ShadowReceiver", "Shadow/MShadowReceiver.mfx");
		mReceiverFXSkined = ShaderFXManager::Instance()->Load("ShadowReceiverSkined", "Shadow/MShadowReceiver.mfx", "#define D_SKINED\n");
	}

	ShadowShaderProvider::~ShadowShaderProvider()
	{
	}

	void ShadowShaderProvider::ApplyShaderFX(RenderObject * able, int flag)
	{
		able->SetCurrentShaderFX(NULL);

		if (!flag)
		{
			if (!able->IsSkined())
			{
				able->SetCurrentShaderFX(mCasterFX);
			}
			else
			{
				able->SetCurrentShaderFX(mCasterFXSkined);
			}
		}
		else
		{
			if (!able->IsSkined())
			{
				able->SetCurrentShaderFX(mReceiverFX);
			}
			else
			{
				able->SetCurrentShaderFX(mReceiverFXSkined);
			}
		}
	}

	//
	ShadowRenderPipline::ShadowRenderPipline(Shadow * shadow)
		: mShadow(shadow)
	{
	}

	void ShadowRenderPipline::DoRender()
	{
		mRenderQueue.Clear();

		RenderContext * pRenderContext = mShadow->GetShadowContext();
		Camera * pCamera = pRenderContext->GetCamera();
		ShaderProviderPtr pShaderProvider = pRenderContext->GetShaderProvider();

		Array<Node*> & visibleArray = pRenderContext->GetVisibleCuller()->GetNodeArray();

		for (int i = 0; i < visibleArray.Size(); ++i)
		{
			Node * n = visibleArray[i];

			if (n->IsCastShadow() && !n->IsReceiveShadow())
			{
				visibleArray[i]->AddRenderQueue(&mRenderQueue);
			}
		}

		RenderSystem::Instance()->SetViewTM(pCamera->GetViewMatrix());
		RenderSystem::Instance()->SetProjTM(pCamera->GetProjMatrix());

		RenderSystem::Instance()->SetClipPlane(pCamera->GetNearClip(), pCamera->GetFarClip());
		RenderSystem::Instance()->SetTime(Root::Instance()->GetTime());

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

			RenderSystem::Instance()->SetLight(World::Instance()->MainLight());

			for (int i = 0; i < arr.Size(); ++i)
			{
				RenderSystem::Instance()->Render(arr[i]->GetCurrentShaderFX(), arr[i]);
			}

		} while (0);

		mShadow->E_RenderDepth(mShadow);
	}

}


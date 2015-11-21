#include "MMesh.h"

#include "MLog.h"
#include "MRenderQueue.h"
#include "MMeshManager.h"
#include "MShaderFXManager.h"
#include "MResourceManager.h"
#include "MHWBufferManager.h"
#include "MAnimationSerializer.h"

namespace Rad {

	SubMesh::SubMesh(Mesh * parent, MeshBuffer * sm)
		: mMeshBuffer(sm)
		, mMeshShader(NULL)
		, mVisible(true)
	{
		mNode = parent;

		if (mMeshBuffer != NULL)
		{
			mMaterial = *(mMeshBuffer->GetMaterial());
			mRenderOp = *(mMeshBuffer->GetRenderOp());

			SetMeshShader(mMeshBuffer->GetShader());
		}
	}

	SubMesh::~SubMesh()
	{
	}

	void SubMesh::SetMeshShader(MeshShaderPtr shader)
	{
		mMeshShader = shader;

		SetShaderProvider(shader.c_ptr());
		SetRenderCallBack(eRenderCallBack::SHADER, mMeshShader.c_ptr());
	}

	VertexBufferPtr SubMesh::QueryVertexAnimationBuffer()
	{
		if (mVertexAnimtionBuffer != NULL)
		{
			int stride = mRenderOp.vertexBuffers[0]->GetStride();
			int count = mRenderOp.vertexBuffers[0]->GetCount();

			mVertexAnimtionBuffer = HWBufferManager::Instance()->NewVertexBuffer(stride, count, eUsage::DYNAMIC_MANAGED);

			void * src = mRenderOp.vertexBuffers[0]->Lock(eLockFlag::READ);
			void * dest = mVertexAnimtionBuffer->Lock(eLockFlag::WRITE);

			memcpy(dest, src, stride * count);

			mVertexAnimtionBuffer->Unlock();
			mRenderOp.vertexBuffers[0]->Unlock();

			mRenderOp.vertexBuffers[0] = mVertexAnimtionBuffer;
		}

		return mVertexAnimtionBuffer;
	}

	int SubMesh::_getBoneTM(Mat4 * boneTM)
	{
		if (mMeshBuffer != NULL)
		{
			const Array<short> & boneIdMap = mMeshBuffer->GetBoneIdMap();

			if (boneTM == NULL)
			{
				return boneIdMap.Size();
			}

			SkeletonPtr skel = GetParent()->GetSkeleton();

			for (int i = 0; i < boneIdMap.Size(); ++i)
			{
				boneTM[i] = skel->GetBone(boneIdMap[i])->GetBoneTM();
			}

			return boneIdMap.Size();
		}

		return 0;
	}


	//
	DF_PROPERTY_BEGIN(Mesh)
		DF_PROPERTY_EX(Mesh, mFilename, "", "File", "PT_Filename mesh(*.mesh)|*.mesh|", PT_String)
		DF_PROPERTY(Mesh, mGrouped, "", "Grouped", PT_Bool)
		DF_PROPERTY_ENUM(Mesh, mSLMode, "StaticLighting", "Mode", eStaticLightingMode)
		DF_PROPERTY(Mesh, mSLCastShadow, "StaticLighting", "CastShadow", PT_Bool)
		DF_PROPERTY(Mesh, mSLReceiveShadow, "StaticLighting", "ReceiveShadow", PT_Bool)
		DF_PROPERTY(Mesh, mSLMapWidth, "StaticLighting", "MapWidth", PT_Int)
		DF_PROPERTY(Mesh, mSLMapHeight, "StaticLighting", "MapHeight", PT_Int)
		DF_PROPERTY_EX(Mesh, mSLMapFile, "StaticLighting", "MapFile", "Static", PT_String)
	DF_PROPERTY_END();

	void Mesh::OnPropertyChanged(const Property * p)
	{
		if (strcmp(p->name, "mFilename") == 0)
		{
			SetFilename(mFilename);
		}
		else
		{
			Node::OnPropertyChanged(p);
		}
	}

	IObject * Mesh::Clone()
	{
		Mesh * mesh = new Mesh;

		mesh->Copy(this);

		return mesh;
	}

	ImplementRTTI(Mesh, Node);

	Mesh::Mesh()
		: Node("Mesh")
		, mSource(NULL)
		, mSkeleton(NULL)
		, mSkeletonShared(false)
		, mAnimtionPaused(false)
	{
		mSLMode = eStaticLightingMode::NONE;
		mSLCastShadow = true;
		mSLReceiveShadow = true;
		mSLMapWidth = 256;
		mSLMapHeight = 256;

		mGrouped = false;
	}

	Mesh::~Mesh(void)
	{
		Unload();

		StopAnimation();
	}

	void Mesh::SetFilename(const String & filename)
	{
		mFilename = filename;

		MeshSourcePtr mesh = NULL;
		if (mFilename != "")
		{
			mesh = MeshManager::Instance()->LoadMesh(filename);
		}
		
		SetSource(mesh);
	}

	void Mesh::SetGrouped(bool grouped)
	{
		mGrouped = grouped;
	}

	void Mesh::SetSLMode(int mode)
	{
		mSLMode = mode;
	}

	void Mesh::SetSLCastShadow(bool cast)
	{
		mSLCastShadow = cast;
	}

	void Mesh::SetSLReceiveShadow(bool receive)
	{
		mSLReceiveShadow = receive;
	}

	void Mesh::SetSLMapSize(int w, int h)
	{
		mSLMapWidth = w;
		mSLMapHeight = h;
	}

	void Mesh::SetSLMapFile(const String & filename)
	{
		mSLMapFile = filename;

		if (filename != "")
		{
			SetLightingMap(HWBufferManager::Instance()->LoadTexture(filename));
		}
	}

	void Mesh::OnMeshLoaded()
	{
		d_assert (mSource != NULL);
		d_assert (mSkeleton == NULL);
		d_assert (mMeshes.Size() == 0);

		for (int i = 0; i < mSource->GetMeshBufferCount(); ++i)
		{
			MeshBuffer * pMeshBuffer = mSource->GetMeshBuffer(i);
			d_assert (pMeshBuffer->GetBoneIdMap().Size() <= MAX_HW_BONE);

			mMeshes.PushBack(new SubMesh(this, pMeshBuffer));
		};

		if (mSource->GetJointCount() > 0)
		{
			mSkeleton = new Skeleton(mSource);
		}

		if (mSkeleton != NULL && !mSkeletonShared)
		{
			for (int i = 0; i < mSkeleton->GetRootBoneCount(); ++i)
			{
				Bone * bone = mSkeleton->GetRootBone(i);
				Attach(bone);
			}
		}

		SetLocalAabb(mSource->GetAabb());

		if (mShape)
		{
			mShape->OnLoaded();
		}

		E_Loaded(this);
	}

	void Mesh::OnMeshUnload()
	{
		E_Unload(this);

		if (mShape != NULL)
		{
			mShape->OnUnload();
		}

		mSkeleton = NULL;
		mSkeletonShared = false;

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			delete mMeshes[i];
		}
		mMeshes.Clear();

		for (int i = 0; i < mAnimationControllers.Size(); ++i)
		{
			mAnimationControllers[i]->Reset();
		}
	}

	void Mesh::SetSource(MeshSourcePtr mesh)
	{
		if (mSource != mesh)
		{
			Unload();

			mSource = mesh;

			if (mSource != NULL)
			{
				mSource->E_Loaded += L_MeshLoaded(this, &Mesh::OnMeshLoaded);
				mSource->E_Unload += L_MeshUnload(this, &Mesh::OnMeshUnload);

				if (mSource->GetLoadState() == Resource::LOADED)
				{
					OnMeshLoaded();
				}
			}
		}
	}

	void Mesh::SetShaderFX(ShaderFX * fx)
	{
		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			mMeshes[i]->SetShaderFX(fx);
		}
	}

	void Mesh::Update(float elapsedTime)
	{
		Node::Update(elapsedTime);

		if (!mAnimtionPaused)
		{
			for (int i = 0; i < mAnimationControllers.Size(); ++i)
			{
				AnimationController * anim = mAnimationControllers[i];
				if (!anim->Update(this, elapsedTime))
				{
					delete mAnimationControllers[i];
					mAnimationControllers.Erase(i);
					continue;
				}
			}
		}

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			MeshShaderPtr shader = mMeshes[i]->GetMeshShader();
			if (shader != NULL)
			{
				shader->Update(elapsedTime);
			}
		}
	}

	void Mesh::AddRenderQueue(RenderQueue * rq)
	{
		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			if (mMeshes[i]->IsVisible())
			{
				rq->AddRenderObject(mMeshes[i]);
			}
		}
	}

	int	Mesh::GetVertexCount()
	{
		int count = 0;

		for (int i = 0; i < GetSubMeshCount(); ++i)
		{
			SubMesh * pMesh = GetSubMesh(i);
			
			count += pMesh->GetRenderOp()->vertexBuffers[0]->GetCount();
		}

		return count;
	}

	int	Mesh::GetPrimCount()
	{
		int count = 0;

		for (int i = 0; i < GetSubMeshCount(); ++i)
		{
			SubMesh * pMesh = GetSubMesh(i);

			count += pMesh->GetRenderOp()->primCount;
		}

		return count;
	}

	int Mesh::GetSubMeshCount()
	{
		return mMeshes.Size();
	}

	SubMesh * Mesh::GetSubMesh(int index)
	{
		return mMeshes[index];
	}

	SubMesh * Mesh::NewSubMesh()
	{
		SubMesh * pMesh = new SubMesh(this, NULL);

		mMeshes.PushBack(pMesh);

		return pMesh;
	}

	void Mesh::DeleteSubMesh(int index)
	{
		SubMesh * pSubMesh = GetSubMesh(index);

		mMeshes.Erase(index);

		delete pSubMesh;
	}

	void Mesh::SetSkeleton(SkeletonPtr skel, bool shared)
	{
		mSkeleton = skel;
		mSkeletonShared = shared;
	}

	SkeletonPtr Mesh::GetSkeleton()
	{
		return mSkeleton;
	}

	bool Mesh::IsSkeletonShared()
	{
		return mSkeletonShared;
	}

	void Mesh::LoadAnimation(const FixedString32 & name, const String & source)
	{
		d_assert (mSource != NULL);

		if (mSkeleton == NULL || mSource->GetAnimation(name) != NULL)
			return ;

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);
		if (stream != NULL)
		{
			AnimationSerializer::Load(mSource->NewAnimation(name), stream);
		}
	}

	AnimationController * Mesh::PlayAnimation(const FixedString32 & name, const BlendInfo & bi, int order)
	{
		if (!bi.Mixed)
		{
			for (int i = 0; i < mAnimationControllers.Size(); ++i)
			{
				mAnimationControllers[i]->BlendOut();
			}
		}

		AnimationController * controller = new AnimationController(name, bi, order);

		AddAnimationController(controller);

		return controller;
	}

	void Mesh::AddAnimationController(AnimationController * controller)
	{
		int i = 0;
		for (i = 0; i < mAnimationControllers.Size(); ++i)
		{
			if (mAnimationControllers[i]->GetOrder() > controller->GetOrder())
				break;
		}

		mAnimationControllers.Insert(i, controller);
	}

	void Mesh::RemoveAnimationController(AnimationController * controller)
	{
		int index = -1;
		for (int i = 0; i < mAnimationControllers.Size(); ++i)
		{
			if (mAnimationControllers[i] == controller)
			{
				index = -1;
				break;
			}
		}

		d_assert (index != -1);

		delete mAnimationControllers[index];
		mAnimationControllers.Erase(index);
	}

	AnimationController * Mesh::GetAnimationController(const FixedString32 & name)
	{
		for (int i = 0; i < mAnimationControllers.Size(); ++i)
		{
			if (mAnimationControllers[i]->GetName() == name &&
				!mAnimationControllers[i]->IsBlendOut())
				return mAnimationControllers[i];
		}

		return NULL;
	}

	AnimationController * Mesh::GetAnimationController(int i)
	{
		return mAnimationControllers[i];
	}

	int Mesh::GetAnimationControllerCount()
	{
		return mAnimationControllers.Size();
	}

	bool Mesh::IsAnimationPlaying(const FixedString32 & name)
	{
		for (int i = 0; i < mAnimationControllers.Size(); ++i)
		{
			if (mAnimationControllers[i]->GetName() == name)
				return true;
		}

		return false;
	}

	void Mesh::StopAnimation()
	{
		for (int i = 0; i < mAnimationControllers.Size(); ++i)
			delete mAnimationControllers[i];

		mAnimationControllers.Clear();
	}

	bool Mesh::IsAnimationStoped()
	{
		return mAnimationControllers.Size() == 0;
	}

	void Mesh::PauseAnimation(bool pause)
	{
		mAnimtionPaused = pause;
	}

	bool Mesh::IsAnimationPaused()
	{
		return mAnimtionPaused;
	}

	void Mesh::SetLightingColor(const Rgba32 * colorBuffer, int count)
	{
		if (colorBuffer == NULL)
		{
			for (int i = 0; i < GetSubMeshCount(); ++i)
			{
				RenderOp * rop = mMeshes[i]->GetRenderOp();

				rop->vertexBuffers[LIGHTING_COLOR_STREAM] = NULL;
				rop->vertexDeclarations[LIGHTING_COLOR_STREAM].Clear();
			}
		}
		else
		{
			int vertexCount = GetVertexCount();
			d_assert (vertexCount == count);

			int startIndex = 0;
			for (int i = 0; i < mMeshes.Size(); ++i)
			{
				RenderOp * rop = mMeshes[i]->GetRenderOp();

				d_assert (
					rop->vertexBuffers[0] != NULL &&
					rop->vertexBuffers[LIGHTING_COLOR_STREAM] == NULL);

				int count = rop->vertexBuffers[0]->GetCount();
				rop->vertexDeclarations[LIGHTING_COLOR_STREAM].AddElement(eVertexSemantic::LIGHTING_COLOR, eVertexType::UBYTE4);
				rop->vertexBuffers[LIGHTING_COLOR_STREAM] = HWBufferManager::Instance()->NewVertexBuffer(4, count);
				Rgba32 * data = (Rgba32 *)rop->vertexBuffers[LIGHTING_COLOR_STREAM]->Lock(eLockFlag::WRITE);
				{
					memcpy(data, colorBuffer + startIndex, 4 * count);
				}
				rop->vertexBuffers[LIGHTING_COLOR_STREAM]->Unlock();

				startIndex += count;
			}
		}
	}

	void Mesh::GetLightingColor(Array<Rgba32> & colorBuffer)
	{
		int vertexCount = GetVertexCount();
		
		if (vertexCount == 0 || mMeshes[0]->GetRenderOp()->vertexBuffers[LIGHTING_COLOR_STREAM] == NULL)
		{
			colorBuffer.Resize(0);
			return ;
		}

		colorBuffer.Resize(vertexCount);

		int startIndex = 0;
		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			RenderOp * rop = mMeshes[i]->GetRenderOp();

			d_assert (
				rop->vertexBuffers[LIGHTING_COLOR_STREAM] != NULL &&
				rop->vertexDeclarations[LIGHTING_COLOR_STREAM].GetVertexSize() == 4);

			int count = rop->vertexBuffers[0]->GetCount();
			Rgba32 * data = (Rgba32 *)rop->vertexBuffers[LIGHTING_COLOR_STREAM]->Lock(eLockFlag::READ);
			{
				memcpy(&colorBuffer[startIndex], data, 4 * count);
			}
			rop->vertexBuffers[LIGHTING_COLOR_STREAM]->Unlock();

			startIndex += count;
		}
	}

	bool Mesh::HasLightingColor()
	{
		return mMeshes.Size() > 0 && mMeshes[0]->GetRenderOp()->vertexBuffers[LIGHTING_COLOR_STREAM] != NULL;
	}

	void Mesh::SetLightingMap(TexturePtr texture)
	{
		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			mMeshes[i]->GetMaterial()->maps[eMapType::LIGHTING_MAP] = texture;
		}
	}

	TexturePtr Mesh::GetLightingMap()
	{
		if (mMeshes.Size())
		{
			return mMeshes[0]->GetMaterial()->maps[eMapType::LIGHTING_MAP];
		}

		return NULL;
	}

	void Mesh::ResetLighting()
	{
		SetLightingColor(NULL, 0);

		SetLightingMap(NULL);
	}

	bool Mesh::HasLightingMapUV()
	{
		bool result = true;

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			RenderOp * rop = mMeshes[i]->GetRenderOp();

			result &= rop->vertexDeclarations[0].HasElement(eVertexSemantic::TEXCOORD1, eVertexType::FLOAT2);
		}

		return result;
	}

	void Mesh::Load(float priority)
	{
		if (mSource != NULL && priority < 0)
		{
			mSource->EnsureLoad();
		}

		if (mSource == NULL && mFilename != "")
		{
			SetSource(MeshManager::Instance()->LoadMesh(mFilename, priority));
		}
	}

	void Mesh::Unload()
	{
		OnMeshUnload();

		if (mSource != NULL)
		{
			mSource->E_Loaded -= L_MeshLoaded(this, &Mesh::OnMeshLoaded);
			mSource->E_Unload -= L_MeshUnload(this, &Mesh::OnMeshUnload);
			mSource = NULL;
		}
	}

}

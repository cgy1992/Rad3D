#include "MMeshSource.h"
#include "MMeshManager.h"
#include "MMeshSerializer.h"
#include "MRoot.h"

namespace Rad {

	MeshSource::MeshSource(const String & name, const String & sourceName)
		: Resource(name, sourceName)
		, mAabb(Float3::Zero, Float3::Zero)
	{
	}

	MeshSource::~MeshSource()
	{
		OnUnload();
	}

	void MeshSource::Release()
	{
		MeshManager::Instance()->_delMesh(this);
	}

	void MeshSource::OnLoad()
	{
		Resource::OnLoad();
	}

	void MeshSource::OnUnload()
	{
		Resource::OnUnload();

		mAabb = Aabb::Zero;

		DeleteAllMeshBuffer();

		DeleteAllJoint();

		DeleteAllAnimation();
	}

	void MeshSource::_loadImp(DataStreamPtr stream)
	{
		if (mLoadState != LOADING)
			return ;

		if (stream != NULL)
		{
			MeshSerializer::Load(this, stream);
		}
		else
		{
			d_log("!: Load mesh failed '%s'.", mSourceName.c_str());
		}
	}

	MeshBuffer * MeshSource::GetMeshBuffer(int index)
	{ 
		return mMeshes[index];
	}

	int MeshSource::GetMeshBufferCount()
	{ 
		return mMeshes.Size();
	}

	MeshBuffer * MeshSource::NewMeshBuffer(int i)
	{
		MeshBuffer * sm = new MeshBuffer(this);

		if (i == -1)
			mMeshes.PushBack(sm);
		else
			mMeshes.Insert(i, sm);

		return sm;
	}

	void MeshSource::DeleteMeshBuffer(int index)
	{   
		delete mMeshes[index];
		mMeshes.Erase(index);
	}

	void MeshSource::DeleteAllMeshBuffer()
	{
		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			delete mMeshes[i];
		}

		mMeshes.Clear();
	}

	ColMesh * MeshSource::GetColMesh()
	{
		return &mColMesh;
	}

	void MeshSource::FillColMesh(Float3 * verts, int numVerts, int * idxs, int numIdxs)
	{
		mColMesh.Alloc(numVerts, numIdxs);

		Float3 * colVert = mColMesh.GetVertices();
		int * colIdx = mColMesh.GetIndices();

		for (int i = 0; i < numVerts; ++i)
			colVert[i] = verts[i];

		for (int i = 0; i < numIdxs; ++i)
			colIdx[i] = idxs[i];
	}

	bool MeshSource::GenColMeshFromRenderMesh()
	{
		if (!mColMesh.IsValid() && mMeshes.Size())
		{
			mColMesh.Clear();

			int iVertexCount = 0;
			int iIndexCount = 0;

			for (int i = 0; i < mMeshes.Size(); ++i)
			{
				RenderOp * pRenderOp = mMeshes[i]->GetRenderOp();

				if (!pRenderOp->vertexDeclarations[0].HasElement(eVertexSemantic::POSITION, eVertexType::FLOAT3) &&
					pRenderOp->vertexBuffers[0] == NULL &&
					pRenderOp->indexBuffer == NULL &&
					pRenderOp->primType != ePrimType::TRIANGLE_LIST)
					return false;

				iVertexCount += pRenderOp->vertexBuffers[0]->GetCount();
				iIndexCount += pRenderOp->indexBuffer->GetCount();
			}

			mColMesh.Alloc(iVertexCount, iIndexCount);

			Float3 * colVert = mColMesh.GetVertices();
			int * colIdx = mColMesh.GetIndices();
			int iStartVertex = 0;

			for (int i = 0; i < mMeshes.Size(); ++i)
			{
				RenderOp * pRenderOp = mMeshes[i]->GetRenderOp();
				int offset = pRenderOp->vertexDeclarations[0].GetElementOffset(eVertexSemantic::POSITION);
				int stride = pRenderOp->vertexBuffers[0]->GetStride();

				unsigned char * vdata = (unsigned char *)pRenderOp->vertexBuffers[0]->Lock(eLockFlag::READ);
				for (int n = 0; n < pRenderOp->vertexBuffers[0]->GetCount(); ++n)
				{
					*colVert++ = *(Float3 *)(vdata + offset);
					vdata += stride;
				}
				
				pRenderOp->vertexBuffers[0]->Unlock();

				unsigned short * idata = (unsigned short *)pRenderOp->indexBuffer->Lock(eLockFlag::READ);
				for (int n = 0; n < pRenderOp->indexBuffer->GetCount(); ++n)
				{
					*colIdx++ = (int)(*idata++) + iStartVertex;
				}
				pRenderOp->indexBuffer->Unlock();

				iStartVertex += pRenderOp->vertexBuffers[0]->GetCount();
			}

			return true;
		}

		return false;
	}

	void MeshSource::SetAabb(const Aabb & bound)
	{
		mAabb = bound;
	}

	bool MeshSource::GenAabbFromRenderMesh()
	{
		if (mMeshes.Size() == 0)
			return false;

		mAabb = Aabb::Invalid;
		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			RenderOp * pRenderOp = mMeshes[i]->GetRenderOp();
			int offset = pRenderOp->vertexDeclarations[0].GetElementOffset(eVertexSemantic::POSITION);
			int stride = pRenderOp->vertexBuffers[0]->GetStride();
			int count = pRenderOp->vertexBuffers[0]->GetCount();

			unsigned char * vdata = (unsigned char *)pRenderOp->vertexBuffers[0]->Lock(eLockFlag::READ);
			for (int n = 0; n < count; ++n)
			{
				Float3 p = *(Float3 *)(vdata + offset);

				mAabb.minimum = Float3::Minimum(p, mAabb.minimum);
				mAabb.maximum = Float3::Maximum(p, mAabb.maximum);

				vdata += stride;
			}
			pRenderOp->vertexBuffers[0]->Unlock();
		}

		return true;
	}

	joint * MeshSource::NewJoint(const char * name)
	{
		joint bn;

		bn.name = name;
		bn.flag = 0;
		bn.position = Float3::Zero;
		bn.rotation = Quat::Identity;
		bn.scale = Float3(1, 1, 1);

		mJoints.PushBack(bn);

		return &mJoints[mJoints.Size() - 1];
	}

	int MeshSource::GetJointCount()
	{
		return mJoints.Size();
	}

	joint * MeshSource::GetJoint(const char * boneName)
	{
		for (int i = 0; i < mJoints.Size(); ++i)
		{
			if (mJoints[i].name == boneName)
				return &mJoints[i];
		}

		return NULL;
	}

	joint * MeshSource::GetJoint(short index)
	{
		d_assert(index < mJoints.Size());

		return &mJoints[index];
	}

	void MeshSource::SetJointLink(short parent, short child)
	{
		joint_link bp;

		bp.parent = parent;
		bp.child = child;

		mJointLinks.PushBack(bp);
	}

	int MeshSource::GetJointLinkCount()
	{
		return mJointLinks.Size();
	}

	joint_link * MeshSource::GetJointLink(int index)
	{
		d_assert(index < mJointLinks.Size());

		return &mJointLinks[index];
	}

	void MeshSource::DeleteAllJoint()
	{
		mJoints.Clear();
		mJointLinks.Clear();
	}

	Animation * MeshSource::NewAnimation(const FixedString32 & sAnimation)
	{
		d_assert(GetAnimation(sAnimation) == NULL);

		Animation * anim = new Animation(sAnimation);

		mAnimations.PushBack(anim);

		return anim;
	}

	Animation *	MeshSource::GetAnimation(const FixedString32 & sAnimation)
	{
		for (int i = 0; i < mAnimations.Size(); ++i)
		{
			if (mAnimations[i]->GetName() == sAnimation)
				return mAnimations[i];
		}

		return NULL;
	}

	int	MeshSource::GetAnimationCount()
	{
		return mAnimations.Size();
	}

	Animation *	MeshSource::GetAnimation(int index)
	{
		return mAnimations[index];
	}

	void MeshSource::DeleteAnimation(const FixedString32 & anim)
	{
		for (int i = 0; i < mAnimations.Size(); ++i)
		{
			if (mAnimations[i]->GetName() == anim)
			{
				delete mAnimations[i];
				mAnimations.Erase(i);
				return ;
			}
		}

		d_assert (0);
	}

	void MeshSource::DeleteAllAnimation()
	{
		for (int i = 0; i < mAnimations.Size(); ++i)
			delete mAnimations[i];

		mAnimations.Clear();
	}

	//
	MeshBuffer::MeshBuffer(MeshSource * parent)
		: mParent(parent)
		, mShader(NULL)
	{
	}

	MeshBuffer::~MeshBuffer()
	{
	}

	bool MeshBuffer::SetShaderClass(const char * className)
	{
		mShader = NULL;

		if (*className != 0 && strcmp(className, "MeshShaderNull") != 0)
		{
			mShader = Root::NEW_OBJECT_T<MeshShader>(className);
			if (mShader == NULL)
			{
				d_log("!: MeshShader '%s' create failed.", className);
			}

			return mShader != NULL;
		}
		
		return true;
	}

}

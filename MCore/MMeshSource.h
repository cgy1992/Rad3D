/*
*	Mesh Source
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAabb.h"
#include "MColMesh.h"
#include "MAnimation.h"
#include "MResource.h"
#include "MRenderObject.h"
#include "MMeshShader.h"

namespace Rad {

	struct joint
	{
		FixedString32 name;
		int flag;
		Float3 position;
		Quat rotation;
		Float3 scale;
	};

	struct joint_link
	{
		short parent;
		short child;
	};

	class MeshBuffer;

	class M_ENTRY MeshSource : public Resource
	{
	public:
		MeshSource(const String & name, const String & sourceName);
		~MeshSource();

		virtual void				
			Release();

		virtual void				
			OnLoad();
		virtual void				
			OnUnload();
		virtual void				
			_loadImp(DataStreamPtr stream);

		MeshBuffer *                
			NewMeshBuffer(int i = -1);
		MeshBuffer *                
			GetMeshBuffer(int index);
		int                         
			GetMeshBufferCount();
		void                        
			DeleteMeshBuffer(int index);
		void						
			DeleteAllMeshBuffer();

		ColMesh *					
			GetColMesh();
		void						
			FillColMesh(Float3 * verts, int numVerts, int * idxs, int numIdxs);
		bool						
			GenColMeshFromRenderMesh();

		void                        
			SetAabb(const Aabb & bound);
		const Aabb &                
			GetAabb() const { return mAabb; }
		bool						
			GenAabbFromRenderMesh();

		joint * 
			NewJoint(const char * name);
		int 
			GetJointCount();
		joint * 
			GetJoint(const char * name);
		joint * 
			GetJoint(short handle);

		void 
			SetJointLink(short parent, short child);
		int 
			GetJointLinkCount();
		joint_link * 
			GetJointLink(int index);

		void 
			DeleteAllJoint();

		Animation *					
			NewAnimation(const FixedString32 & sAnimation);
		Animation *					
			GetAnimation(const FixedString32 & sAnimation);
		int							
			GetAnimationCount();
		Animation *					
			GetAnimation(int index);
		void						
			DeleteAnimation(const FixedString32 & anim);
		void						
			DeleteAllAnimation();

	protected:
		Aabb mAabb;
		Array<MeshBuffer *> mMeshes;
		ColMesh	mColMesh;

		Array<joint> mJoints;
		Array<joint_link> mJointLinks;
		
		Array<Animation *> mAnimations;
	};

	typedef SmartPtr<MeshSource> MeshSourcePtr;

	//
	class M_ENTRY MeshBuffer
	{
		DECLARE_ALLOC();

	public:
		MeshBuffer(MeshSource * parent);
		~MeshBuffer();

		MeshSourcePtr
			GetParent() { return mParent; }

		void 
			SetName(const FixedString32 & name) { mName = name; }
		const FixedString32 & 
			GetName() { return mName; }

		Material * 
			GetMaterial() { return &mMaterial; }
		RenderOp * 
			GetRenderOp() { return &mRenderOp; }
		Array<short> & 
			GetBoneIdMap() { return mBoneIdMap; }
		MeshShaderPtr
			GetShader() { return mShader; }

		bool
			SetShaderClass(const char * className);

	protected:
		MeshSource * mParent;
		FixedString32 mName;
		Material mMaterial;
		RenderOp mRenderOp;
		Array<short> mBoneIdMap;
		MeshShaderPtr mShader;
	};
}
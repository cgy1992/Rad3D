/*
*	Skeleton
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MXml.h"
#include "MNode.h"
#include "MMeshSource.h"

namespace Rad {

	class M_ENTRY Bone : public Node
	{
		DECLARE_RTTI();

	public:
		Bone(const FixedString32 & name, short handle);
		virtual ~Bone();

		void                   
			_init(const Mat4 & parentTM);

		const Float3 &
			GetInitPosition() { return mInitPosition; }
		const Quat &
			GetInitRotation() { return mInitRotation; }
		const Float3 &
			GetInitScale() { return mInitScale; }
		const Mat4 &
			GetOffsetTM() { return mOffetTM; }
		const Mat4 &			
			GetBoneTM();

		virtual void
			_updateTM();
		virtual void
			Update(float elapsedTime) {}

		Bone *                  
			GetParentBone();
		short					
			GetBoneHandle() { return mBoneHandle; }
		void				    
			SetBoneFlag(int flag) { mBoneFlags = flag; }
		short
			GetBoneFlag() const { return mBoneFlags; }

		bool                    
			_IsRoot() const;

	protected:
		short mBoneHandle;
		short mBoneFlags;

		Float3 mInitPosition;
		Quat mInitRotation;
		Float3 mInitScale;

		Mat4 mOffetTM;
		Mat4 mBoneTM;
	};

	inline const Mat4 &	Bone::GetBoneTM()
	{
		if (mTmChangeFlags != 0)
			_updateTM();

		return mBoneTM;
	}

	//
	class M_ENTRY Skeleton : public IObject
	{
		DECLARE_REF();

	public:
		Skeleton();
		Skeleton(MeshSourcePtr mesh);
		virtual ~Skeleton();

		void
			Release() { delete this; }

		void 
			Clear();

		Bone * 
			CreateBone(const FixedString32 & name);

		void 
			InitBones();

		int 
			GetRootBoneCount();
		Bone * 
			GetRootBone(int i);

		int 
			GetBoneCount();
		Bone * 
			GetBone(const char * name);
		Bone * 
			GetBone(short handle);

		void 
			SaveXml(MeshSourcePtr mesh, xml_node root);

	protected:
		Array<Bone *> mBones;
		Array<Bone *> mRoots;
	};

	typedef SmartPtr<Skeleton> SkeletonPtr;
}
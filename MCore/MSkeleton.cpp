#include "MSkeleton.h"

namespace Rad {

	ImplementRTTI(Bone, Node);

	Bone::Bone(const FixedString32 & name, short handle)
		: Node(name, false)
		, mBoneHandle(handle)
		, mBoneFlags(0)
		, mBoneTM(Mat4::Identity)
		, mOffetTM(Mat4::Identity)
	{
	}

	Bone::~Bone()
	{
	}

	void Bone::_init(const Mat4 & parentTM)
	{
		mInitPosition = mPosition;
		mInitRotation = mRotation;
		mInitScale = mScale;

		mOffetTM.MakeTransform(mPosition, mRotation, mScale);
		mOffetTM *= parentTM;

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Node * child = mChildren[i];
			if (TYPE_OF(Bone, child))
			{
				static_cast<Bone *>(child)->_init(mOffetTM);
			}
		}

		mOffetTM.Inverse();
	}

	void Bone::_updateTM()
	{
		if (mTmChangeFlags == 0)
			return ;

		if (mTmChangeFlags & (eTmFlags::TRANSLATE | eTmFlags::ROTATE | eTmFlags::SCALE))
		{
			if (mParent)
			{
				if (mParent->GetTmChangeFlags() != 0)
					mParent->_updateTM();

				const Quat & parentRotation = mParent->GetWorldRotation();
				const Float3 & parentScale = mParent->GetWorldScale();
				const Float3 & parentPosition = mParent->GetWorldPosition();

				mWorldRotation = parentRotation * mRotation;
				mWorldScale = mScale * parentScale;
				mWorldPosition = (mPosition * parentScale) * parentRotation + parentPosition;
			}
			else
			{
				mWorldPosition = mPosition;
				mWorldRotation = mRotation;
				mWorldScale = mScale;
			}

			mWorldTM.MakeTransform(mWorldPosition, mWorldRotation, mWorldScale);

			mBoneTM = mOffetTM * mWorldTM;
		}

		mWorldAABB.minimum = mWorldPosition;
		mWorldAABB.maximum = mWorldPosition;

		if (mParent != NULL)
			mWorldOpacity = mOpacity * mParent->GetWorldOpacity();
		else
			mWorldOpacity = mOpacity;

		mTmChangeFlags = 0;
	}

	bool Bone::_IsRoot() const
	{
		return !TYPE_OF(Bone, mParent);
	}

	Bone * Bone::GetParentBone()
	{
		if (TYPE_OF(Bone, mParent))
		{
			return (Bone *)mParent;
		}

		return NULL;
	}


	//
	Skeleton::Skeleton(MeshSourcePtr mesh)
	{
		int numJoints = mesh->GetJointCount();
		for (int i = 0; i < numJoints; ++i)
		{
			joint * bn = mesh->GetJoint(i);

			Bone * bone = CreateBone(bn->name);
			bone->SetBoneFlag(bn->flag);
			bone->SetPosition(bn->position);
			bone->SetRotation(bn->rotation);
			bone->SetScale(bn->scale);
		}

		int numLinks = mesh->GetJointLinkCount();
		for (int i = 0; i < numLinks; ++i)
		{
			joint_link * bp = mesh->GetJointLink(i);

			if (bp->parent != -1)
			{
				Bone * parent = GetBone(bp->parent);
				Bone * child = GetBone(bp->child);
				parent->Attach(child);
			}
		}

		InitBones();
	}

	Skeleton::Skeleton()
	{
	}

	Skeleton::~Skeleton()
	{
		Clear();
	}

	void Skeleton::Clear()
	{
		for (int i = 0; i < mBones.Size(); ++i)
		{
			delete mBones[i];
		}

		mBones.Clear();
		mRoots.Clear();
	}

	void Skeleton::InitBones()
	{
		mRoots.Clear();

		for (int i = 0; i < mBones.Size(); ++i)
		{
			Bone * bone = mBones[i];

			if (bone->_IsRoot())
				mRoots.PushBack(bone);
		}

		for (int i = 0; i < mRoots.Size(); ++i)
		{
			mRoots[i]->_init(Mat4::Identity);
		}
	}

	int Skeleton::GetRootBoneCount()
	{
		return mRoots.Size();
	}

	Bone * Skeleton::GetRootBone(int i)
	{
		return mRoots[i];
	}

	Bone * Skeleton::CreateBone(const FixedString32 & name)
	{
		Bone * bone = new Bone(name, mBones.Size());
		
		mBones.PushBack(bone);

		return bone;
	}

	int Skeleton::GetBoneCount()
	{
		return mBones.Size();
	}

	Bone * Skeleton::GetBone(const char * name)
	{
		for (int i = 0; i < mBones.Size(); ++i)
		{
			if (mBones[i]->GetName() == name)
				return mBones[i];
		}
	
		return NULL;
	}

	Bone * Skeleton::GetBone(short handle)
	{
		assert(handle < mBones.Size());

		return mBones[handle];
	}

	void Skeleton::SaveXml(MeshSourcePtr mesh, xml_node root)
	{
		for (int i = 0; i < mBones.Size(); ++i)
		{
			Bone * pBone = mBones[i];

			xml_node b_node = root.append_node("Bone");
			b_node.append_attribute("name", pBone->GetName().c_str());

			 Float3 position;
			 Quat rotation;
			 Float3 scale;

			 if (mesh != NULL)
			 {
				 joint * pJoint = mesh->GetJoint(pBone->GetBoneHandle());

				 position = pJoint->position;
				 rotation = pJoint->rotation;
				 scale = pJoint->scale;
			 }
			 else
			 {
				 position = pBone->GetPosition();
				 rotation = pBone->GetRotation();
				 scale = pBone->GetScale();
			 }

			String str;

			xml_node node = b_node.append_node("Position");
			str += f2str(position.x, true) + " ";
			str += f2str(position.y, true) + " ";
			str += f2str(position.z, true);
			node.append_attribute("value", str.c_str());

			node = b_node.append_node("Rotation");
			str += f2str(rotation.x, true) + " ";
			str += f2str(rotation.y, true) + " ";
			str += f2str(rotation.z, true) + " ";
			str += f2str(rotation.w, true);
			node.append_attribute("value", str.c_str());

			node = b_node.append_node("Scale");
			str += f2str(scale.x, true) + " ";
			str += f2str(scale.y, true) + " ";
			str += f2str(scale.z, true);
			node.append_attribute("value", str.c_str());
		}

		for (int i = 0; i < mBones.Size(); ++i)
		{
			Bone * pBone = mBones[i];

			if (pBone->GetParentBone() == NULL)
				continue;

			xml_node node = root.append_node("Hierarchy");
			{
				short parent = pBone->GetParentBone()->GetBoneHandle();
				short child = pBone->GetBoneHandle();

				node.append_attribute("parent", i2str(parent).c_str());
				node.append_attribute("child", i2str(child).c_str());
			}
		}
	}

}
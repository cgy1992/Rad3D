#include "MAnimation.h"
#include "MMesh.h"

namespace Rad {

	Animation::Animation(const FixedString32 & name)
		: mName(name)
		, mLength(0)
	{
	}

	Animation::~Animation()
	{
		for (int i = 0; i < mSkelAnims.Size(); ++i)
		{
			delete mSkelAnims[i];
		}

		for (int i = 0; i < mVertexAnims.Size(); ++i)
		{
			delete mVertexAnims[i];
		}
	}

	void Animation::SetLength(float length)
	{
		mLength = length;
	}

	void Animation::AddSkeletonAnimation(SkeletonAnimation * anim)
	{
		mSkelAnims.PushBack(anim);
	}

	void Animation::RemoveSkeletonAnimation(int i)
	{
		delete mSkelAnims[i];
		mSkelAnims.Erase(i);
	}

	void Animation::AddVertexAnimation(VertexAnimation * anim)
	{
		mVertexAnims.PushBack(anim);
	}

	void Animation::RemoveVertexAnimation(int i)
	{
		delete mVertexAnims[i];
		mVertexAnims.Erase(i);
	}

	void Animation::AddEvent(float time, const FixedString32 & name)
	{
		Event e;
		e.time = time;
		e.name = name;

		mEvents.PushBack(e);
	}

	void Animation::RemoveEvent(int index)
	{
		mEvents.Erase(index);
	}

	Animation::Event * Animation::GetEvent(int index)
	{
		return &mEvents[index];
	}

	int Animation::GetEventCount()
	{
		return mEvents.Size();
	}

	void Animation::_calcuLength()
	{
		d_assert (mSkelAnims.Size() > 0);

		int count = mSkelAnims[0]->GetFrameCount();
	
		mLength = mSkelAnims[0]->GetKeyFrame(count - 1)->time;

		d_assert (mLength > 0);
	}

	//void Animation::_convertSkinAnim(Skeleton * skel)
	//{
	//	for (int i = 0; i < mSkelAnims.Size(); ++i)
	//	{
	//		SkeletonAnimation * anim = mSkelAnims[i];
	//
	//		short boneId = anim->GetBoneHandle();
	//		Bone * bone = skel->GetBone(boneId);
	//
	//		Quat bone_q = bone->GetInitOrientation().Inverse();
	//		Float3 bone_p = bone->GetInitPosition();
	//		Float3 bone_s = bone->GetInitScale();
	//
	//		for (int k = 0; k < anim->GetFrameCount(); ++k)
	//		{
	//			KeyFrame * kf = anim->GetKeyFrame(k);
	//
	//			Float3 p = (kf->GetTranslate() - bone_p) * bone_q;
	//			Quat q = bone_q * kf->GetRotation();
	//			Float3 s = kf->GetScale() / bone_s;
	//
	//			kf->SetTranslate(p);
	//			kf->SetRotation(q);
	//			kf->SetScale(s);
	//		}
	//	}
	//}
	
}
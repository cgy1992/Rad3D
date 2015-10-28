#include "MSkeletonAnimation.h"
#include "MAnimationController.h"
#include "MSkeleton.h"

namespace Rad {

	SkeletonAnimation::SkeletonAnimation(short handle)
		: mBoneId(handle)
	{
	}

	SkeletonAnimation::~SkeletonAnimation()
	{
	}

	void SkeletonAnimation::Update(Skeleton * skel, AnimationController * controller, int & last)
	{
		float time = controller->GetPosition();
		float weight = controller->GetWeight();
		bool interpolation = controller->GetBlendInfo().Interpolation;

		int k1 = last;
		for (int i = last + 1; i < mFrames.Size(); ++i)
		{
			if (mFrames[i].time > time)
				break;
			
			k1 = i;
		}

		if (k1 >= 0)
		{
			if (!interpolation && k1 == last)
				return ;

			Float3 position;
			Quat rotation;
			Float3 scale;

			if (interpolation && k1 < mFrames.Size() - 1)
			{
				int k2 = k1 + 1;
				float t = (time - mFrames[k1].time) / (mFrames[k2].time - mFrames[k1].time);

				position = Float3::Lerp(mFrames[k1].position, mFrames[k2].position, t);
				rotation = Quat::Slerp(mFrames[k1].rotation, mFrames[k2].rotation, t);
				scale = Float3::Lerp(mFrames[k1].scale, mFrames[k2].scale, t);
			}
			else
			{
				position = mFrames[k1].position;
				rotation = mFrames[k1].rotation;
				scale = mFrames[k1].scale;
			}

			Bone * bone = skel->GetBone(mBoneId);

			if (weight < 1)
			{
				position = Float3::Lerp(bone->GetPosition(), position, weight);
				rotation = Quat::Slerp(bone->GetRotation(), rotation, weight);
				scale = Float3::Lerp(bone->GetScale(), scale, weight);
			}

			bone->SetPosition(position);
			bone->SetRotation(rotation);
			bone->SetScale(scale);

			last = k1;
		}
	}

	void SkeletonAnimation::Optimize()
	{
		int i = 0;
		while (i < mFrames.Size() - 3)
		{
			const KeyFrame & k0 = mFrames[i + 0];
			const KeyFrame & k1 = mFrames[i + 1];
			const KeyFrame & k2 = mFrames[i + 2];

			if (k0.Equal(k1) && k0.Equal(k2))
				mFrames.Erase(i + 1);
			else
				++i;
		}
	}

	SkeletonAnimation::KeyFrame * SkeletonAnimation::CreateKeyFrame(float time)
	{
		mFrames.Resize(mFrames.Size() + 1);
		mFrames.Back().time = time;

		return &mFrames[mFrames.Size() - 1];
	}

	void SkeletonAnimation::RemoveKeyFrame(int index)
	{
		mFrames.Erase(index);
	}

	int SkeletonAnimation::GetFrameCount()
	{
		return mFrames.Size();
	}

	SkeletonAnimation::KeyFrame * SkeletonAnimation::GetKeyFrame(int index)
	{
		return &mFrames[index];
	}

	short SkeletonAnimation::GetBoneHandle()
	{
		return mBoneId;
	}

}
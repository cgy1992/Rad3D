#include "MAnimationController.h"
#include "MMesh.h"

namespace Rad {

	const BlendInfo BlendInfo::Default;

	AnimationController::AnimationController(const FixedString32 & name, const BlendInfo & bi, int order)
		: mName(name)
		, mOrder(order)
		, mWeightBlendInDelta(0)
		, mWeightBlendOutDelta(0)
		, mWeight(1)
	{
		mBlendInfo = bi;
		mPosition = mBlendInfo.Offset;

		mCatchedAnim = NULL;

		BlendIn();
	}

	AnimationController::~AnimationController()
	{
	}

	void AnimationController::BlendIn()
	{
		if (mBlendInfo.InTime < 0.01f)
			mWeightBlendInDelta = 0;
		else
			mWeightBlendInDelta = 1 / mBlendInfo.InTime;

		mWeightBlendOutDelta = 0;
		mWeight = 0;
	}

	void AnimationController::BlendOut()
	{
		if (mBlendInfo.OutTime < 0.01f)
			mWeightBlendOutDelta = 0;
		else
			mWeightBlendOutDelta = 1 / mBlendInfo.OutTime;

		mWeightBlendInDelta = 0;
	}

	void AnimationController::Reset()
	{
		mCatchedAnim = NULL;
	}

	bool AnimationController::Update(Mesh * mesh, float elapsedTime)
	{
		float lastPosition = mPosition;

		mPosition += elapsedTime * mBlendInfo.Speed;

		if (!IsBlendOut())
		{
			if (mWeightBlendInDelta == 0)
			{
				mWeight = 1;
			}
			else
			{
				mWeight += elapsedTime * mWeightBlendInDelta;

				if (mWeight >= 1)
				{
					mWeight = 1;
					mWeightBlendInDelta = 0;
				}
			}
		}
		else
		{
			if (mWeight == 0)
				return false;

			if (mWeightBlendOutDelta > 0)
			{
				mWeight -= elapsedTime * mWeightBlendOutDelta;
				mWeight = Max(0.0f, mWeight);
			}
		}
		
		//
		if (mCatchedAnim == NULL && mesh->GetSource() != NULL)
		{
			mCatchedAnim = mesh->GetSource()->GetAnimation(mName.c_str());
			if (mCatchedAnim == NULL)
				return true;

			mSkelAnimLastKeys.Resize(mCatchedAnim->GetSkeletonAnimationCount());
			for (int i = 0; i < mSkelAnimLastKeys.Size(); ++i)
				mSkelAnimLastKeys[i] = -1;
		}

		if (mCatchedAnim != NULL)
		{
			float length = mCatchedAnim->GetLength();

			if (mPosition >= length)
			{
				if (mBlendInfo.Looped)
				{
					while (mPosition > length)
						mPosition -= length;

					for (int i = 0; i < mSkelAnimLastKeys.Size(); ++i)
						mSkelAnimLastKeys[i] = -1;
				}
				else
				{
					mPosition = length;
					return false;
				}
			}

			for (int i = 0; i < mCatchedAnim->GetEventCount(); ++i)
			{
				Animation::Event * e = mCatchedAnim->GetEvent(i);
				if (e->time > lastPosition && e->time <= mPosition)
					E_Event(e);
			}

			Skeleton * skel = mesh->GetSkeleton().c_ptr();
			if (skel != NULL && !mesh->IsSkeletonShared())
			{
				for (int i = 0; i < mCatchedAnim->GetSkeletonAnimationCount(); ++i)
				{
					SkeletonAnimation * skelAnim = mCatchedAnim->GetSkeletonAnimation(i);

					skelAnim->Update(skel, this, mSkelAnimLastKeys[i]);
				}
			}
		}

		return true;
	}

}
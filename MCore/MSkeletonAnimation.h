/*
*	Skeleton Animation
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"

namespace Rad {

	class Skeleton;
	class AnimationController;

	class M_ENTRY SkeletonAnimation
	{
		DECLARE_ALLOC();

	public:
		struct KeyFrame {
			float time;

			Float3 position;
			Quat rotation;
			Float3 scale;

			bool Equal(const KeyFrame & rk) const
			{
				return
					position == rk.position &&
					rotation == rk.rotation &&
					scale == rk.scale;
			}
		};

	public:
		SkeletonAnimation(short handle);
		~SkeletonAnimation();

		void 
			Update(Skeleton * skel, AnimationController * controller, int & last);
		void
			Optimize();

		KeyFrame *
			CreateKeyFrame(float time);
		void
			RemoveKeyFrame(int index);
		int 
			GetFrameCount();
		KeyFrame * 
			GetKeyFrame(int index);
		short 
			GetBoneHandle();

	protected:
		short mBoneId;
		Array<KeyFrame> mFrames;
	};

}
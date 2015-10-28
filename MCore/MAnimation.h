/*
*	Animation
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"
#include "MKeyController.h"
#include "MSkeletonAnimation.h"
#include "MVertexAnimation.h"

namespace Rad {

	class Mesh;

	class M_ENTRY Animation
	{
		DECLARE_ALLOC();

	public:
		struct Event
		{
			float time;
			FixedString32 name;
		};

	public:
		Animation(const FixedString32 & name);
		~Animation();

		const FixedString32 & 
			GetName() const { return mName; }

		void 
			SetLength(float length);
		float 
			GetLength() const { return mLength; }

		void
			AddSkeletonAnimation(SkeletonAnimation * anim);
		void
			RemoveSkeletonAnimation(int i);
		SkeletonAnimation * 
			GetSkeletonAnimation(int i) { return mSkelAnims[i]; }
		int 
			GetSkeletonAnimationCount() { return mSkelAnims.Size(); }

		void
			AddVertexAnimation(VertexAnimation * anim);
		void
			RemoveVertexAnimation(int i);
		VertexAnimation *
			GetVertexAnimation(int i) { return mVertexAnims[i]; }
		int 
			GetVertexAnimationCount() { return mVertexAnims.Size(); }

		void
			AddEvent(float time, const FixedString32 & name);
		void
			RemoveEvent(int index);
		Event *
			GetEvent(int index);
		int
			GetEventCount();

		void 
			_calcuLength();

		//void _convertSkinAnim(Skeleton * skel);

	protected:
		FixedString32 mName;
		float mLength;
		Array<SkeletonAnimation *> mSkelAnims;
		Array<VertexAnimation *> mVertexAnims;

		Array<Event> mEvents;
	};

}

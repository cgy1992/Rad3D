/*
*	Animation Controller
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"
#include "MAnimation.h"

namespace Rad {

	struct M_ENTRY BlendInfo
	{
		static const BlendInfo Default;

		float InTime;
		float OutTime;

		float Offset;
		float Weight;
		float Speed;

		bool Looped;
		bool Mixed;
		bool Interpolation;

		BlendInfo()
		{
			InTime = 0.2f;
			OutTime = 0.2f;
			Offset = 0.0f;
			Weight = 1.0f;
			Speed = 1.0f;

			Looped = true;
			Mixed = false;
			Interpolation = true;
		}
	};

	//
	class Mesh;

	class M_ENTRY AnimationController
	{
		DECLARE_POOL_ALLOC();

	public:
		tEvent1<const Animation::Event *> E_Event;

	public:
		AnimationController(const FixedString32 & name, const BlendInfo & bi, int order);
		virtual ~AnimationController();

		const FixedString32 &
			GetName() const { return mName; }
		const BlendInfo & 
			GetBlendInfo() const { return mBlendInfo; }
		int
			GetOrder() const { return mOrder; }

		void 
			SetPosition(float position) { mPosition = position; }
		float 
			GetPosition() const { return mPosition; }

		void
			SetWeiget(float weight) { mBlendInfo.Weight = weight; }
		float
			GetWeight() const { return mWeight * mBlendInfo.Weight; }

		void
			SetSpeed(float speed) { mBlendInfo.Speed = speed; }
		float
			GetSpeed() const { return mBlendInfo.Speed; }

		void
			SetLooped(bool looped) { mBlendInfo.Looped = looped; }
		bool
			IsLooped() const { return mBlendInfo.Looped; }

		void
			BlendIn();
		bool
			IsBlendIn() { return mWeightBlendInDelta > 0; }
		void 
			BlendOut();
		bool 
			IsBlendOut() { return mWeightBlendOutDelta > 0; }

		virtual void
			Reset();
		virtual bool
			Update(Mesh * mesh, float elapsedTime);

	protected:
		FixedString32 mName;

		int mOrder;
		float mPosition;
		BlendInfo mBlendInfo;

		float mWeightBlendInDelta;
		float mWeightBlendOutDelta;
		float mWeight;

		Animation * mCatchedAnim;
		Array<int, t_alloc_pool<int> > mSkelAnimLastKeys;
	};

}
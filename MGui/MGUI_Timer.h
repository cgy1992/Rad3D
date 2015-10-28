/*
*	Timer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Component.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Timer : public Component
	{
		DECLARE_RTTI();

	public:
		tEvent1<Component *> E_Timer;
		tEvent1<Component *> E_End;

	public:
		Timer();
		virtual ~Timer();

		virtual int
			Update(float elapsedTime);

		void 
			SetTime(float interval, float duration);
		void 
			Reset();

		float 
			GetIntervalTime() { return mIntervalTime; }
		float 
			GetDurationTime() { return mDurationTime; }
		float 
			GetTime() { return mTime; }

	protected:
		float mIntervalTime;
		float mDurationTime;
		float mLastTime;
		float mTime;
	};

}}

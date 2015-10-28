#include "MGUI_Timer.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(Timer, Component);

	Timer::Timer()
		: mIntervalTime(0)
		, mDurationTime(0)
		, mTime(0)
		, mLastTime(0)
	{
	}

	Timer::~Timer()
	{
	}

	void Timer::SetTime(float interval, float duration)
	{
		mIntervalTime = interval;
		mDurationTime = duration;
	}

	void Timer::Reset()
	{
		mTime = mLastTime = 0;
	}

	int Timer::Update(float elapsedTime)
	{
		mTime += elapsedTime;

		if (mTime - mLastTime >= mIntervalTime)
		{
			E_Timer(this);
			mLastTime = mTime;
		}

		if (mDurationTime > 0 && mTime > mDurationTime)
		{
			E_End(this);

			return -1;
		}

		return 0;
	}

}}
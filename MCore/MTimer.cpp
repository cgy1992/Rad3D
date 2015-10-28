#include "MTimer.h"
#include "MMath.h"
#include <time.h>


#ifdef M_PLATFORM_WIN32

inline uint64_t __timeGetTime()
{
	static uint64_t s_time = 0;
	static uint32_t s_tick = 0;

	uint32_t c_tick = GetTickCount();

	if (c_tick >= s_tick)
		s_time += c_tick - s_tick;
	else
		s_time += MAX_UINT - s_tick + c_tick;

	s_tick = c_tick;

	return s_time * 1000;
}

#else

#include <sys/time.h>

inline uint64_t __timeGetTime()
{
	timeval tv;

	gettimeofday(&tv, NULL);

	return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

#endif


namespace Rad {

#define ON				1
#define OFF				2
#define SUPENDED		4

#define SAMPLE_TIME_L(tv) tv = __timeGetTime()

	Timer::Timer()
	{
		mLastTime = 0;
		mCurrentTime = 0;
		mState = OFF;
	}

	Timer::~Timer()
	{
	}

	void Timer::Start()
	{
		SAMPLE_TIME_L(mLastTime);
		mCurrentTime = mLastTime;
		mState = ON;
	}

	void Timer::Stop()
	{
		if (mState == ON)
		{
			SAMPLE_TIME_L(mCurrentTime);
			mState = OFF;
		}
	}

	void Timer::Supend()
	{
		if (mState == ON)
		{
			SAMPLE_TIME_L(mCurrentTime);
			mState = SUPENDED;
		}
	}

	void Timer::Resume()
	{
		if (mState == SUPENDED)
		{
			SAMPLE_TIME_L(mLastTime);
			mCurrentTime = mLastTime;
			mState = ON;
		}
	}

	float Timer::ElapsedTime()
	{
		if (mState == ON)
		{
			SAMPLE_TIME_L(mCurrentTime);

			if (mCurrentTime < mLastTime)
			{
				mLastTime = 0;
			}
		}

		return float(mCurrentTime - mLastTime) / 1000000.0f;
	}

}

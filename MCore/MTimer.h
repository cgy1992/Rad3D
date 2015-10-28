/*
*	Timer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include <time.h>
#include "MMemory.h"

namespace Rad {

	class M_ENTRY Timer
	{
		DECLARE_ALLOC();

	public:
		Timer();
		~Timer();

		void
			Start();
		void
			Stop();
		void 
			Supend();
		void 
			Resume();

		float 
			ElapsedTime();

	protected:
		uint64_t mLastTime;
		uint64_t mCurrentTime;
		int mState;
	};

	inline void DataTimeToString(char * str, struct tm * t)
	{
		sprintf(str,"%-4d-%02d-%02d %02d:%02d:%02d ", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
	}
}

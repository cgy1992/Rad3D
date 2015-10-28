/*
*	NavThread
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MThread.h"
#include "NavPathFinder.h"

namespace Rad {

	class M_ENTRY NavThread
	{
	public:
		struct NavQuery{
			NavPathFinderPtr finder;
			Float3 start;
			Float3 end;
		};

	public:
		NavThread();
		~NavThread();

		void 
			SetEnable(bool enable);
		bool 
			IsEnable();

		void 
			Query(NavPathFinderPtr finder, const Float3 & start, const Float3 & end);
		void 
			Update();

		void 
			DoThreadProc();

	protected:
		Mutex mMutex;
		Thread * mThread;

		Array<NavQuery> mQueryQueue;
		NavQuery mThreadQuery;
		NavPathFinderPtr mFinishFinder;
	};

}
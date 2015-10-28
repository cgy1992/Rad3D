#include "MThread.h"

#ifndef M_PLATFORM_WIN32
#include <unistd.h>
#endif

namespace Rad {


#ifdef M_PLATFORM_WIN32

	DWORD WINAPI Thread_Shared_Proc(LPVOID param)
	{
		Thread * p = (Thread *)param;

		p->Run();

		return 0;
	}

#else

	void * Thread_Shared_Proc(void * param)
	{
		Thread * p = (Thread *)param;

		p->Run();

		pthread_exit((void *)0);
	}

#endif

	void Thread::Sleep(unsigned long ms)
	{
		if (ms > 0)
		{
#ifdef M_PLATFORM_WIN32
			::Sleep(ms);
#else
			usleep(ms * 1000);
#endif
		}
	}

	Thread::Thread(bool start)
	{
		mThreadId = 0;
		mPriority = NORMAL;
		mStatus = RUN;

		if (start)
		{
			Start();
		}
	}

	Thread::~Thread()
	{
		Stop();
	}

	void Thread::SetPriority(PRIORITY p)
	{
		d_assert (mThreadId != NULL);

		mPriority = p;

#ifdef M_PLATFORM_WIN32
		int n;
		switch(p) 
		{
		case LOW:
			n = THREAD_PRIORITY_BELOW_NORMAL;
			break;
		case HIGH:
			n = THREAD_PRIORITY_ABOVE_NORMAL;
			break;
		default:
			n = THREAD_PRIORITY_NORMAL;
		}

		::SetThreadPriority(mThreadId, n);
#endif
	}

	void Thread::Start()
	{
		d_assert (mThreadId == NULL);

#ifdef M_PLATFORM_WIN32
		mThreadId = CreateThread(NULL, 0, Thread_Shared_Proc, (void *)this, 0, NULL);
#else
		pthread_create(&mThreadId, NULL, Thread_Shared_Proc, (void *)this);
#endif
	}

	void Thread::Suspend()
	{
		if (mThreadId != 0)
		{
#ifdef M_PLATFORM_WIN32
			SuspendThread(mThreadId);
#endif
			mStatus = SUSPEND;
		}
	}

	void Thread::Resume()
	{
		if (mThreadId != 0)
		{
#ifdef M_PLATFORM_WIN32
			ResumeThread(mThreadId);
#endif

			mStatus = RUN;
		}
	}

	void Thread::Stop()
	{
		if (mThreadId != 0)
		{
			mStatus = STOP;

#ifdef M_PLATFORM_WIN32
			WaitForSingleObject(mThreadId, INFINITE);

			TerminateThread(mThreadId, 0);
			CloseHandle(mThreadId);
#else
			pthread_join(mThreadId, NULL);
#endif

			mThreadId = 0;
		}
	}

}
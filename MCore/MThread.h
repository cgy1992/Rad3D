/*
*	Thread
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

#ifndef M_PLATFORM_WIN32
#include <pthread.h>
#endif

namespace Rad {

	class Mutex
	{
#ifdef M_PLATFORM_WIN32
		CRITICAL_SECTION mSection;

	public:
		Mutex()
		{
			InitializeCriticalSection(&mSection);
		}

		~Mutex()
		{
			DeleteCriticalSection(&mSection);
		}

		void Lock()
		{
			EnterCriticalSection(&mSection);
		}

		void Unlock()
		{
			LeaveCriticalSection(&mSection);
		}

#else
		pthread_mutex_t mSection;

	public:
		Mutex()
		{
			pthread_mutex_init(&mSection, NULL);
		}

		~Mutex()
		{
			pthread_mutex_destroy(&mSection);
		}

		void Lock()
		{
			pthread_mutex_lock(&mSection);
		}

		void Unlock()
		{
			pthread_mutex_unlock(&mSection);
		}

#endif
	};

	struct _MutexHelper
	{
		Mutex * mm;

		_MutexHelper(Mutex * m) : mm(m) { mm->Lock(); }
		~_MutexHelper() { mm->Unlock(); }
	};

#define MUTEX_ENTER(mutex) _MutexHelper __mmhelper(&mutex)

	class M_ENTRY Thread
	{
		DECLARE_ALLOC();

	public:
		enum PRIORITY {
			NORMAL,
			LOW,
			HIGH,
		};

		enum STATUS {
			RUN,
			SUSPEND,
			STOP,
		};

	public:
		static void Sleep(unsigned long ms);

	public:
		Thread(bool start = true);
		virtual ~Thread();

		void
			SetPriority(PRIORITY p);
		PRIORITY
			GetProperty() { return mPriority; }
		STATUS
			GetStatus() { return mStatus; }

		void
			Start();
		void
			Suspend();
		void
			Resume();
		void
			Stop();

		virtual void
			Run() = 0;

	protected:
#ifdef M_PLATFORM_WIN32
		HANDLE mThreadId;
#else
		pthread_t mThreadId;
#endif
		PRIORITY mPriority;
		STATUS mStatus;
	};

	class M_ENTRY ThreadStandard : public Thread
	{
	public:
		typedef void (*Function)(void * param);

	public:
		ThreadStandard(Function fn, void * param, bool start = true);
		virtual ~ThreadStandard();

		virtual void
			Run();

	protected:
		Function mFunction;
		void * mParam;
	};

}
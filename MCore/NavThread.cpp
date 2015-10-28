#include "NavThread.h"

namespace Rad {

	class NavRunnable : public Thread
	{
	public:
		NavRunnable(NavThread * p) { mNavThread = p; }
		virtual ~NavRunnable() {}

		virtual void Run()
		{
			mNavThread->DoThreadProc();
		}

	protected:
		NavThread * mNavThread;
	};

	NavThread::NavThread()
		: mThread(NULL)
	{
	}

	NavThread::~NavThread()
	{
		SetEnable(false);
	}

	void NavThread::SetEnable(bool enable)
	{
		if (enable)
		{
			if (mThread == NULL)
			{
				mThread = new NavRunnable(this);
			}
		}
		else
		{
			if (mThread != NULL)
			{
				delete mThread;
				mThread = NULL;
			}
		}
	}

	bool NavThread::IsEnable()
	{
		return mThread != NULL;
	}

	void NavThread::Query(NavPathFinderPtr finder, const Float3 & start, const Float3 & end)
	{
		MUTEX_ENTER(mMutex);

		NavQuery nq;
		nq.start = start;
		nq.end = end;
		nq.finder = finder;
		mQueryQueue.PushBack(nq);
	}

	void NavThread::Update()
	{
		mMutex.Lock();

		mFinishFinder = mThreadQuery.finder;
		mThreadQuery.finder = NULL;

		mMutex.Unlock();

		if (mFinishFinder != NULL)
		{
			mFinishFinder->E_ThreadCallback();
			mFinishFinder = NULL;
		}
	}

	void NavThread::DoThreadProc()
	{
		while (mThread != NULL && mThread->GetStatus() != Thread::STOP)
		{
			if (mThreadQuery.finder == NULL)
			{
				NavQuery nq;

				mMutex.Lock();

				if (mQueryQueue.Size() > 0)
				{
					nq = mQueryQueue[0];
					mQueryQueue.Erase(0, 1);
				}

				mMutex.Unlock();

				if (nq.finder != NULL)
				{
					nq.finder->Do(mThreadQuery.start, mThreadQuery.end, 4096);

					mMutex.Lock();

					mThreadQuery = nq;

					mMutex.Unlock();
				}
			}

			Thread::Sleep(5);
		}
	}

}


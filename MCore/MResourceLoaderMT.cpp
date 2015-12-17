#include "MResourceLoaderMT.h"
#include "MResourceManager.h"

namespace Rad {

	class ResourceLoaderThread : public Thread
	{
	public:
		ResourceLoaderThread(ResourceLoaderMT * p) { mLoader = p; }
		virtual ~ResourceLoaderThread() {}

		virtual void Run() { mLoader->_ThreadProc(); }

	protected:
		ResourceLoaderMT * mLoader;
	};


	ResourceLoaderMT::ResourceLoaderMT()
	{
		mResourceQueue.Alloc(128);

		mThread = new ResourceLoaderThread(this);
	}

	ResourceLoaderMT::~ResourceLoaderMT()
	{
		delete mThread;
	}

	void ResourceLoaderMT::Load(ResourcePtr pResource)
	{
		MUTEX_ENTER(mMutex);

		int index = 0;

		while (index < mResourceQueue.Size())
		{
			if (pResource->GetPriority() > mResourceQueue[index]->GetPriority())
				break;

			++index;
		}

		mResourceQueue.Insert(index, pResource);
	}

	void ResourceLoaderMT::Update()
	{
		Lock();

		mStream = mStreamThread;
		mStreamThread.pResource = NULL;
		mStreamThread.pDataStream = NULL;

		Unlock();

		if (mStream.pResource != NULL)
		{
			mStream.pResource->_load(mStream.pDataStream);

			if (mStream.pResource->_getDependences() == NULL)
			{
				mStream.pResource->OnLoad();
			}
		}
		mStream.pResource = NULL;
		mStream.pDataStream = NULL;
	}

	void ResourceLoaderMT::_ThreadProc()
	{
		while (mThread != NULL && mThread->GetStatus() != Thread::STOP)
		{
			if (mStreamThread.pResource != NULL)
			{
				Thread::Sleep(5);
				continue;
			}

			Lock();

			if (mResourceThread == NULL && mResourceQueue.Size() > 0)
			{
				mResourceThread = mResourceQueue[0];
				mResourceQueue.Erase(0, 1);
			}

			Unlock();

			if (mResourceThread == NULL)
			{
				Thread::Sleep(5);

				continue;
			}

			DataStreamPtr pDataStream = ResourceManager::Instance()->OpenResource(mResourceThread->GetSourceName());

			Lock();

			mStreamThread.pResource = mResourceThread;
			mStreamThread.pDataStream = pDataStream;

			mResourceThread = NULL;
			pDataStream = NULL;

			Unlock();

			Thread::Sleep(5);
		}
	}

}
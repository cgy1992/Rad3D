/*
*	ResourceLoaderMT
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MThread.h"
#include "MResourceLoader.h"

namespace Rad {

	class M_ENTRY ResourceLoaderMT : public ResourceLoader
	{
		struct ResourceStreamPair
		{
			ResourcePtr pResource;
			DataStreamPtr pDataStream;
		};

	public:
		ResourceLoaderMT();
		virtual ~ResourceLoaderMT();

		virtual void 
			Load(ResourcePtr pResource);

		virtual bool
			IsMultiThread() { return true; }

		void
			Lock() { mMutex.Lock(); }
		void
			Unlock() { mMutex.Unlock(); }

		void 
			Update();

		void 
			_ThreadProc();

	protected:
		Thread * mThread;
		Mutex mMutex;

		Array<ResourcePtr> mResourceQueue;
		ResourcePtr mResourceThread;

		ResourceStreamPair mStream;
		ResourceStreamPair mStreamThread;
	};

}
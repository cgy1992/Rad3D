/*
*	DataStream Pool
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDataStream.h"
#include "MMap.h"

namespace Rad {

	class M_ENTRY DataStreamPool
	{
	public:
		DataStreamPool();
		~DataStreamPool();

		void 
			Clear();
		void 
			Insert(const String & key, DataStreamPtr stream);
		void 
			Remove(const String & key);
		DataStreamPtr 
			Find(const String & key);

	protected:
		Map<String, DataStreamPtr> mDataStreamMap;
	};

}

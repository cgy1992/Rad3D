#include "MDataStreamPool.h"

namespace Rad {

	DataStreamPool::DataStreamPool()
	{
	}

	DataStreamPool::~DataStreamPool()
	{
		Clear();
	}

	void DataStreamPool::Clear()
	{
		mDataStreamMap.Clear();
	}

	void DataStreamPool::Insert(const String & key, DataStreamPtr stream)
	{
		d_assert (stream != NULL);

		mDataStreamMap.Insert(key, stream);
	}

	void DataStreamPool::Remove(const String & key)
	{
		int index = mDataStreamMap.Find(key);
		if (index != -1)
		{
			mDataStreamMap[index].value = NULL;
			mDataStreamMap.Erase(index);
		}
	}
	
	DataStreamPtr DataStreamPool::Find(const String & key)
	{
		int index = mDataStreamMap.Find(key);
		if (index != -1)
		{
			return mDataStreamMap[index].value;
		}

		return NULL;
	}

}


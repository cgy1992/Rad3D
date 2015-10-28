/*
*	UserData
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MField.h"

namespace Rad {

	template <class T, int COUNT>
	struct UserDataT
	{
		void * data[COUNT];

		UserDataT()
		{
			for (int i = 0; i < COUNT; ++i)
			{
				data[i] = NULL;
			}
		}
	};

#define DECLARE_USERDATA(T, count) \
protected: \
	UserDataT<T, count> mUserData; \
	\
public: \
	void SetUserData(int i, void * data) \
	{ \
		d_assert(i < count); \
		mUserData.data[i] = data; \
	} \
	\
	void * GetUserData(int i) \
	{ \
		d_assert(i < count); \
		return mUserData.data[i]; \
	} \
	\
	const void * GetUserData(int i) const\
	{ \
		d_assert(i < count); \
		return mUserData.data[i]; \
	}

}
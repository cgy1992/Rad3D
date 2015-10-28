/*
*	Singleton
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

#ifdef _MSC_VER
#pragma warning(disable : 4661)
#endif

namespace Rad {

	template <class T>
	struct Singleton
	{
		static T * msInstance;

	public:
		Singleton()
		{
			d_assert(msInstance == NULL);

			msInstance = static_cast<T *>(this);
		}

		~Singleton()
		{
			msInstance = NULL;
		}

		static T * Instance()
		{
			d_assert(msInstance != NULL);

			return msInstance;
		}
	};

#define ImplementSingleton(classname) \
	template<> classname * Singleton<classname>::msInstance = NULL;

}
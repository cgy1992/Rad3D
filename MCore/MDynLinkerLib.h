/*
*	Dynamic Linker Lib
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDebug.h"

#ifndef M_PLATFORM_WIN32
#include <dlfcn.h>
#endif

namespace Rad {

	class DynLinkerLib
	{
	public:
		DynLinkerLib() : mHandle(NULL)
		{
		}

		~DynLinkerLib()
		{
			Close();
		}

		bool Open(const char * filename)
		{
			d_assert (mHandle == NULL);

#ifdef M_PLATFORM_WIN32
			mHandle = LoadLibrary(filename);
#else
			mHandle = dlopen(filename, RTLD_LAZY | RTLD_GLOBAL);
#endif
			return IsOpen();
		}

		bool IsOpen()
		{
			return mHandle != NULL;
		}

		void Close()
		{
			if (mHandle == NULL)
				return ;

#ifdef M_PLATFORM_WIN32
			FreeLibrary(mHandle);
#else
			dlclose(mHandle);
#endif

			mHandle = NULL;
		}

		void * GetFunction(const char * funcName)
		{
			d_assert (IsOpen());

#ifdef M_PLATFORM_WIN32
			return GetProcAddress(mHandle, funcName);
#else
			return dlsym(mHandle, funcName);
#endif
		}

	protected:

#ifdef M_PLATFORM_WIN32
		HMODULE mHandle;
#else
		void * mHandle;
#endif
	};

}
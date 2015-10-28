/*
*	Log
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFile.h"
#include "MSingleton.h"

#ifndef M_NO_LOG
	#define d_log Rad::Log::Instance()->Print
#else 
	#define d_log
#endif

namespace Rad {

	class M_ENTRY Log : public Singleton<Log>
	{
	public:
		Log(const char * filename);
		~Log();

		void 
			Print(const char * format, ...);

		FILE *
			FileHandle() { return mFile; }

	protected:
		FILE * mFile;
	};
}


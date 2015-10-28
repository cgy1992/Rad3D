/*
*	Log2
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include <time.h>
#include "MFile.h"
#include "MSingleton.h"

namespace Rad {

#define d_log2 Rad::Log2::Instance()

	struct M_ENTRY Log2 : public Singleton<Log2>
	{
	public:
		enum {
			BLACK,
			RED,
			GREEN,
			BROWN,
			BLUE,
			MAGENTA,
			CYAN,
			GREY,
			YELLOW,
			LRED,
			LGREEN,
			LBLUE,
			LMAGENTA,
			LCYAN,
			WHITE
		};

		static const int Color_count = int(WHITE)+1;

	public:
		Log2(const char * filename, const char * mode = "a");
		~Log2();

		void 
			SetColor(bool stdout_stream, int color);
		void 
			ResetColor(bool stdout_stream);

		void
			SetConsoleEnable(bool enable);
		bool
			GetConsoleEnable();

		void 
			print(const char * err, ...);
		void 
			debug(const char * err, ...);
		void 
			warning(const char * err, ...);
		void 
			error(const char * err, ...);

		String 
			GetTimestampString();
		void 
			outTime();
		void 
			outTimestamp(FILE* fp);

		static String 
			GetDateString();

	protected:
		FILE * mFile;
		bool mConsoleEnable;
	};

}


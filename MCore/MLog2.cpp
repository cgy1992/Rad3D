#include "MLog2.h"

namespace Rad {

	ImplementSingleton(Log2);

	Log2::Log2(const char * filename, const char * mode)
		: mConsoleEnable(false)
	{
		mFile = fopen(filename, mode);
		if (mFile == NULL)
		{
			error("log file open failed");
		}
	}

	Log2::~Log2()
	{
		if (mFile)
		{
			fclose(mFile);
		}
	}

	void Log2::SetColor(bool stdout_stream, int color)
	{
		d_assert (color < Color_count);

#ifdef M_PLATFORM_WIN32

		static WORD WinColorFG[Color_count] =
		{
			0,                                                  // BLACK
			FOREGROUND_RED,                                     // RED
			FOREGROUND_GREEN,                                   // GREEN
			FOREGROUND_RED | FOREGROUND_GREEN,                  // BROWN
			FOREGROUND_BLUE,                                    // BLUE
			FOREGROUND_RED |                    FOREGROUND_BLUE,// MAGENTA
			FOREGROUND_GREEN | FOREGROUND_BLUE,                 // CYAN
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,// WHITE
			// YELLOW
			FOREGROUND_RED | FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
			// RED_BOLD
			FOREGROUND_RED |                                      FOREGROUND_INTENSITY,
			// GREEN_BOLD
			FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
			FOREGROUND_BLUE | FOREGROUND_INTENSITY,             // BLUE_BOLD
			// MAGENTA_BOLD
			FOREGROUND_RED |                    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
			// CYAN_BOLD
			FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
			// WHITE_BOLD
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
		};

		HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE );
		SetConsoleTextAttribute(hConsole, WinColorFG[color]);
#else

		enum ANSITextAttr
		{
			TA_NORMAL=0,
			TA_BOLD=1,
			TA_BLINK=5,
			TA_REVERSE=7
		};

		enum ANSIFgTextAttr
		{
			FG_BLACK=30, FG_RED,  FG_GREEN, FG_BROWN, FG_BLUE,
			FG_MAGENTA,  FG_CYAN, FG_WHITE, FG_YELLOW
		};

		enum ANSIBgTextAttr
		{
			BG_BLACK=40, BG_RED,  BG_GREEN, BG_BROWN, BG_BLUE,
			BG_MAGENTA,  BG_CYAN, BG_WHITE
		};

		static uint8_t UnixColorFG[Color_count] =
		{
			FG_BLACK,                                           // BLACK
			FG_RED,                                             // RED
			FG_GREEN,                                           // GREEN
			FG_BROWN,                                           // BROWN
			FG_BLUE,                                            // BLUE
			FG_MAGENTA,                                         // MAGENTA
			FG_CYAN,                                            // CYAN
			FG_WHITE,                                           // WHITE
			FG_YELLOW,                                          // YELLOW
			FG_RED,                                             // LRED
			FG_GREEN,                                           // LGREEN
			FG_BLUE,                                            // LBLUE
			FG_MAGENTA,                                         // LMAGENTA
			FG_CYAN,                                            // LCYAN
			FG_WHITE                                            // LWHITE
		};

		fprintf((stdout_stream? stdout : stderr), "\x1b[%d%sm",UnixColorFG[color],(color>=YELLOW&&color<Color_count ?";1":""));
#endif
	}

	void Log2::ResetColor(bool stdout_stream)
	{
#ifdef M_PLATFORM_WIN32
		HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE );
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED );
#else
		fprintf(( stdout_stream ? stdout : stderr ), "\x1b[0m");
#endif
	}

	void Log2::SetConsoleEnable(bool enable)
	{
		mConsoleEnable = enable;
	}

	bool Log2::GetConsoleEnable()
	{
		return mConsoleEnable;
	}

	void Log2::print(const char * err, ...)
	{
		if (mConsoleEnable)
		{
			SetColor(false, WHITE);

			va_list ap;
			va_start(ap, err);
			vfprintf(stderr, err, ap);
			va_end(ap);
			fflush(stdout);

			printf( "\n" );
		}
	}

	void Log2::debug(const char * err, ...)
	{
		if (mConsoleEnable)
		{
			SetColor(false, WHITE);

			va_list ap;
			va_start(ap, err);
			vfprintf(stderr, err, ap);
			va_end(ap);
			fflush(stdout);

			printf( "\n" );
		}

		if (mFile)
		{
			va_list ap;

			outTimestamp(mFile);
			va_start(ap, err);
			vfprintf(mFile, err, ap);
			fprintf(mFile, "\n" );
			va_end(ap);
			fflush(mFile);
		}
	}

	void Log2::warning(const char * err, ...)
	{
		if (mConsoleEnable)
		{
			SetColor(false, YELLOW);

			va_list ap;
			va_start(ap, err);
			vfprintf(stderr, err, ap);
			va_end(ap);
			fflush(stdout);

			printf( "\n" );

			ResetColor(false);
		}

		if (mFile)
		{
			va_list ap;

			outTimestamp(mFile);
			fprintf(mFile, "Warning: ");
			va_start(ap, err);
			vfprintf(mFile, err, ap);
			fprintf(mFile, "\n" );
			va_end(ap);
			fflush(mFile);
		}
	}

	void Log2::error(const char * err, ...)
	{
		if (mConsoleEnable)
		{
			SetColor(false, RED);

			va_list ap;
			va_start(ap, err);
			vfprintf(stderr, err, ap);
			va_end(ap);
			fflush(stdout);

			printf( "\n" );

			ResetColor(false);
		}

		if (mFile)
		{
			va_list ap;

			outTimestamp(mFile);
			fprintf(mFile, "Error: ");
			va_start(ap, err);
			vfprintf(mFile, err, ap);
			fprintf(mFile, "\n" );
			va_end(ap);
			fflush(mFile);
		}
	}

	String Log2::GetTimestampString()
	{
		char buf[32];

		time_t t = time(NULL);
		tm* aTm = localtime(&t);
		//       YYYY   year
		//       MM     month (2 digits 01-12)
		//       DD     day (2 digits 01-31)
		//       HH     hour (2 digits 00-23)
		//       MM     minutes (2 digits 00-59)
		//       SS     seconds (2 digits 00-59)
		sprintf(buf, "%04d-%02d-%02d_%02d-%02d-%02d",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);

		return buf;
	}

	void Log2::outTime()
	{
		time_t t = time(NULL);
		tm* aTm = localtime(&t);
		//       YYYY   year
		//       MM     month (2 digits 01-12)
		//       DD     day (2 digits 01-31)
		//       HH     hour (2 digits 00-23)
		//       MM     minutes (2 digits 00-59)
		//       SS     seconds (2 digits 00-59)
		printf("%02d:%02d:%02d ",aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
	}

	void Log2::outTimestamp(FILE* fp)
	{
		time_t t = time(NULL);
		tm* aTm = localtime(&t);
		//       YYYY   year
		//       MM     month (2 digits 01-12)
		//       DD     day (2 digits 01-31)
		//       HH     hour (2 digits 00-23)
		//       MM     minutes (2 digits 00-59)
		//       SS     seconds (2 digits 00-59)
		fprintf(fp,"%-4d-%02d-%02d %02d:%02d:%02d ",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
	}

	String Log2::GetDateString()
	{
		char buf[32];

		time_t t = time(NULL);
		tm* aTm = localtime(&t);
		//       YYYY   year
		//       MM     month (2 digits 01-12)
		//       DD     day (2 digits 01-31)
		//       HH     hour (2 digits 00-23)
		//       MM     minutes (2 digits 00-59)
		//       SS     seconds (2 digits 00-59)
		sprintf(buf, "%04d-%02d-%02d",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday);

		return buf;
	}

}

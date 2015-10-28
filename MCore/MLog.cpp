#include "MLog.h"

namespace Rad {

	ImplementSingleton(Log);

	Log::Log(const char * filename)
	{
		mFile = fopen(filename, "wb");
		if (mFile == NULL)
		{
			d_printf("?: Log file can't open.");
		}
	}

	Log::~Log()
	{
		if (mFile)
		{
			fclose(mFile);
		}
	}

	void Log::Print(const char * format, ...)
	{
		char text[1024 * 2];

		va_list arglist;

		va_start(arglist, format);
		vsprintf(text, format, arglist);
		va_end(arglist);

#ifdef M_PLATFORM_WIN32
		strcat(text, "\r\n");
#else
		strcat(text, "\n");
#endif

		d_printf(text);

		if (mFile)
		{
			fwrite(text, 1, sizeof(char) * strlen(text), mFile);

#ifdef M_DEBUG
			fflush(mFile);
#endif
		}
	}

}

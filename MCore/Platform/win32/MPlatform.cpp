#include "MPlatform.h"
#include <stdio.h>
#include <stdarg.h>

namespace Rad {

	void d_printf(const char * fmt, ...)
	{
		const int TEXT_BUFFER_SIZE = 4096;
		char text[TEXT_BUFFER_SIZE];

		va_list arglist;

		va_start(arglist, fmt);
		vsprintf(text, fmt, arglist);
		va_end(arglist);

		strcat(text, "\n");

		::OutputDebugString(text);
	}

}

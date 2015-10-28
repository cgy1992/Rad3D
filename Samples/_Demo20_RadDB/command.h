#pragma once

#include "RadDBClient.h"

using namespace Rad;

inline char * _getcmd(char * str)
{
	while (*str)
	{
		if (*str == ' ')
		{
			*str++ = 0;
			break;
		}

		++str;
	}

	while (*str)
	{
		if (*str != ' ')
			break;

		++str;
	}

	return str;
}

typedef void (*function)(rdb_handle handle, char * args);

void
	OnQuery(rdb_handle handle, char * args);
void 
	OnBatchInsert(rdb_handle handle, char * args);
/*
*	RadDB Client
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "RadDBType.h"

#define RDBC_STATIC_MEMORY_CHANEL 3

namespace Rad {

	M_ENTRY const char *
		rdbc_getError(rdb_handle handle);

	M_ENTRY void
		rdbc_init();
	M_ENTRY void
		rdbc_shutdown();

	M_ENTRY rdb_handle
		rdbc_open(const char * host, int port, const char * user, const char * passworld, int timeout = 3);
	M_ENTRY void
		rdbc_close(rdb_handle handle, int waittime);
	M_ENTRY int
		rdbc_mode(rdb_handle handle);

	M_ENTRY int
		rdbc_heartbeat(rdb_handle handle);

	M_ENTRY int
		rdbc_query(rdb_handle handle, rdb_collection * collection, const char * rsql, IObject * obj);
}
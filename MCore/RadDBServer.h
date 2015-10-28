/*
*	RadDB Server
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "RadDBType.h"

namespace Rad {

	M_ENTRY bool
		rdbc_server_init(int port, int timeout = 20);
	M_ENTRY void
		rdbc_server_shutdown();
	M_ENTRY void
		rdbc_server_select(int seconds, int microseconds);
}
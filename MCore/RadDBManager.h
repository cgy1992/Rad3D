/*
*	RadDB Manager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMap.h"
#include "MLog2.h"
#include "RadDB.h"
#include "RadDBTable.h"

namespace Rad {

	struct rdb_user
	{
		DECLARE_POOL_ALLOC();

		FixedString32 name;
		FixedString32 password;
		int mode;
	};

	struct rdb_manager : public Singleton<rdb_manager>
	{
		char error[256];
		String floder;

		Log2 * log;
		Array<rdb_user *> users;
		Map<int, rdb_table *> tables;

		void
			load();
		void
			save();
	};
}


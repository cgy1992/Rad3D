/*
*	RadDB
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "RadDBType.h"

namespace Rad {

	//
	M_ENTRY void
		rdb_setError(const char * fmt, ...);
	M_ENTRY const char *
		rdb_getError();

	M_ENTRY bool
		rdb_init(const char * dbfloder, const char * log, bool console);
	M_ENTRY void
		rdb_shutdown();
	M_ENTRY bool
		rdb_load();

	M_ENTRY int
		rdb_createUser(const char * username, const char * passworld, int mode);
	M_ENTRY int
		rdb_deleteUser(const char * username);
	M_ENTRY int
		rdb_alterUser(const char * username, const char * passworld, int mode);
	M_ENTRY int
		rdb_verifyUser(const char * username, const char * passworld);
	M_ENTRY int
		rdb_getUserMode(const char * username);

	M_ENTRY int
		rdb_createTable(const char * db, const char * name, const char * classname, int mode = 0);
	M_ENTRY int
		rdb_deleteTable(const char * db, const char * name);
	M_ENTRY int
		rdb_optimizeTable(const char * db, const char * name);
	M_ENTRY rdb_handle
		rdb_getTable(const char * dbtable);
	
	M_ENTRY void
		rdb_regFunc(const char * name, rdb_function pfn);

#define rdb_check_args1(args, type0) \
	(args->Size() >= 1 && args->At(0).Type() == type0)
#define rdb_check_args2(args, type0, type1) \
	(args->Size() >= 2 && args->At(0).Type() == type0 && args->At(1).Type() == type1)
#define rdb_check_args3(args, type0, type1, type2) \
	(args->Size() >= 3 && args->At(0).Type() == type0 && args->At(0).Type() == type1 && args->At(2).Type() == type2)
#define rdb_check_args4(args, type0, type1, type2, type3) \
	(args->Size() >= 4 && args->At(0).Type() == type0 && args->At(0).Type() == type1 && args->At(2).Type() == type2 && args->At(3).Type() == type3)
}
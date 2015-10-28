/*
*	RadDB Script
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "RadDB.h"

namespace Rad {

	bool 
		rdb_property(IObject * obj, rdb_value * result, const rdb_vlist * args);
	bool 
		rdb_typeof(IObject * obj, rdb_value * result, const rdb_vlist * args);
	bool 
		rdb_kindof(IObject * obj, rdb_value * result, const rdb_vlist * args);

	bool
		rdb_index(IObject * obj, rdb_value * result, const rdb_vlist * args);
	bool
		rdb_count(IObject * obj, rdb_value * result, const rdb_vlist * args);
	bool
		rdb_offset(IObject * obj, rdb_value * result, const rdb_vlist * args);
	bool
		rdb_orderby(IObject * obj, rdb_value * result, const rdb_vlist * args);
}
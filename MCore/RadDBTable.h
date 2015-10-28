/*
*	RadDB Table
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "RadDB.h"

namespace Rad {

	struct rdb_record
	{
		word i_blockid;
		word i_size;
		dword i_offset;
		dword i_priority;

		IObject * object;

		rdb_record()
		{
			object = NULL;
		}

		void init()
		{
			i_blockid = 0;
			i_size = 0;
			i_offset = 0;
			i_priority = 0;

			object = NULL;
		}
	};

#define RDB_BLOCK_RECORDS (4 * 1024)
#define RDB_PRIMARY_STRING_MAX_LEN 64

	//
	struct rdb_table
	{
		FixedString32 i_name;
		FixedString32 i_dbname;
		FixedString32 i_classname;
		int i_mode;

		IObject * i_object;
		FixedArray<const Property *, 4> i_primaryks;
		int i_queryblock;
		Array<rdb_record *> i_btree;
		Allocator<rdb_record, RDB_BLOCK_RECORDS> i_alloc;

		Map<int, byte *> i_blockcache;

		rdb_table()
		{
			i_object = NULL;
			i_queryblock = 0;
			i_mode = 0;
		}

		~rdb_table()
		{
			b_free_cache_all();

			safe_delete (i_object);

			for (int i = 0; i < i_btree.Size(); ++i)
			{
				safe_delete (i_btree[i]->object);
			}
		}

		bool
			load();
		void
			save();
		
		rdb_record *
			alloc_record();
		void
			free_record(rdb_record * p);
		rdb_record *
			record(int i) { return i_btree[i]; }
		int
			record_size() { return i_btree.Size(); }
		int
			record_compare(const rdb_record & r1, const rdb_record & r2);

		bool
			t_append_record(rdb_record & r);

		byte *
			b_load_cache(int id);
		void
			b_free_cache(int id);
		void
			b_free_cache_all();
		void
			b_save_block(int id);
		bool
			b_load_record(rdb_record & r);
		bool
			b_append_record(rdb_record & r);
		bool
			b_update_record(rdb_record & r);
		int
			b_num_records(int blockid);
		int
			b_block_size(int blockid);
		int
			b_query_block();
		
		bool
			build();
		void
			optimize();
		
		int
			check_unique(rdb_record & r);
		void
			insert(rdb_record & r);
		void
			update(int ri, rdb_record & r);
		void
			remove(int ri);
		void
			search(int & first, int & last, int i, const rdb_value & _min, const rdb_value & _max);
	};

}


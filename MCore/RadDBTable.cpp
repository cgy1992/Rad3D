#include "RadDBTable.h"
#include "RadDBManager.h"
#include "RadDBSql.h"
#include "MFunctional.h"

namespace Rad {

#define K_TB_RECORD (int)(0x3001)

	bool rdb_table::load()
	{
		d_log2->print("load table '%s/%s'...", i_dbname.c_str(), i_name.c_str());

		String filename = 
			rdb_manager::Instance()->floder + "/" + i_dbname.c_str() + "/" + i_name.c_str() + "/0.table";

		FILE * fp = fopen(filename.c_str(), "rb");
		if (!fp)
		{
			d_log2->error("table open failed");
			return false;
		}
		
		ISerializerF IS(fp, true);
		int ckId = 0;
		while (IS.Read(&ckId, sizeof(int)))
		{
			if (ckId == K_TB_RECORD)
			{
				rdb_record * record = alloc_record();
				IS >> record->i_blockid;
				IS >> record->i_offset;
				IS >> record->i_size;
				IS >> record->i_priority;

				i_btree.PushBack(record);
			}
			else if (ckId != 0)
			{
				d_log2->error("table chunk error, table '%s!", i_name.c_str());
				break;
			}
		}

		for (int i = 0; i < record_size(); ++i)
		{
			if (!b_load_record(*record(i)))
				return false;
		}

		b_free_cache_all();

		for (int i = 0; i < i_btree.Size(); ++i)
		{
			if (i_btree[i]->object == NULL)
			{
				d_log2->error("record object load field.");
				i_btree.Erase(i--);
			}
		}

		return true;
	}

	void rdb_record_osl(rdb_record & r, OSerializer & OS)
	{
		OS << K_TB_RECORD;
		OS << r.i_blockid;
		OS << r.i_offset;
		OS << r.i_size;
		OS << r.i_priority;
	}

	void rdb_table::save()
	{
		d_log2->print("save table '%s/%s'...", i_dbname.c_str(), i_name.c_str());

		String filename = 
			rdb_manager::Instance()->floder + "/" + i_dbname.c_str() + "/" + i_name.c_str() + "/0.table";

		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
		{
			d_log2->error("file open failed");
			return ;
		}

		OSerializerF OS(fp, true);
		for (int i = 0; i < record_size(); ++i)
		{
			rdb_record_osl(*record(i), OS);
		}
	}

	bool rdb_table::t_append_record(rdb_record & r)
	{
		String filename = 
			rdb_manager::Instance()->floder + "/" + i_dbname.c_str() + "/" + i_name.c_str() + "/0.table";

		FILE * fp = fopen(filename.c_str(), "ab");
		if (!fp)
		{
			d_log2->error("file '%s' open failed", filename.c_str());
			return false;
		}

		OSerializerF OS(fp, true);
		rdb_record_osl(r, OS);

		return true;
	}

	rdb_record * rdb_table::alloc_record()
	{
		rdb_record * p = i_alloc.Alloc();

		p->init();
	
		return p;
	}

	void rdb_table::free_record(rdb_record * p)
	{
		i_alloc.Free(p);
	}

	int rdb_table::record_compare(const rdb_record & r1, const rdb_record & r2)
	{
		d_assert (i_primaryks.Size() > 0);

		int ret = rsql_property::compare(i_primaryks[0], r1.object, r2.object);
		if (ret < 0)
			return -1;
		else if (ret > 0)
			return 1;

		if (i_primaryks.Size() > 1)
		{
			ret = rsql_property::compare(i_primaryks[1], r1.object, r2.object);
			if (ret < 0)
				return -1;
			else if (ret > 0)
				return 1;

			if (i_primaryks.Size() > 2)
			{
				ret = rsql_property::compare(i_primaryks[2], r1.object, r2.object);
				if (ret < 0)
					return -1;
				else if (ret > 0)
					return 1;

				if (i_primaryks.Size() > 3)
				{
					ret = rsql_property::compare(i_primaryks[3], r1.object, r2.object);
					if (ret < 0)
						return -1;
					else if (ret > 0)
						return 1;
				}
			}
		}

		return 0;
	}

	FixedString8 rdb_blockName(int id)
	{
		const int MAX_FILE_COUNT = 16 * 16 * 16 * 16;

		d_assert (id < MAX_FILE_COUNT);

		static char IdStr[] = {
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'A', 'B', 'C', 'D', 'E', 'F'
		};

		FixedString8 uName = "TD_0000";

		int i = 0;
		while (i < 4)
		{
			int index = id & 0x0F;

			id = id >> 4;

			uName[6 - i] = IdStr[index];
			i++;
		}

		return uName;
	}

	byte * rdb_table::b_load_cache(int id)
	{
		int i = i_blockcache.Find(id);
		if (i != -1)
		{
			return i_blockcache[i].value;
		}

		FixedString8 blockname = rdb_blockName(id);
		String filename = 
			rdb_manager::Instance()->floder + "/" + i_dbname.c_str() + "/" + i_name.c_str() + "/" + blockname.c_str();

		FILE_HANDLE fp;
		fp.handle = fopen(filename.c_str(), "rb");
		if (!fp.handle)
		{
			rdb_setError("file open failed, '%s'", blockname.c_str());
			return NULL;
		}

		int size = ftell_size(fp.handle);
		if (size == 0)
			return NULL;

		byte * data = new byte[size];

		fread(data, 1, size, fp.handle);

		i_blockcache.Insert(id, data);

		return data;
	}

	void rdb_table::b_free_cache(int id)
	{
		int i = i_blockcache.Find(id);
		if (i != -1)
		{
			delete i_blockcache[i].value;
			i_blockcache.Erase(i);
		}
	}

	void rdb_table::b_free_cache_all()
	{
		for (int i = 0; i < i_blockcache.Size(); ++i)
		{
			delete i_blockcache[i].value;
		}

		i_blockcache.Clear();
	}

	void rdb_table::b_save_block(int id)
	{
		FixedString8 blockname = rdb_blockName(id);
		String filename = 
			rdb_manager::Instance()->floder + "/" + i_dbname.c_str() + "/" + i_name.c_str() + "/" + blockname.c_str();

		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
		{
			d_log2->error("file '%s' open failed", filename.c_str());
			return ;
		}

		OSerializerF OS(fp, true);
		OSerializerTester OT;

		int offset = 0;
		for (int i = 0; i < record_size(); ++i)
		{
			rdb_record & r = *record(i);

			if (r.object != NULL && r.i_blockid == id)
			{
				OT.Clear();
				r.object->Serialize(OT);

				r.i_offset = offset;
				r.i_size = OT.Size();
				
				OS << r.object->GetRTTI()->TypeId();
				r.object->Serialize(OS);

				offset += 4 + OT.Size();
			}
		}

		b_free_cache(id);
	}

	bool rdb_table::b_load_record(rdb_record & r)
	{
		d_assert (r.object == NULL);

		byte * data = b_load_cache(r.i_blockid);
		if (data == NULL)
			return false;

		ISerializerM IS(data + r.i_offset, r.i_size, false);

		int typeId;
		IS >> typeId;

		r.object = Root::NEW_OBJECT(typeId);
		if (r.object != NULL)
		{
			r.object->Serialize(IS);
			return true;
		}
		else
		{
			rdb_setError("object load '%d' error!", typeId);
			return false;
		}
	}

	bool rdb_table::b_append_record(rdb_record & r)
	{
		FixedString8 blockname = rdb_blockName(r.i_blockid);
		String filename = 
			rdb_manager::Instance()->floder + "/" + i_dbname.c_str() + "/" + i_name.c_str() + "/" + blockname.c_str();

		FILE * fp = fopen(filename.c_str(), "ab");
		if (fp == NULL)
			fp = fopen(filename.c_str(), "wb");

		if (!fp)
		{
			d_log2->error("file '%s' open failed", filename.c_str());
			return false;
		}

		OSerializerF OS(fp, true);
		OSerializerTester OT;

		r.object->Serialize(OT);

		r.i_offset = ftell_size(fp);
		r.i_size = OT.Size();

		OS << r.object->GetRTTI()->TypeId();
		r.object->Serialize(OS);

		b_free_cache(r.i_blockid);

		return true;
	}

	bool rdb_table::b_update_record(rdb_record & r)
	{
		OSerializerTester OT;
		r.object->Serialize(OT);

		if (OT.Size() <= r.i_size)
		{
			r.i_size = OT.Size();

			FixedString8 blockname = rdb_blockName(r.i_blockid);
			String filename = 
				rdb_manager::Instance()->floder + "/" + i_dbname.c_str() + "/" + i_name.c_str() + "/" + blockname.c_str();

			FILE * fp = fopen(filename.c_str(), "rb+");
			if (!fp)
			{
				d_log2->error("file '%s' open failed", filename.c_str());
				return false;
			}

			fseek(fp, r.i_offset, SEEK_CUR);

			OSerializerF OS(fp, true);
			OS << r.object->GetRTTI()->TypeId();
			r.object->Serialize(OS);
		}
		else
		{
			b_save_block(r.i_blockid);
		}

		return true;
	}

	int rdb_table::b_num_records(int blockid)
	{
		int num = 0;
		for (int i = 0; i < record_size(); ++i)
		{
			if (record(i)->i_blockid == blockid)
			{
				num += 1;
			}
		}

		return num;
	}

	int rdb_table::b_block_size(int blockid)
	{
		int size = 0;
		for (int i = 0; i < record_size(); ++i)
		{
			const rdb_record & r = *record(i);

			if (r.i_blockid == blockid)
			{
				int position = r.i_offset + r.i_size;
				if (size < position)
				{
					size = position;
				}
			}
		}

		return size;
	}

	int rdb_table::b_query_block()
	{
		for (int i = i_queryblock; i < 0xFFFF; ++i)
		{
			if (b_num_records(i) < RDB_BLOCK_RECORDS)
			{
				i_queryblock = i;
				return i;
			}
		}

		for (int i = 0; i < i_queryblock; ++i)
		{
			if (b_num_records(i) < RDB_BLOCK_RECORDS)
			{
				i_queryblock = i;
				return i;
			}
		}

		return -1;
	}

	struct rdb_record_cmp
	{
		rdb_table * table;
		
		rdb_record_cmp(rdb_table * p) { table = p; }

		int operator()(const rdb_record * r1, const rdb_record * r2) const
		{
			return table->record_compare(*r1, *r2);
		}
	};

	struct rdb_record_sorter
	{
		rdb_table * table;

		rdb_record_sorter(rdb_table * p) { table = p; }

		bool operator()(const rdb_record * r1, const rdb_record * r2)
		{
			return table->record_compare(*r1, *r2) <= 0;
		}
	};

	bool rdb_table::build()
	{
		FixedArray<const Property *, 4> primaryks;

		int propSize = i_object->GetPropertySize();
		for (int i = 0; i < propSize; ++i)
		{
			const Property * p = i_object->GetProperty(i);
			if (p->flag & PF_Primary)
			{
				if (p->type == PT_Int || p->type == PT_Float || p->type == PT_FixedString || p->type == PT_String)
				{
					if (primaryks.Size() < 4)
					{
						primaryks.PushBack(p);					
					}
					else
					{
						rdb_setError("Primary property too mush");
						return false;
					}
				}
				else
				{
					rdb_setError("Primary property type error");
					return false;
				}
			}
		}

		i_primaryks = primaryks;

		if (i_primaryks.Size() > 0 && i_btree.Size() > 0)
		{
			Array<rdb_record *> arr;
			arr.Alloc(i_btree.Size());

			rdb_record_cmp op(this);
			for (int i = 0; i < i_btree.Size(); ++i)
			{
				arr.InsertT(i_btree[i], op);
			}

			memcpy(i_btree.c_ptr(), arr.c_ptr(), i_btree.Size() * sizeof(rdb_record *));

#ifdef M_DEBUG
			for (int i = 0; i < i_btree.Size() - 1; ++i)
			{
				d_assert (record_compare(*i_btree[i], *i_btree[i + 1]) <= 0);
			}
#endif
		}

		return true;
	}

	void rdb_table::optimize()
	{
		d_log2->debug("table '%s' optimize", i_name.c_str());

		Array<int> blocks;
		blocks.PushBack(0);
		
		for (int i = 0; i < i_btree.Size(); ++i)
		{
			if (blocks.Back() < RDB_BLOCK_RECORDS)
			{
				i_btree[i]->i_blockid = blocks.Size() - 1;
				blocks[i_btree[i]->i_blockid] += 1;
			}
			else
			{
				blocks.PushBack(0);
				--i;
			}
		}

		for (int i = 0; i < blocks.Size(); ++i)
		{
			b_save_block(i);
		}

		save();
	}

	int rdb_table::check_unique(rdb_record & r)
	{
		int first = 0, last = record_size() - 1;

		while (first <= last)
		{
			int mid = (first + last) / 2;

			int cmp = record_compare(r, *record(mid));
			if (cmp > 0)
			{
				first = mid + 1;
			}
			else if (cmp < 0)
			{
				last = mid - 1;
			}
			else
			{
				return mid;
			}
		}

		return -1;
	}

	void rdb_table::insert(rdb_record & r)
	{
		b_append_record(r);

		rdb_record * p = alloc_record();
		*p = r;

		if (i_primaryks.Size() > 0)
		{
			rdb_record_cmp op(this);
			i_btree.InsertT(p, op);
		}
		else
		{
			i_btree.PushBack(p);
		}

		t_append_record(*p);
	}

	void rdb_table::update(int ri, rdb_record & r)
	{
		rdb_record * p = i_btree[ri];
		p->object->Copy(r.object);
		p->i_priority += 1;

		b_update_record(r);

		i_btree.Erase(ri);

		if (i_primaryks.Size() > 0)
		{
			rdb_record_cmp op(this);
			i_btree.InsertT(p, op);
		}
		else
		{
			i_btree.PushBack(p);
		}
	}

	void rdb_table::remove(int ri)
	{
		rdb_record * p = i_btree[ri];

		i_btree.Erase(ri);

		safe_delete(p->object);
		free_record(p);
	}

	void rdb_table::search(int & first, int & last, int i, const rdb_value & _min, const rdb_value & _max)
	{
		const Property * prop = i_primaryks[i];

		if (_min.Type() != rdb_value::TYPE_NULL)
		{
			int start = first, end = last;

			while (start < end)
			{
				int mid = (start + end) / 2;
				rdb_record * r = i_btree[mid];

				int cmp = rsql_property::compare(r->object, prop, _min);
				if (cmp >= 0)
				{
					end = mid;
				}
				else
				{
					start = mid + 1;
				}
			}

			first = start;
		}

		if (_max.Type() != rdb_value::TYPE_NULL)
		{
			int start = first, end = last;

			while (start < end)
			{
				int mid = (start + end + 1) / 2;
				rdb_record * r = i_btree[mid];

				int cmp = rsql_property::compare(r->object, prop, _max);
				if (cmp <= 0)
				{
					start = mid;
				}
				else
				{
					end = mid - 1;
				}
			}

			last = start;
		}

		if (_min.Type() != rdb_value::TYPE_NULL && first == last && first < i_btree.Size())
		{
			rdb_record * r = i_btree[first];

			int cmp = rsql_property::compare(r->object, prop, _min);
			if (cmp < 0)
			{
				++first;
			}
		}

		if (_max.Type() != rdb_value::TYPE_NULL && first == last && first < i_btree.Size())
		{
			rdb_record * r = i_btree[last];

			int cmp = rsql_property::compare(r->object, prop, _max);
			if (cmp > 0)
			{
				--last;
			}
		}
	}

}
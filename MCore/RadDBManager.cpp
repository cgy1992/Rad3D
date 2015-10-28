#include "RadDBManager.h"
#include "RadDB.h"

namespace Rad {

	void rdb_manager_serialize(rdb_manager * mgr, Serializer & sl)
	{
		const int K_MAGIC = MAKE_DWORD('R', 'D', 'B', '0');
		const int K_USER = 0x1001;
		const int K_TABLE = 0x2001;

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			OS << K_MAGIC;

			OS << K_USER;
			OS << mgr->users.Size();
			for (int i = 0; i < mgr->users.Size(); ++i)
			{
				OS << mgr->users[i]->name;
				OS << mgr->users[i]->password;
				OS << mgr->users[i]->mode;
			}

			for (int i = 0; i < mgr->tables.Size(); ++i)
			{
				OS << K_TABLE;
				OS << mgr->tables[i].value->i_dbname;
				OS << mgr->tables[i].value->i_name;
				OS << mgr->tables[i].value->i_classname;
			}

			OS << 0;
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int magic;
			IS >> magic;

			d_assert(magic == K_MAGIC);

			int ckId = 0;
			while (IS.Read(&ckId, sizeof(int)) && ckId != 0)
			{
				if (ckId == K_USER)
				{
					int count;
					IS >> count;

					for (int i = 0; i < count; ++i)
					{
						rdb_user * user = new rdb_user;
						IS >> user->name;
						IS >> user->password;
						IS >> user->mode;

						mgr->users.PushBack(user);
					}
				}
				else if (ckId == K_TABLE)
				{
					FixedString32 dbname;
					FixedString32 tablename;
					FixedString32 classname;
					
					IS >> dbname;
					IS >> tablename;
					IS >> classname;

					String fullname = dbname.c_str() + String("/") + tablename.c_str();

					int crc32 = Math::Crc32(fullname.c_str());
					int i = mgr->tables.Find(crc32);
					if (i != -1)
					{
						d_log2->error(
							"table name conflict, '%s' - '%s'", tablename.c_str(), mgr->tables[i].value->i_name.c_str());
						return ;
					}

					rdb_table * table = new rdb_table;
					table->i_name = tablename;
					table->i_dbname = dbname;
					table->i_classname = classname;
					table->i_object = Root::NEW_OBJECT(classname.c_str());

					mgr->tables.Insert(crc32, table);

					if (table->i_object == NULL)
					{
						d_log2->error("table class '%s' unregister.", classname.c_str());
					}
				}
				else
				{
					d_log2->error("rdb data error.");
					break;
				}
			}
		}
	}

	ImplementSingleton(rdb_manager);

	void rdb_manager::load()
	{
		d_log2->print("load rdb data...");
		
		String filename = floder + "/rdb.data";

		FILE * fp = fopen(filename.c_str(), "rb");
		if (!fp)
		{
			d_log2->error("file open failed");
			return ;
		}

		ISerializerF IS(fp, true);
		rdb_manager_serialize(this, IS);
	}

	void rdb_manager::save()
	{
		d_log2->print("save rdb data...");

		String filename = floder + "/rdb.data";

		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
		{
			d_log2->error("file open failed");
			return ;
		}

		OSerializerF OS(fp, true);
		rdb_manager_serialize(this, OS);
	}

}
#include "RadDB.h"
#include "RadDBSql.h"
#include "RadDBManager.h"

namespace Rad {

	//
	void rdb_setError(const char * fmt, ...)
	{
		va_list arglist;

		va_start(arglist, fmt);
		vsprintf(rdb_manager::Instance()->error, fmt, arglist);
		va_end(arglist);
	}

	const char * rdb_getError()
	{
		return rdb_manager::Instance()->error;
	}

	//
	bool rdb_init(const char * dbfloder, const char * log, bool console)
	{
		new rdb_manager;

		String logfile = log + String("/log_") + Log2::GetDateString() + ".txt";
		rdb_manager::Instance()->log = new Log2(logfile.c_str());
		rdb_manager::Instance()->log->SetConsoleEnable(console);

		rdb_manager::Instance()->error[0] = 0;
		rdb_manager::Instance()->floder = dbfloder;

		new rsql_engine;

		return rdb_manager::Instance()->error[0] == 0;
	}

	void rdb_shutdown()
	{
		delete rsql_engine::Instance();

		for (int i = 0; i < rdb_manager::Instance()->tables.Size(); ++i)
		{
			delete rdb_manager::Instance()->tables[i].value;
		}
		rdb_manager::Instance()->tables.Clear();

		for (int i = 0; i < rdb_manager::Instance()->users.Size(); ++i)
		{
			delete rdb_manager::Instance()->users[i];
		}
		rdb_manager::Instance()->users.Clear();

		safe_delete(rdb_manager::Instance()->log);

		delete rdb_manager::Instance();
	}

	bool rdb_load()
	{
		rdb_manager::Instance()->load();

		for (int i = 0; i < rdb_manager::Instance()->tables.Size(); ++i)
		{
			rdb_table * table = rdb_manager::Instance()->tables[i].value;
			if (!table->load() || !table->build())
			{
				d_log2->error("table '%s' load error.", table->i_name.c_str());
				return false;
			}
		}

		return true;
	}

	//
	int rdb_createUser(const char * username, const char * passworld, int mode)
	{
		for (int i = 0; i < rdb_manager::Instance()->users.Size(); ++i)
		{
			if (stricmp(rdb_manager::Instance()->users[i]->name.c_str(), username) == 0)
			{
				rdb_setError("user exist.");
				return -1;
			}
		}

		if (strlen(username) > RDB_NAME_SIZE)
		{
			rdb_setError("username to long.");
			return -1;
		}

		if (strlen(passworld) > RDB_NAME_SIZE)
		{
			rdb_setError("passworld to long.");
			return -1;
		}

		rdb_user * user = new rdb_user;
		user->name = username;
		user->mode = mode;

		Math::Md5(user->password.c_str(), passworld);

		rdb_manager::Instance()->users.PushBack(user);

		rdb_manager::Instance()->save();

		return 0;
	}

	int rdb_deleteUser(const char * username)
	{
		for (int i = 0; i < rdb_manager::Instance()->users.Size(); ++i)
		{
			if (stricmp(rdb_manager::Instance()->users[i]->name.c_str(), username) == 0)
			{
				delete rdb_manager::Instance()->users[i];
				rdb_manager::Instance()->users.Erase(i);

				rdb_manager::Instance()->save();

				return -1;
			}
		}

		rdb_setError("user '%s' not exist.", username);

		return false;
	}

	int rdb_alterUser(const char * username, const char * passworld, int mode)
	{
		if (strlen(username) > RDB_NAME_SIZE)
		{
			rdb_setError("username to long.");
			return -1;
		}

		if (strlen(passworld) > RDB_NAME_SIZE)
		{
			rdb_setError("passworld to long.");
			return -1;
		}

		for (int i = 0; i < rdb_manager::Instance()->users.Size(); ++i)
		{
			rdb_user * user = rdb_manager::Instance()->users[i];

			if (stricmp(user->name.c_str(), username) == 0)
			{
				if (*passworld != 0)
				{
					Math::Md5(user->password.c_str(), passworld);
				}

				user->mode = mode;

				rdb_manager::Instance()->save();

				return 0;
			}
		}

		rdb_setError("user '%s' not exist.", username);

		return -1;
	}

	int rdb_verifyUser(const char * username, const char * passworld)
	{
		for (int i = 0; i < rdb_manager::Instance()->users.Size(); ++i)
		{
			rdb_user * user = rdb_manager::Instance()->users[i];

			if (user->name == username)
			{
				char temp[32];
				Math::Md5(temp, passworld);

				return memcmp(user->password.c_str(), temp, 16) == 0 ? user->mode : -1;
			}
		}

		return -1;
	}

	int rdb_getUserMode(const char * username)
	{
		for (int i = 0; i < rdb_manager::Instance()->users.Size(); ++i)
		{
			rdb_user * user = rdb_manager::Instance()->users[i];
			if (user->name == username)
			{
				return user->mode;
			}
		}

		return 0;
	}

	//
	int rdb_createTable(const char * db, const char * name, const char * classname, int mode)
	{
		if (strlen(db) > RDB_NAME_SIZE)
		{
			rdb_setError("dbname to long.");
			return -1;
		}

		if (strlen(name) > RDB_NAME_SIZE)
		{
			rdb_setError("tablename to long.");
			return -1;
		}

		if (strlen(classname) > 32)
		{
			rdb_setError("classname to long.");
			return -1;
		}

		String fullname = db;
		fullname += "/";
		fullname += name;

		int crc32 = Math::Crc32(fullname.c_str());
		int i = rdb_manager::Instance()->tables.Find(crc32);
		if (i != -1)
		{
			rdb_setError("table exist.");
			return -1;
		}

		IObject * object = Root::Instance()->NEW_OBJECT(classname);
		if (object == NULL)
		{
			rdb_setError("class not exist.");
			return -1;
		}

		if (!FileHelper::Exist(rdb_manager::Instance()->floder + "/" + db))
		{
			FileHelper::NewDir(rdb_manager::Instance()->floder + "/" + db);
		}

		if (!FileHelper::NewDir(rdb_manager::Instance()->floder + "/" + fullname))
		{
			rdb_setError("create dir failed.");
			return -1;
		}

		rdb_table * table = new rdb_table;
		table->i_name = name;
		table->i_dbname = db;
		table->i_classname = classname;
		table->i_mode = mode;
		table->i_object = object;

		table->build();

		table->save();

		rdb_manager::Instance()->tables.Insert(crc32, table);
		rdb_manager::Instance()->save();

		return 0;
	}

	int rdb_deleteTable(const char * db, const char * name)
	{
		if (strlen(db) > RDB_NAME_SIZE)
		{
			rdb_setError("dbname to long.");
			return -1;
		}

		if (strlen(name) > RDB_NAME_SIZE)
		{
			rdb_setError("tablename to long.");
			return -1;
		}

		String fullname = db;
		fullname += "/";
		fullname += name;

		int crc32 = Math::Crc32(fullname.c_str());
		int i = rdb_manager::Instance()->tables.Find(crc32);
		if (i == -1)
		{
			rdb_setError("table not exist.");
			return -1;
		}

		if (!FileHelper::DelDir(rdb_manager::Instance()->floder + "/" + fullname))
		{
			rdb_setError("delete dir failed.");
			return -1;
		}

		delete rdb_manager::Instance()->tables[i].value;
		rdb_manager::Instance()->tables.Erase(i);

		return 0;
	}

	int rdb_optimizeTable(const char * db, const char * name)
	{
		if (strlen(db) > RDB_NAME_SIZE)
		{
			rdb_setError("dbname to long.");
			return -1;
		}

		if (strlen(name) > RDB_NAME_SIZE)
		{
			rdb_setError("tablename to long.");
			return -1;
		}

		String fullname = db;
		fullname += "/";
		fullname += name;

		int crc32 = Math::Crc32(fullname.c_str());
		int i = rdb_manager::Instance()->tables.Find(crc32);
		if (i == -1)
		{
			rdb_setError("table not exist.");
			return -1;
		}

		rdb_manager::Instance()->tables[i].value->optimize();

		return 0;
	}

	rdb_handle rdb_getTable(const char * dbtable)
	{
		int crc32 = Math::Crc32(dbtable);
		int i = rdb_manager::Instance()->tables.Find(crc32);

		return i != -1 ? (rdb_handle)rdb_manager::Instance()->tables[i].value : NULL;
	}

	void rdb_regFunc(const char * name, rdb_function pfn)
	{
		rsql_engine::Instance()->reg_function(name, pfn);
	}
}
	
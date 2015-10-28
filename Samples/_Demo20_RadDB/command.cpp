#include "command.h"

void OnQuery(rdb_handle handle, char * args)
{
	rdb_obj_test obj;
	obj.data0 = 0;
	obj.data1 = 1;
	obj.data2 = "2";

	char * script = args;

	rdb_collection collection;
	int ret = rdbc_query(handle, &collection, script, &obj);
	if (ret >= 0)
	{
		printf("%d results.\n", ret);

		for (int i = 0; i < collection.Size(); ++i)
		{
			rdb_obj_test * obj = (rdb_obj_test *)collection[i];

			printf("%d, %f, %s\n", obj->data0, obj->data1, obj->data2.c_str());

			delete obj;
		}
	}
	else
	{
		printf("X: %s", rdbc_getError(handle));
	}
}

void OnBatchInsert(rdb_handle handle, char * args)
{
	rdb_obj_test obj;
	obj.data0 = 0;
	obj.data1 = 1;
	obj.data2 = "2";

	char * str = args;
	args = _getcmd(str);

	int insert = 0;
	int count = atoi(str);
	for (int i = 0; i < count; ++i)
	{
		const char * sql = "INSERT db001/table001";

		obj.data0 = i;
		obj.data1 = i + 1.0f;
		obj.data2.Format("%d", i + 2);

		int ret = rdbc_query(handle, NULL, sql, &obj);
		if (ret == -1)
		{
			printf("X: %s\n", rdbc_getError(handle));
		}
		else
		{
			insert += 1;
		}
	}

	printf("%d inserted.\n", insert);
}
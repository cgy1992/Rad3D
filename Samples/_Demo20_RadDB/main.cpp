#include "MSocket.h"
#include "command.h"

using namespace Rad;

Mutex gMutex;

class CommnadThread : public Thread
{
	rdb_handle handle;

public:
	CommnadThread(rdb_handle h) { handle = h; }
	virtual ~CommnadThread() {}

	virtual void Run()
	{
		Map<FixedString32, function> fmap;

		fmap.Insert("Query", OnQuery);
		fmap.Insert("BatchInsert", OnBatchInsert);

		while (1)
		{
			char cmd[256];
			gets(cmd);

			char * args = _getcmd(cmd);
			if (strcmp(cmd, "Quit") == 0)
			{
				break;
			}
			else
			{
				int i = fmap.Find(cmd);
				if (i != -1)
				{
					gMutex.Lock();

					fmap[i].value(handle, args);

					gMutex.Unlock();
				}
				else if (*cmd != 0)
				{
					printf("X: cmd '%s' error.", cmd);
				}
			}

			printf("\n\n");
		}

		mStatus = STOP;
	}
};

int main()
{
	printf("RadDB Client (Object Data Base).\n");
	printf("\n\n");

	Root * root = new Root;

	//
	Socket::Init();

	rdbc_init();

	rdb_handle handle = rdbc_open("127.0.0.1", 8086, "dbmaster", "123456");
	if (handle != NULL)
	{
		Thread * p = new CommnadThread(handle);

		while (p->GetStatus() != Thread::STOP)
		{
			gMutex.Lock();
			rdbc_heartbeat(handle);
			gMutex.Unlock();

			Thread::Sleep(1);

		}

		delete p;
	}

	if (handle != NULL)
		rdbc_close(handle, 0);

	rdbc_shutdown();

	Socket::Shutdown();

	delete root;

	return 0;
}


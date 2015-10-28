#include "RadDBC.h"
#include "RadDBClient.h"
#include "RadDBServer.h"
#include "RadDBSql.h"
#include "RadDBManager.h"
#include "MCodeTimer.h"

namespace Rad {

#define ON_PACKET(classname, fn) \
	if (TYPE_OF(classname, pk)) return fn(cl, (classname *)pk)

	struct rdbc_client : public TcpClient
	{
		byte M;
		char U[16];
		char K[16];

		time_t lasttime;

		rdbc_client() { M = 0, U[0] = 0, K[0] = 0; }
		virtual ~rdbc_client() {}

		void Send(IObject * obj)
		{
			OSerializerT<RDBC_PACK_MAX_SIZE> OS;

			obj->Serialize(OS);

			void * data = OS.Data();
			int length = OS.Size();

			static_memory buffer2;
			if (length > 0)
			{
				buffer2.query(RDBC_STATIC_MEMORY_CHANEL, RDBC_PACK_MAX_SIZE);

				int ret = Math::RLE8_Encode((byte *)buffer2.data(), RDBC_PACK_MAX_SIZE, (byte *)OS.Data(), OS.Size());
				if (ret > 0 && ret < length)
				{
					data = buffer2.data();
					length = ret | 0x80000000;
				}
			}

			rdbc::pk_head head;
			head.id = obj->GetRTTI()->TypeId();
			head.length = length;

			if (M != 0)
			{
				rdbc::encrypt((char *)&head, sizeof(head), K, sizeof(K));
			}

			OStream()->Write(RDBC_PACK_COMPACT, RDBC_PACK_COMPACT_SIZE);
			OStream()->Write(&head, sizeof(head));
			OStream()->Write(data, length & 0x7FFFFFFF);
		}
	};

	struct rdbc_server : public TcpServer, public Singleton<rdbc_server>
	{
		virtual ~rdbc_server() {}

		int i_timeout;

		virtual void Select(int seconds, int microseconds)
		{
			TcpServer::Select(seconds, microseconds);

			time_t curtime = time(NULL);

			for (int i = 0; i < mClients.Size(); ++i)
			{
				rdbc_client * cl = (rdbc_client *)mClients[i];
				if (!cl->Valid())
				{
					delete cl;
					mClients.Erase(i--);
				}
				else
				{
					CL_Input((rdbc_client *)cl);

					if (cl->M != RDB_MODE_MONSTER && curtime - cl->lasttime > i_timeout)
					{
						d_log2->print("'%s' time out", cl->U);
						
						cl->Close(0);
					}
				}
			}
		}

		virtual TcpClient * OnAccept(SOCKET s, const sockaddr_in & addr)
		{
			rdbc_client * client = new rdbc_client;
			client->Open(s, addr);
			client->SetNonBlocking();
			client->SetLinger(0);
			client->lasttime = time(NULL);

			d_log2->print("'%s' connected", client->GetIp());

			mClients.PushBack(client);

			return client;
		}

	protected:
		void CL_Input(rdbc_client * cl)
		{
			while (cl->Valid())
			{
				// skip compact
				char compact[RDBC_PACK_COMPACT_SIZE + 1];
				if (cl->IStream()->Read(compact, RDBC_PACK_COMPACT_SIZE) == 0)
					break;

				compact[RDBC_PACK_COMPACT_SIZE] = 0;
				if (strcmp(compact, RDBC_PACK_COMPACT) != 0)
				{
					d_log("'%s' compact error", cl->U);
					cl->Close(0);
					break;
				}

				int skip = RDBC_PACK_COMPACT_SIZE;

				// read packet head
				rdbc::pk_head head;
				if (cl->IStream()->Read(&head, sizeof(head)) == 0)
				{
					cl->IStream()->Skip(-skip);
					break;
				}

				skip += sizeof(head);

				// encrypt head
				if (cl->M != 0)
				{
					rdbc::encrypt((char *)&head, sizeof(head), cl->K, sizeof(cl->K));
				}

				int length = head.length & 0x7FFFFFFF;
				if (length <= 0 || length > RDBC_PACK_MAX_SIZE)
				{
					d_log2->error("packet size invalid, id %d, size %d", head.id, head.length);
					cl->Close(0);
					break;
				}

				// read data
				char buffer[RDBC_PACK_MAX_SIZE];
				if (cl->IStream()->Read(buffer, length) == 0)
				{
					cl->IStream()->Skip(-skip);
					break;
				}

				char * data = buffer;
				static_memory buffer2;

				if (head.length & 0x80000000)
				{
					buffer2.query(RDBC_STATIC_MEMORY_CHANEL, RDBC_PACK_MAX_SIZE);

					length = Math::RLE8_Decode((byte *)buffer2.data(), RDBC_PACK_MAX_SIZE, (const byte *)buffer, length);
					if (length < 0)
					{
						d_log("packet uncompress error, '%d'", head.id);
						cl->Close(0);
						break;
					}

					data = (char *)buffer2.data();
				}

				ISerializerM IS((byte *)data, length, false);

				rdbc::packet * pk = Root::Instance()->NEW_OBJECT_T<rdbc::packet>(head.id);
				if (pk == NULL)
				{
					d_log2->error("packet unregister '%d'", head.id);
					cl->Close(0);
					break;
				}

				pk->Serialize(IS);

				buffer2.free();
				
				rdb_setError("");

				rdbc::pk_result opk;
				opk.ret = CL_Proccess(cl, pk);
				opk.error = rdb_getError();

				cl->Send(&opk);
				cl->lasttime = time(NULL);

				delete pk;
			}
		}

		int CL_Proccess(rdbc_client * cl, rdbc::packet * pk)
		{
			ON_PACKET(rdbc::pk_logon, OnLogon);
			ON_PACKET(rdbc::pk_query, OnQuery);

			return -1;
		}

		void CL_Kick(const char * username)
		{
			for (int i = 0; i < mClients.Size(); ++i)
			{
				rdbc_client * cl = (rdbc_client *)mClients[i];
				if (cl->Valid() && strcmp(cl->U, username) == 0)
				{
					d_log2->debug("Kick '%s'", cl->U);

					cl->Close(0);
					break;
				}
			}
		}

		int OnLogon(rdbc_client * cl, rdbc::pk_logon * pk)
		{
			if (cl->U[0] != 0)
				return -1;

			int M = rdb_verifyUser(pk->username.c_str(), pk->password.c_str());
			if (M > 0)
			{
				CL_Kick(pk->username.c_str());

				rdbc::pk_auth opk;
				opk.M = M;
				Math::Md5(opk.K, pk->password.c_str());
				opk.K[2] ^= Math::RandRange(0, 255);
				opk.K[4] ^= Math::RandRange(0, 255);
				opk.K[6] ^= Math::RandRange(0, 255);
				opk.K[8] ^= Math::RandRange(0, 255);

				cl->Send(&opk);
				
				cl->M = M;
				strcpy(cl->U, pk->username.c_str());
				memcpy(cl->K, opk.K, 16);
				
				d_log2->print("'%s' logon", cl->U);

				return 0;
			}
			else
			{
				d_log2->error("user verify failed, '%s'", pk->username.c_str());
			}

			return -1;
		}

		int OnQuery(rdbc_client * cl, rdbc::pk_query * pk)
		{
			if (cl->U[0] == 0)
				return -1;

			profile_code();

			rdb_collection collection;

			int ret = -1;

			rsql_engine::Instance()->begin();

			ret = rsql_engine::Instance()->compile(pk->rsql.c_str(), cl->M);
			if (ret >= 0)
			{
				ret = rsql_engine::Instance()->execute(pk->obj, !pk->nodata ? &collection : NULL);
				if (rsql_engine::Instance()->i_op == rsql_engine::RSQL_INSERT && ret == 1)
				{
					pk->obj = NULL;
				}
			}

			if (ret == -255)
			{
				d_log2->error("user access limit, '%s', '%s'", cl->U, pk->rsql.c_str());
				ret = -1;
			}

			rsql_engine::Instance()->end();

			if (ret > 0)
			{
				for (int i = 0; i < collection.Size(); ++i)
				{
					rdbc::pk_object opk;
					opk.obj = collection[i];

					cl->Send(&opk);
				}
			}

			safe_delete (pk->obj);

			if (rsql_engine::Instance()->is_error())
			{
				rdb_setError(rsql_engine::Instance()->i_error);
			}

			return ret;
		}
	};

	bool rdbc_server_init(int port, int timeout)
	{
		rdbc_init();

		rdbc_server * server = new rdbc_server;
		server->i_timeout = timeout;

		if (server->Bind(port))
		{
			d_log2->print("server start...");
			return true;
		}
		else
		{
			d_log2->error("server start failed.");
			return false;
		}
	}

	ImplementSingleton(rdbc_server);

	//
	void rdbc_server_shutdown()
	{
		rdbc_server::Instance()->Close();

		d_log2->print("server close...");

		delete rdbc_server::Instance();

		rdbc_shutdown();
	}

	void rdbc_server_select(int seconds, int microseconds)
	{
		rdbc_server::Instance()->Select(seconds, microseconds);
	}

}
#include "RadDBC.h"
#include "RadDBClient.h"

namespace Rad {

	struct rdbc_connection : public TcpClient
	{
		char err[256];

		int M;
		char K[16];

		rdbc_connection() { M = 0, K[0] = 0; }
		virtual ~rdbc_connection() {}

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

		rdbc::packet * GetResult()
		{
			while (Valid())
			{
				rdbc::pk_head head;

				// skip compact
				char compact[RDBC_PACK_COMPACT_SIZE + 1];
				if (IStream()->Read(compact, RDBC_PACK_COMPACT_SIZE) == 0)
					break;

				compact[RDBC_PACK_COMPACT_SIZE] = 0;
				if (strcmp(compact, RDBC_PACK_COMPACT) != 0)
				{
					d_assert(0);
					d_log("packet compact error");
					Close(0);
					break;
				}

				int skip = RDBC_PACK_COMPACT_SIZE;

				// read packet head
				if (IStream()->Read(&head, sizeof(head)) == 0)
				{
					IStream()->Skip(-skip);
					break;
				}

				skip += sizeof(head);

				// encrypt head
				if (M != 0)
				{
					rdbc::encrypt((char *)&head, sizeof(head), K, sizeof(K));
				}

				int length = head.length & 0x7FFFFFFF;
				if (length <= 0 || length > RDBC_PACK_MAX_SIZE)
				{
					d_log("packet size invalid, id %d, size %d", head.id, head.length);
					Close(0);
					break;
				}

				// read data
				char buffer[RDBC_PACK_MAX_SIZE];
				if (IStream()->Read(buffer, length) == 0)
				{
					IStream()->Skip(-skip);
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
						Close(0);
						break;
					}

					data = (char *)buffer2.data();
				}

				ISerializerM IS((byte *)data, length, false);

				rdbc::packet * pk = Root::Instance()->NEW_OBJECT_T<rdbc::packet>(head.id);
				if (pk == NULL)
				{
					d_log("packet unregister '%d'", head.id);
					Close(0);
					break;
				}

				pk->Serialize(IS);

				buffer2.free();

				return pk;
			}

			return NULL;
		}
	};

	const char * rdbc_getError(rdb_handle handle)
	{
		rdbc_connection * cl = (rdbc_connection *)handle;

		return cl->err;
	}

	void rdbc_init()
	{
		Root::REG_OBJECT_T<rdbc::pk_heartbeat>();
		Root::REG_OBJECT_T<rdbc::pk_logon>();
		Root::REG_OBJECT_T<rdbc::pk_auth>();
		Root::REG_OBJECT_T<rdbc::pk_query>();
		Root::REG_OBJECT_T<rdbc::pk_object>();
		Root::REG_OBJECT_T<rdbc::pk_result>();

		Root::REG_OBJECT_T<rdb_obj_test>();
	}

	void rdbc_shutdown()
	{
	}

	rdb_handle rdbc_open(const char * host, int port, const char * user, const char * passworld, int timeout)
	{
		rdbc_connection * cl = new rdbc_connection;
		cl->Open(host, port);
		cl->SetNonBlocking();
		cl->SetLinger(0);
		
		if (!cl->Connect(timeout))
		{
			cl->Close(0);
			delete cl;
			return NULL;
		}

		rdbc::pk_logon pk;
		pk.username = user;
		pk.password = passworld;

		cl->Send(&pk);

		bool hr = false;

		do
		{
			cl->Select(0, 5);

			rdbc::packet * i_pk = cl->GetResult();

			if (TYPE_OF(rdbc::pk_auth, i_pk))
			{
				rdbc::pk_auth * pk = (rdbc::pk_auth *)i_pk;
				if(cl->M == 0)
				{
					cl->M = pk->M;
					memcpy(cl->K, pk->K, 16);
				}
				else
				{
					cl->Close(0);
				}
			}
			else if (TYPE_OF(rdbc::pk_result, i_pk))
			{
				rdbc::pk_result * pk = (rdbc::pk_result *)i_pk;

				hr = pk->ret >= 0;

				safe_delete (i_pk);

				break;
			}

			safe_delete (i_pk);

		} while (cl->Valid());
		
		if (!hr)
		{
			cl->Close(0);
			delete cl;
			cl = NULL;
		}

		return (rdb_handle)cl;
	}

	void rdbc_close(rdb_handle handle, int waittime)
	{
		rdbc_connection * cl = (rdbc_connection *)handle;
		cl->Close(waittime);

		while (cl->Valid())
		{
			cl->Select(0, 5);
		}

		delete cl;
	}

	int rdbc_mode(rdb_handle handle)
	{
		rdbc_connection * cl = (rdbc_connection *)handle;

		return cl->M;
	}

	int _rdbc_waitresult(rdbc_connection * cl)
	{
		int ret = -1;

		do
		{
			cl->Select(0, 5);

			rdbc::packet * i_pk = cl->GetResult();

			if (TYPE_OF(rdbc::pk_result, i_pk))
			{
				rdbc::pk_result * pk = (rdbc::pk_result *)i_pk;

				ret = pk->ret;
				if (ret == -1)
				{
					strcpy(cl->err, pk->error.c_str());
				}

				safe_delete (i_pk);

				break;
			}

			safe_delete (i_pk);

		} while (cl->Valid());

		return ret;
	}

	int rdbc_heartbeat(rdb_handle handle)
	{
		rdbc_connection * cl = (rdbc_connection *)handle;

		rdbc::pk_heartbeat pk;
		cl->Send(&pk);

		return _rdbc_waitresult(cl);
	}
	
	int rdbc_query(rdb_handle handle, rdb_collection * collection, const char * rsql, IObject * obj)
	{
		rdbc_connection * cl = (rdbc_connection *)handle;

		rdbc::pk_query pk;
		pk.nodata = collection == NULL;
		pk.rsql = rsql;
		pk.obj = obj;

		cl->Send(&pk);

		int ret = -1;

		do
		{
			cl->Select(0, 5);

			rdbc::packet * i_pk = cl->GetResult();

			if (TYPE_OF(rdbc::pk_object, i_pk))
			{
				rdbc::pk_object * pk = (rdbc::pk_object *)i_pk;

				if (collection != NULL)
				{
					collection->PushBack(pk->obj);
				}
			}
			else if (TYPE_OF(rdbc::pk_result, i_pk))
			{
				rdbc::pk_result * pk = (rdbc::pk_result *)i_pk;

				ret = pk->ret;
				if (ret == -1)
				{
					strcpy(cl->err, pk->error.c_str());
				}

				safe_delete (i_pk);

				break;
			}

			safe_delete (i_pk);

		} while (cl->Valid());

		return ret;
	}

}
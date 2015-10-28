/*
*	RadDB Connection
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TcpClient.h"
#include "TcpServer.h"
#include "RadDB.h"

namespace Rad { namespace rdbc {

#define RDBC_PACK_COMPACT "$-$"
#define RDBC_PACK_COMPACT_SIZE 3
#define RDBC_PACK_MAX_SIZE 2048

	inline void encrypt(char * x, int xlen, const char * k, int klen, int kstep = 1)
	{
		for (int i = 0, j = 0; i < xlen; i++, j += kstep)
		{
			x[i] ^= k[j & (klen - 1)];
		}
	}

	struct pk_head
	{
		int id;
		int length;
	};

	struct packet : public IObject
	{
		DECLARE_RTTI();
		DECLARE_POOL_ALLOC();
	};

	struct pk_logon : public packet
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(packet);

		String username;
		String password;
	};

	struct pk_heartbeat : public packet
	{
		DECLARE_RTTI();
	};

	struct pk_auth : public packet
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(packet);

		byte M;
		char K[16];
	};

	struct pk_query : public packet
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(packet);

		byte nodata;
		String rsql;

		IObject * obj;

		virtual void 
			Serialize(Serializer & sl);
	};
	
	struct pk_object : public packet
	{
		DECLARE_RTTI();

		IObject * obj;

		virtual void 
			Serialize(Serializer & sl);
	};

	struct pk_result : public packet
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(packet);

		int ret;
		String error;
	};

}
}
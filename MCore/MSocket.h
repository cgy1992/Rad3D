/*
*	Socket
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSocketAPI.h"
#include "MArray.h"
#include "MThread.h"

namespace Rad {

	struct M_ENTRY Socket
	{
		sockaddr_in i_addr;
		SOCKET i_socket;

	public:
		Socket() {}
		virtual ~Socket() {}

		static void
			Init();
		static void
			Shutdown();

		static void 
			FD_Zero();
		static void 
			FD_Set(Socket * s, bool rd, bool wt, bool ex);
		static void
			FD_Get(Socket * s, bool & rd, bool & wt, bool & ex);
		static bool 
			FD_Select(int seconds, int microseconds);

		bool 
			Valid() { return i_socket != INVALID_SOCKET; }
	};

	class M_ENTRY SocketIStream
	{
	public:
		SocketIStream();
		~SocketIStream();

		int 
			Refresh(SOCKET sock);

		int 
			Read(void * buff, int size);
		int 
			Skip(int size);

		void 
			Clear() { i_buffer.Clear(); i_cursor = 0; }
		int 
			Size() const { return i_buffer.Size() - i_cursor; }

	protected:
		Array<char> i_buffer;
		int i_cursor;
	};

	class M_ENTRY SocketOStream
	{
	public:
		SocketOStream();
		~SocketOStream();

		int 
			Refresh(SOCKET sock);

		int 
			Write(const void * buff, int size);

		void 
			Clear() { i_buffer.Clear(); }
		int 
			Size() const { return i_buffer.Size(); }

	protected:
		Array<char> i_buffer;
	};

}
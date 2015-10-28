#include "MSocket.h"
#include "MMath.h"

namespace Rad {

	void Socket::Init()
	{
		socket_init();
	}

	void Socket::Shutdown()
	{
		socket_shutdown();
	}

	static fd_set s_rd, s_wt, s_ex;

	void Socket::FD_Zero()
	{
		FD_ZERO(&s_rd);
		FD_ZERO(&s_wt);
		FD_ZERO(&s_ex);
	}

	void Socket::FD_Set(Socket * s, bool rd, bool wt, bool ex)
	{
		d_assert (s->i_socket != INVALID_SOCKET);

		if (rd)
			FD_SET(s->i_socket, &s_rd);

		if (wt)
			FD_SET(s->i_socket, &s_wt);

		if (ex)
			FD_SET(s->i_socket, &s_ex);
	}

	void Socket::FD_Get(Socket * s, bool & rd, bool & wt, bool & ex)
	{
		d_assert (s->i_socket != INVALID_SOCKET);

		rd = FD_ISSET(s->i_socket, &s_rd) == TRUE;
		wt = FD_ISSET(s->i_socket, &s_wt) == TRUE;
		ex = FD_ISSET(s->i_socket, &s_ex) == TRUE;
	}

	bool Socket::FD_Select(int seconds, int microseconds)
	{
		timeval val;
		val.tv_sec = seconds;
		val.tv_usec = microseconds;

		return select_ex(0, &s_rd, &s_wt, &s_ex, &val) != SOCKET_ERROR;
	}

	//
	SocketIStream::SocketIStream()
	{
		i_buffer.Alloc(32 * 1024);
		i_cursor = 0;
	}

	SocketIStream::~SocketIStream()
	{
	}

	int SocketIStream::Refresh(SOCKET sock)
	{
		int size = 0;
		if (ioctlsocket_ex(sock, FIONREAD, &size) == SOCKET_ERROR)
			return -1;

		if (size == 0)
			return 0;

		int length = i_buffer.Size() - i_cursor;
		if (length > 0)
		{
			memcpy(&i_buffer[0], &i_buffer[i_cursor], length);
		}

		i_cursor = 0;
		i_buffer.Resize(length + size);

		int rcvs = recv_ex(sock, &i_buffer[length], size, 0);
		if (rcvs == -1)
			return -1;

		i_buffer.Resize(length + rcvs);

		return rcvs;
	}

	int SocketIStream::Read(void * buff, int size)
	{
		if (i_cursor + size > i_buffer.Size())
			return 0;

		char * src = &i_buffer[i_cursor];

		memcpy(buff, src, size);

		i_cursor += size;

		return size;
	}

	int SocketIStream::Skip(int size)
	{
		int newcur = i_cursor + size;

		d_assert (newcur >= 0 && newcur <= i_buffer.Size());

		int sk = newcur - i_cursor;

		i_cursor = newcur;

		return sk;
	}


	//
	SocketOStream::SocketOStream()
	{
		i_buffer.Alloc(32 * 1024);
	}

	SocketOStream::~SocketOStream()
	{
	}

	int SocketOStream::Refresh(SOCKET sock)
	{
		const int K_MTU = 1500 - 100;

		int length = i_buffer.Size();
		if (length > 0)
		{
			int snds = 0;
			if (length > K_MTU)
				snds = send_ex(sock, &i_buffer[0], K_MTU, 0);
			else
				snds = send_ex(sock, &i_buffer[0], length, 0);
			
			if (snds == SOCKET_ERROR)
				return -1;

			if (snds > 0)
			{
				length -= snds;
				if (length > 0)
				{
					memcpy(&i_buffer[0], &i_buffer[snds], length);
				}

				i_buffer.Resize(length);
			}

			return snds;
		}
		
		return 0;
	}

	int SocketOStream::Write(const void * buff, int size)
	{
		int length = i_buffer.Size();

		i_buffer.Resize(length + size);
		memcpy(&i_buffer[length], buff, size);

		return size;
	}

}
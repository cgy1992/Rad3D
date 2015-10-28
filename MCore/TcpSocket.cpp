#include "TcpSocket.h"

namespace Rad {

	TcpSocket::TcpSocket()
	{
		i_socket = INVALID_SOCKET;
		i_nonblocking = false;
	}

	TcpSocket::~TcpSocket()
	{
		d_assert(!Valid());
	}

	void TcpSocket::SetReuseAddr(bool enable)
	{
		d_assert (Valid());

		setsockopt_ex(i_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(bool));
	}

	void TcpSocket::SetKeepActive(bool enable)
	{
		d_assert (Valid());

		setsockopt_ex(i_socket, SOL_SOCKET, SO_KEEPALIVE, (const char*)&enable, sizeof(bool));
	}

	void TcpSocket::SetSendBufferSize(int size)
	{
		d_assert (Valid());

		setsockopt_ex(i_socket, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(int));
	}

	void TcpSocket::SetRecvBufferSize(int size)
	{
		d_assert (Valid());

		setsockopt_ex(i_socket, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(int));
	}

	void TcpSocket::SetNonBlocking()
	{
		d_assert (Valid());

		nonblocking_ex(i_socket);

		i_nonblocking = true;
	}

	void TcpSocket::SetLinger(int time)
	{
		d_assert (Valid());

		linger l;
		l.l_onoff = time > 0 ? 1 : 0;
		l.l_linger = time;

		setsockopt_ex(i_socket, SOL_SOCKET, SO_LINGER, (const char*)&l, sizeof(linger));
	}

	const char * TcpSocket::GetIp()
	{
		d_assert (Valid());

		return socket_ip(&i_addr);
	}

	void TcpSocket::Refresh()
	{
		if (!Valid())
			return ;

		bool rd = false, wt = false, ex = false;
		
		Socket::FD_Get(this, rd, wt, ex);

		if (Valid() && rd)
		{
			OnRead();
		}

		if (Valid() && wt)
		{
			OnWrite();
		}

		if (Valid() && ex)
		{
			OnExpect();
		}
	}

}

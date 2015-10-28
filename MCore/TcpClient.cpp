#include "TcpClient.h"

namespace Rad {

	TcpClient::TcpClient()
	{
		mConnectingTimer = false;
		mClosingTimer = 0;
		mTime = 0;
	}

	TcpClient::~TcpClient()
	{
	}

	bool TcpClient::Open(const char * host, int port)
	{
		d_assert (i_socket == INVALID_SOCKET);

		i_socket = socket_ex(AF_INET, SOCK_STREAM, 0);

		i_addr.sin_family = AF_INET;
		i_addr.sin_addr.s_addr = inet_addr(host);
		i_addr.sin_port = htons(port);

		mTime = time(NULL);

		return i_socket != INVALID_SOCKET;
	}

	bool TcpClient::Open(SOCKET s, const sockaddr_in & addr)
	{
		d_assert (i_socket == INVALID_SOCKET);

		i_socket = s;
		i_addr = addr;

		mTime = time(NULL);

		return i_socket != INVALID_SOCKET;
	}

	void TcpClient::Close(int time)
	{
		if (i_socket != INVALID_SOCKET)
		{
			if (time <= 0)
			{
				closesocket_ex(i_socket);
				i_socket = INVALID_SOCKET;

				mConnectingTimer = 0;
				mClosingTimer = 0;

				OnClose();

				mIStream.Clear();
				mOStream.Clear();
			}
			else
			{
				mClosingTimer = time;
			}
		}
	}

	bool TcpClient::Connect(int timeout)
	{
		if (!IsConnecting())
		{
			mIStream.Clear();
			mOStream.Clear();

			mTime = time(NULL);

			if (i_nonblocking)
			{
				connect_ex(i_socket, (sockaddr*)&i_addr, sizeof(i_addr));
				mConnectingTimer = timeout;
			}
			else
			{
				if (connect_ex(i_socket, (sockaddr*)&i_addr, sizeof(i_addr)) == SOCKET_ERROR)
				{
					Close(0);
				}
				else
				{
					OnConnect();
				}
			}
		}

		return Valid();
	}

	bool TcpClient::CheckConnect()
	{
		int err;
		int errlen = sizeof(err);
		getsockopt_ex(i_socket, SOL_SOCKET, SO_ERROR, &err, &errlen);

		return err == 0;
	}
	
	void TcpClient::Select(int seconds, int microseconds)
	{
		if (Valid())
		{
			Socket::FD_Zero();
			Socket::FD_Set(this, true, true, true);
			Socket::FD_Select(seconds, microseconds);

			Refresh();
		}
	}

	void TcpClient::Refresh()
	{
		time_t current = time(NULL);
		time_t interval = current - mTime;
		mTime = current;

		if (Valid() && IsClosing())
		{
			mClosingTimer -= interval;
			if (mClosingTimer <= 0 || mOStream.Size() == 0)
			{
				Close(0);
			}
		}

		if (IsConnecting())
		{
			mConnectingTimer -= interval;
			if (mConnectingTimer <= 0)
			{
				Close(0);
			}
		}

		TcpSocket::Refresh();
	}

	void TcpClient::OnRead()
	{
		if (mIStream.Refresh(i_socket) == -1)
		{
			Close(0);
		}
	}

	void TcpClient::OnWrite()
	{
		if (IsConnecting())
		{
			if (CheckConnect())
			{
				OnConnect();
				mConnectingTimer = 0;
			}
			else
			{
				Close(0);
			}
		}
		else
		{
			if (Valid() && mOStream.Refresh(i_socket) == -1)
			{
				Close(0);
			}
		}
	}

}
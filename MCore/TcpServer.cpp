#include "TcpServer.h"

namespace Rad {

	TcpServer::TcpServer()
	{
	}

	TcpServer::~TcpServer()
	{
	}

	bool TcpServer::Bind(int port, int depth)
	{
		d_assert (i_socket == INVALID_SOCKET);

		i_socket = socket_ex(AF_INET, SOCK_STREAM, 0);

		if (i_socket != INVALID_SOCKET)
		{
			SetNonBlocking();

			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			addr.sin_port = htons(port);

			int ret = bind_ex(i_socket, (sockaddr*)&addr, sizeof(addr));
			if (ret != SOCKET_ERROR)
			{
				ret = listen_ex(i_socket, depth);
			}

			if (ret == SOCKET_ERROR)
			{
				Close();
			}
		}

		return i_socket != INVALID_SOCKET;
	}

	void TcpServer::Close()
	{
		for (int i = 0; i < mClients.Size(); ++i)
		{
			mClients[i]->Close(0);
			delete mClients[i];
		}
		mClients.Clear();

		if (i_socket != INVALID_SOCKET)
		{
			closesocket_ex(i_socket);
			i_socket = INVALID_SOCKET;
		}
	}

	void TcpServer::Select(int seconds, int microseconds)
	{
		if (i_socket != INVALID_SOCKET)
		{
			Socket::FD_Zero();
			Socket::FD_Set(this, true, true, true);
			Socket::FD_Select(0, 2);

			Refresh();
		}

		int i = 0, count = 0;
		while (i < mClients.Size())
		{
			while (i < mClients.Size() && count < FD_SETSIZE)
			{
				if (mClients[i]->Valid())
				{
					Socket::FD_Set(mClients[i], true, true, true);
				}

				++i;
				++count;
			}

			Socket::FD_Select(seconds, microseconds);

			for (int j = i - count; j < i; ++j)
			{
				mClients[j]->Refresh();
			}

			count = 0;
		}
	}

	void TcpServer::OnRead()
	{
		sockaddr_in addr;
		int len = sizeof(addr);

		SOCKET s = accept_ex(i_socket, (sockaddr*)&addr, &len);
		if (s != INVALID_SOCKET)
		{
			OnAccept(s, addr);
		}
	}

	TcpClient * TcpServer::OnAccept(SOCKET s, const sockaddr_in & addr)
	{
		TcpClient * client = new TcpClient;
		client->Open(s, addr);

		mClients.PushBack(client);

		return client;
	}

	void TcpServer::RemoveClient(TcpClient * client)
	{
		for (int i = 0; i < mClients.Size(); ++i)
		{
			if (mClients[i] == client)
			{
				mClients.Erase(i);
				break;
			}
		}

		client->Close(0);
		delete client;
	}
}

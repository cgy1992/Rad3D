#pragma once

#include "TcpClient.h"
#include "TcpServer.h"
#include "MRoot.h"

using namespace Rad;

class MyServer : public TcpServer
{
public:
	virtual ~MyServer() {}

	virtual TcpClient *
		OnAccept(SOCKET s, const sockaddr_in & addr)
	{
		TcpClient * client = TcpServer::OnAccept(s, addr);
		client->SetNonBlocking();

		printf("Server: Accept '%s'\n", client->GetIp());

		return client;
	}
};

class MyClient : public TcpClient
{
public:
	virtual ~MyClient() {}

	virtual void OnConnect()
	{
		printf("Client: connected\n");
	}

protected:
};

int main()
{
	Socket::Init();

	const char * c2s_str = "Hello Server!\n";
	const char * s2c_str = "Hello Client!\n";

	MyServer server;
	if (server.Bind(80886))
	{
		printf("server start...\n");
	}

	MyClient client;
	client.Open("127.0.0.1", 80886);
	client.SetNonBlocking();

	client.Connect();
	client.OStream()->Write(c2s_str, strlen(c2s_str));

	while (1)
	{
		//
		server.Select(0, 5);
		for (int i = 0; i < server.GetClientSize(); ++i)
		{
			TcpClient * cl = server.GetClient(i);
			
			int length = cl->IStream()->Size();
			if (length > 0)
			{
				char buffer[256];

				bool r = false;
				int n = 0;
				while (n < length)
				{
					char ch;
					cl->IStream()->Read(&ch, 1);

					++n;

					if (ch == '\n')
					{
						r = true;
						break;
					}
				}

				cl->IStream()->Skip(-n);

				if (r)
				{
					cl->IStream()->Read(buffer, n);
					buffer[n] = 0;

					printf("Server Recv: %s", buffer);

					cl->OStream()->Write(s2c_str, strlen(s2c_str));
				}
			}
		}

		//
		client.Select(0, 5);
		{
			TcpClient * cl = &client;

			int length = cl->IStream()->Size();
			if (length > 0)
			{
				char buffer[256];

				bool r = false;
				int n = 0;
				while (n < length)
				{
					char ch;
					cl->IStream()->Read(&ch, 1);

					++n;

					if (ch == '\n')
					{
						r = true;
						break;
					}
				}

				cl->IStream()->Skip(-n);

				if (r)
				{
					cl->IStream()->Read(buffer, n);
					buffer[n] = 0;

					printf("Client Recv: %s", buffer);

					cl->OStream()->Write(c2s_str, strlen(c2s_str));
				}
			}
		}

		Thread::Sleep(2000);
	}

	client.Close(0);
	server.Close();

	Socket::Shutdown();

	getchar();

	return 0;
}
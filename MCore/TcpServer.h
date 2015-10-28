/*
*	TCP Server
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TcpClient.h"

namespace Rad {

	struct M_ENTRY TcpServer : public TcpSocket
	{
	public:
		TcpServer();
		virtual ~TcpServer();

		bool
			Bind(int port, int depth = 20);
		void
			Close();

		void
			Select(int seconds, int microseconds);

		virtual void
			OnRead();
		virtual TcpClient *
			OnAccept(SOCKET s, const sockaddr_in & addr);
		
		TcpClient *
			GetClient(int i) { return mClients[i]; }
		int
			GetClientSize() { return mClients.Size(); }
		void
			RemoveClient(TcpClient * client);

	protected:
		Array<TcpClient *> mClients;
	};
}
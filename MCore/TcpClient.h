/*
*	TCP Client
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "TcpSocket.h"
#include "MThread.h"
#include "MEvent.h"

namespace Rad {

	class M_ENTRY TcpClient : public TcpSocket
	{
	public:
		TcpClient();
		virtual ~TcpClient();

		bool
			Open(const char * host, int port);
		bool
			Open(SOCKET s, const sockaddr_in & addr);
		
		void 
			Close(int time);
		bool
			IsClosing() { return mClosingTimer > 0; }
		virtual void
			OnClose() {}

		bool
			Connect(int timeout = 5);
		bool
			IsConnecting() { return mConnectingTimer > 0; }
		bool
			CheckConnect();
		virtual void
			OnConnect() {}

		void
			Select(int seconds, int microseconds);
		
		virtual void
			Refresh();
		virtual void
			OnRead();
		virtual void
			OnWrite();
		
		SocketIStream *
			IStream() { return &mIStream; }
		SocketOStream *
			OStream() { return &mOStream; }

	protected:
		SocketIStream mIStream;
		SocketOStream mOStream;

		time_t mTime;
		time_t mConnectingTimer;
		time_t mClosingTimer;
	};

}
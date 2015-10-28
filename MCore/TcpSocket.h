/*
*	TCP Socket
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSocket.h"

namespace Rad {

	class M_ENTRY TcpSocket : public Socket
	{
	public:
		TcpSocket();
		virtual ~TcpSocket();

		void 
			SetReuseAddr(bool enable);
		void 
			SetKeepActive(bool enable);
		void 
			SetSendBufferSize(int size);
		void 
			SetRecvBufferSize(int size);
		void 
			SetNonBlocking();
		void 
			SetLinger(int time);
		const char * 
			GetIp();

		virtual void
			Refresh();
		virtual void
			OnRead() {}
		virtual void
			OnWrite() {}
		virtual void
			OnExpect() {}

	protected:
		bool i_nonblocking;
	};
	

}

/*
*	Socket API
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include <time.h>

#ifdef _WIN32

#ifndef FD_SETSIZE
#define FD_SETSIZE 1024
#endif

#include <winsock2.h>

#else

#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

typedef	int SOCKET;
#define INVALID_SOCKET   -1
#define SOCKET_ERROR	 -1

#endif

#include "MDebug.h"
#include "MMemory.h"
#include "MString.h"

namespace Rad {

	inline bool socket_init()
	{
#ifdef M_PLATFORM_WIN32
		WORD version = MAKEWORD(2, 1);
		WSADATA data;

		if (WSAStartup(version, &data) != 0)
			return FALSE;

		if (LOBYTE(data.wVersion) != LOBYTE(version) || 
			HIBYTE(data.wVersion) != HIBYTE(version))
		{
			WSACleanup();
			return false;
		}
#endif

		return true;
	}

	inline void socket_shutdown()
	{
#ifdef M_PLATFORM_WIN32
		WSACleanup();
#endif
	}

	inline SOCKET socket_ex (int domain, int type , int protocol ) 
	{
		return socket(domain,type,protocol);
	}

	inline const char * socket_ip(sockaddr_in * addr)
	{
		static char ip_str[128];

		in_addr n;

		n.s_addr = addr->sin_addr.s_addr;

		strcpy_s(ip_str, 128, inet_ntoa(n));

		return ip_str;
	}

	inline int bind_ex ( SOCKET s , const struct sockaddr * addr , int addrlen ) 
	{
		return bind(s , addr , addrlen);
	}

	inline int connect_ex ( SOCKET s , const struct sockaddr * addr , int addrlen )
	{
		return connect(s,addr,addrlen);
	}

	inline int listen_ex ( SOCKET s , int backlog ) 
	{
		return listen(s, backlog);
	}

	inline SOCKET accept_ex ( SOCKET s , struct sockaddr * addr , int * addrlen )
	{
#ifdef M_PLATFORM_WIN32
		return accept(s , addr , (int*)addrlen);
#else
		return accept(s , addr , addrlen);
#endif
	}

	inline int getsockopt_ex ( SOCKET s , int level , int optname , void * optval , int * optlen )
	{
#ifdef M_PLATFORM_WIN32
		return getsockopt( s , level , optname , (CHAR*)optval , (int*)optlen );
#else
		return getsockopt( s , level , optname , optval , optlen );
#endif
	}

	inline int setsockopt_ex ( SOCKET s , int level , int optname , const void * optval , int optlen )
	{
#ifdef M_PLATFORM_WIN32
		return setsockopt( s , level , optname , (CHAR*)optval , optlen );
#else
		return setsockopt( s , level , optname , optval , optlen );
#endif
	}

	inline int send_ex ( SOCKET s , const void * buf , int len , int flags )
	{
#ifdef M_PLATFORM_WIN32
		return send(s,(const CHAR *)buf,len,flags);
#else
		return send(s,buf,len,flags);
#endif
	}

	inline int sendto_ex ( SOCKET s , const void * buf , int len , int flags , const struct sockaddr * to , int tolen )
	{
#ifdef M_PLATFORM_WIN32
		return sendto(s,(const CHAR *)buf,len,flags,to,tolen);
#else
		return sendto(s,buf,len,flags,to,tolen);
#endif
	}

	inline int recv_ex ( SOCKET s , void * buf , int len , int flags )
	{
#ifdef M_PLATFORM_WIN32
		return recv(s,(CHAR*)buf,len,flags);
#else
		return recv(s,buf,len,flags);
#endif
	}

	inline int recvfrom_ex ( SOCKET s , void * buf , int len , int flags , struct sockaddr * from , int * fromlen )
	{
#ifdef M_PLATFORM_WIN32
		return recvfrom(s,(CHAR*)buf,len,flags,from,(int*)fromlen);
#else
		return recvfrom(s,buf,len,flags,from,fromlen);
#endif
	}

	inline int closesocket_ex (SOCKET s)
	{
#ifdef M_PLATFORM_WIN32
		return closesocket(s);
#else
		return close(s);
#endif
	}

	inline int nonblocking_ex (SOCKET s)
	{
#ifdef M_PLATFORM_WIN32
		ULONG argp = 1;
		return ioctlsocket(s, FIONBIO, &argp);
#else
		return fcntl(s, F_SETFL, O_NONBLOCK);
#endif
	}

	inline int ioctlsocket_ex(SOCKET s, int command, void * arg)
	{
#ifdef M_PLATFORM_WIN32
		return ioctlsocket(s, command, (u_long *)arg);
#else
		return ioctl(s, command, arg);
#endif
	}

	inline int select_ex ( int maxfdp1 , fd_set * readset , fd_set * writeset , fd_set * exceptset , struct timeval * timeout )
	{
		return select( maxfdp1 , readset , writeset , exceptset, timeout);
	}

}


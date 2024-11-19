#include "stdafx.h"
#include "NetworkUtil.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetworkCore.h"
#include "NetworkIO.h"

bool NetworkUtil::bind(SOCKET socket, const char* ip, int port)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    
    if(FALSE == ::bind(socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)))
    {
        // TODO: 오류 LOG
        return false;
    }

    return true;
}

bool NetworkUtil::listen(SOCKET socket, int backlog)
{
    if(FALSE == ::listen(socket, backlog))
        return false;

    return true;
}

bool NetworkUtil::accept(std::shared_ptr<NetworkCore> network_core, SOCKET listen_socket, std::shared_ptr<AcceptIO> io)
{
    network_core->register_socket_in_iocp_handle(listen_socket);
    
    constexpr DWORD addr_length = sizeof(sockaddr_in) + 16;
    DWORD dwBytes = 0;
    
    if(false == ::AcceptEx(listen_socket, io->m_socket, reinterpret_cast<PVOID>(io->m_accept_buffer),0
        , addr_length, addr_length,
        &dwBytes, reinterpret_cast<LPOVERLAPPED>(&io)))
    {
		const int err_no = ::WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
		{
		    // TODO: Error Log
			return false;
		}
	}
    
	return true;
}

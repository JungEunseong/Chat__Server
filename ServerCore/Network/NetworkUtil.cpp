#include "pch.h"
#include "NetworkUtil.h"

bool NetworkUtil::bind(SOCKET socket, const char* ip, int port)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(addr.sin_addr.s_addr));
    addr.sin_port = htons(port);
    
    if(FALSE == ::bind(socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)))
    {
        // TODO: ERROR LOG
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

bool NetworkUtil::accept(SOCKET listen_socket, AcceptIO* io)
{
    
    constexpr DWORD addr_length = sizeof(sockaddr_in) + 16;
    DWORD dwBytes = 0;
    
    if(false == ::AcceptEx(listen_socket, io->m_socket, reinterpret_cast<PVOID>(io->m_accept_buffer),0
        , addr_length, addr_length,
        &dwBytes, reinterpret_cast<LPOVERLAPPED>(io)))
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

bool NetworkUtil::connect(std::shared_ptr<NetworkCore> network_core, SOCKET socket, ConnectIO* io)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, io->m_ip.c_str(), &(addr.sin_addr.s_addr));
    addr.sin_port = htons(io->m_port);
    
    if(false == ::WSAConnect(socket,reinterpret_cast<sockaddr*>(&addr), sizeof(addr), nullptr, nullptr, nullptr, nullptr))
    {
        const int err_no = ::WSAGetLastError();
        if(WSA_IO_PENDING != err_no)
        {
            // TODO: Error LOG
            return false;
        }
    }

    return true;
}

bool NetworkUtil::receive(SOCKET socket, RecvIO* io)
{
    WSABUF buf;
    buf.buf = io->m_session->get_recv_buffer().GetReadPos();
    buf.len = io->m_session->get_recv_buffer().GetRemainingSize();

    DWORD recv_bytes = 0;
    DWORD flag = 0;

    if(false == WSARecv(socket, &buf, 1, &recv_bytes, &flag, io, nullptr))
    {
        int err_code = ::GetLastError();

        if(err_code == WSA_IO_PENDING)
        {
            // TODO: 로그
            return false;
        }
    }
    return true;
}


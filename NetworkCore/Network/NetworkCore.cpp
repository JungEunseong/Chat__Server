#include "stdafx.h"
#include "NetworkCore.h"

NetworkCore::NetworkCore()
    :m_iocp_handle(nullptr)
{
    m_iocp_handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    if(nullptr == m_iocp_handle)
    {
        // TODO: 크래시
        return;
    }
}

void NetworkCore::init(int iocp_thread_count)
{
    for(int i = 0; i < iocp_thread_count; ++i)
        m_threads.emplace_back([this](){ iocp_thread_work(); });
}

bool NetworkCore::register_socket_in_iocp_handle(SOCKET socket)
{
    return ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), m_iocp_handle,0,0);
}

void NetworkCore::iocp_thread_work()
{
    while(m_is_running == true)
    {
        DWORD bytes_transferred = 0;
        ULONG_PTR key = 0;
        std::shared_ptr<NetworkIO> io = nullptr;
        if(false == ::GetQueuedCompletionStatus(m_iocp_handle, &bytes_transferred, &key, reinterpret_cast<LPOVERLAPPED*>(io.get()), INFINITE))
        {
            const int err_no = ::WSAGetLastError();
            // TODO: error log
        }

        switch(io->GetType())
        {
        case IoType::CONNECT:
            OnConnect(bytes_transferred, io);
            break;
        case IoType::DISCONNECT:
            OnDisconnect(bytes_transferred, io);
            break;
        case IoType::ACCEPT:
            OnAccept(bytes_transferred, io);
            break;
        case IoType::RECV:
            OnRecv(bytes_transferred, io);
            break;
        case IoType::SEND:
            OnSend(bytes_transferred, io);
            break;
        default:
            // TODO: error log
            break;
        }
        
    }
}

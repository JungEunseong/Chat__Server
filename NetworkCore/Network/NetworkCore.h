#pragma once
#include <map>

#include "iChannel.h"

class NetworkCore : public std::enable_shared_from_this<NetworkCore>
{
public:
    NetworkCore();
    ~NetworkCore();

public:
    void init(int iocp_thread_count);
    bool register_socket_in_iocp_handle(SOCKET socket);

    void iocp_thread_work();

    virtual void OnConnect(int bytes_transferred, std::shared_ptr<NetworkIO> io) abstract;
    virtual void OnAccept(int bytes_transferred, std::shared_ptr<NetworkIO> io) abstract;
    virtual void OnRecv(int bytes_transferred, std::shared_ptr<NetworkIO> io) abstract;
    virtual void OnSend(int bytes_transferred, std::shared_ptr<NetworkIO> io) abstract;
    virtual void OnDisconnect(int bytes_transferred, std::shared_ptr<NetworkIO> io) abstract;

private:
    bool m_is_running;
    HANDLE m_iocp_handle;
    
    std::vector<std::thread> m_threads;
};


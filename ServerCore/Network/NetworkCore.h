#pragma once
class NetworkIO;

class NetworkCore : public std::enable_shared_from_this<NetworkCore>
{
public:
    NetworkCore();
    ~NetworkCore();

public:
    void init(int iocp_thread_count);
    bool register_socket_in_iocp_handle(SOCKET socket);

    void iocp_thread_work();

    virtual void on_connect(int bytes_transferred, NetworkIO* io) = 0;
    virtual void on_accept(int bytes_transferred, NetworkIO* io) = 0;
    virtual void on_recv(int bytes_transferred, NetworkIO* io) = 0;
    virtual void on_send(int bytes_transferred, NetworkIO* io) = 0;
    virtual void on_disconnect(int bytes_transferred, NetworkIO* io) = 0;

private:
    bool m_is_running;
    HANDLE m_iocp_handle;
    
    std::vector<std::thread> m_threads;
};


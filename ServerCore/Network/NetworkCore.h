#pragma once
class NetworkIO;

class NetworkCore
{
public:
    NetworkCore();
    virtual ~NetworkCore();

public:
    HANDLE get_iocp_handle() { return m_iocp_handle; }
    
public:
    void init(int iocp_thread_count);

    virtual void on_connect(int bytes_transferred, NetworkIO* io) = 0;
    virtual void on_accept(int bytes_transferred, NetworkIO* io) = 0;
    virtual void on_recv(int bytes_transferred, NetworkIO* io) = 0;
    virtual void on_send(int bytes_transferred, NetworkIO* io) = 0;
    virtual void on_disconnect(int bytes_transferred, NetworkIO* io) = 0;

public:
    bool is_running() { return m_is_running; }

protected:
    void iocp_thread_work();
    std::atomic<bool> m_is_running;
    HANDLE m_iocp_handle;
    
    std::vector<std::thread> m_threads;
};


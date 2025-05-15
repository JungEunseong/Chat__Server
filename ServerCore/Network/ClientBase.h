#pragma once

class ClientBase : public NetworkCore
{
public:
    ClientBase() = default;
    virtual ~ClientBase() = default;

public:
    void init();
    void connect(std::string connecting_ip, int connecting_port, std::function<Session*()> session_factory);

public:
    void on_connect(int bytes_transferred, NetworkIO* io);

protected:
    void on_iocp_io(NetworkIO* io, int bytes_transferred) override;

private:
    void job_thread_work();

private:
    SOCKET m_socket;

    std::thread m_job_thread;
    
    ConnectIO m_connect_io;
    Session* m_session;
};

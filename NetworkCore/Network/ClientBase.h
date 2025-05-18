﻿#pragma once

class ClientBase : public NetworkCore
{
public:
    ClientBase() = default;
    virtual ~ClientBase() = default;

public:
    void open(std::string connecting_ip, int connecting_port, std::function<class ServerSession*()> session_factory, int
              session_count);

protected:
    void on_iocp_io(NetworkIO* io, int bytes_transferred) override;

private:
    void job_thread_work();

private:
    std::thread m_job_thread;
    
    ConnectIO m_connect_io;
    std::function<class ServerSession*()> m_session_factory;

    std::map<unsigned int, class ServerSession*> m_sessions;
};

#pragma once

class ServerBase : public NetworkCore
{
public:
    ServerBase() = default;
    virtual ~ServerBase() = default;
    
public:
    void init(int iocp_thread_count = 1, int section_count = 1);
    void open(std::string open_ip, int open_port, std::function<class ClientSession*()> session_factory, int accpet_back_log = 1);

public:
    void on_accept(int bytes_transferred, NetworkIO* io);

private:
    void central_thread_work();

protected:
    void on_iocp_io(NetworkIO* io, int bytes_transferred) override;

private:
    SOCKET m_listen_socket;
    
    std::thread m_central_thread;

    std::map<unsigned int, class NetworkSection*> m_sections;
    std::function<class ClientSession*()> m_session_factory;
};

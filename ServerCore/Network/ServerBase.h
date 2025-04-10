#pragma once

class ServerBase : public NetworkCore
{
public:
    ServerBase();
    virtual ~ServerBase();
    
public:
    void init(int iocp_thread_count = 1, int section_count = 1);
    void open(std::string open_ip, int open_port, std::function<Session*()> session_factory, int accpet_back_log = 1);

public:
    void on_connect(int bytes_transferred, NetworkIO* io) override;
    void on_accept(int bytes_transferred, NetworkIO* io) override;
    void on_recv(int bytes_transferred, NetworkIO* io) override;
    void on_send(int bytes_transferred, NetworkIO* io) override;
    void on_disconnect(int bytes_transferred, NetworkIO* io) override;

private:
    void central_thread_work(); 
    
private:
    SOCKET m_listen_socket;
    
    std::thread m_central_thread;
    concurrency::concurrent_queue<Packet*> m_packet_queue;

    std::map<unsigned int, class NetworkSection*> m_sections;
    std::function<Session*()> m_session_factory;
};

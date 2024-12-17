#pragma once

class ServerBase : public NetworkCore
{
    ServerBase();
    virtual ~ServerBase();
public:
    void on_connect(int bytes_transferred, NetworkIO* io) override;
    void on_accept(int bytes_transferred, NetworkIO* io) override;
    void on_recv(int bytes_transferred, NetworkIO* io) override;
    void on_send(int bytes_transferred, NetworkIO* io) override;
    void on_disconnect(int bytes_transferred, NetworkIO* io) override;

private:
    void central_thread_work(); 
    
private:
    std::thread m_central_thread;
    concurrency::concurrent_queue<std::shared_ptr<Packet>> m_packet_queue;

    std::map<unsigned int, std::shared_ptr<NetworkSection>> m_sections;
};

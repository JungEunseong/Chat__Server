#pragma once

class ChatServerSession : public ServerSession 
{
public:
    NetworkCore* get_network_core() override;
    void on_connected() override;
    void on_send(int data_size) override;
    void on_disconnected() override;
    void execute_packet(Packet* packet) override;

private:
    void logic_thread();

private:
    std::thread m_logic_thread;
    
};
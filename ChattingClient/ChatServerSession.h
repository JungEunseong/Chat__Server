#pragma once

class ChatServerSession : public ServerSession 
{
public:
    void init() override;
public:
    NetworkCore* get_network_core() override;
    void on_connected() override;
    void on_send(int data_size) override;
    void on_disconnected() override;

private:
    void logic_thread_work();

private:
    void login_hadler(Packet* packet);

private:
    std::thread m_logic_thread;
    
};
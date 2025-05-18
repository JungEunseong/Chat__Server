#pragma once

class ChatClientSession : public ClientSession
{
public:
    ChatClientSession() = default;
    ~ChatClientSession() override = default;

public:
    NetworkCore* get_network_core() override;
    
public:
    void on_connected() override;
    void on_send(int data_size) override;
    void on_disconnected() override;
    
    void execute_packet(Packet* packet) override;
    

private:
};

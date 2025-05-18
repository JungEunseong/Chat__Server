#pragma once

class ServerSession : public Session
{
public:
    ServerSession() = default;
    ~ServerSession() override = default;
public:
    NetworkCore* get_network_core() override { return m_owner; }

public:
    void on_connected() override;
    int on_recieve() override;
    void on_send(int data_size) override;
    void on_disconnected() override;
    void execute_packet(Packet* packet) override;

private:
    class ClientBase* m_owner;
};

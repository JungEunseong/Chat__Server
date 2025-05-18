#pragma once
#include <queue>

class MultiSender
{
public:
    MultiSender(Session* session);
    ~MultiSender();

public:
    bool is_register_queue_empty() { return m_register_packet.empty(); }
public:
    bool register_packet(Packet* packet);
    bool on_send();
private:
    bool send();

private:
    Concurrency::concurrent_queue<Packet*> m_register_packet;
    std::queue<Packet*> m_sending_packet; // send중인 패킷

    std::atomic<bool> m_sending_flag;

    Session* m_owner;
    SendIO m_send_io;
};

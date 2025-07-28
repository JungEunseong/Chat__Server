#include "pch.h"
#include "ServerSession.h"

void ServerSession::on_connected()
{
}

int ServerSession::on_recieve()
{
    return Session::on_recieve();
}

void ServerSession::on_send(int data_size)
{
}

void ServerSession::on_disconnected()
{
}

bool ServerSession::do_send(iProtocol& protocol)
{
    std::shared_ptr<Packet> p = std::make_shared<Packet>();
    p->reserve_packet_buffer(sizeof(protocol) + sizeof(PACKET_HEADER_SIZEOF));
    p->initialize();
    protocol.Write(*(p.get()));
    if (true == performance_check_mode)
    {
        long long tick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        p->push(tick);
    }
    p->finalize();

    return Session::do_send(p);
}

#include "pch.h"
#include "ClientSession.h"

NetworkCore* ClientSession::get_network_core()
{
    if (m_section == nullptr)
        return nullptr;
    
    return m_section->get_network_core();
}

void ClientSession::on_connected()
{
}

int ClientSession::on_recieve()
{
    return Session::on_recieve();
}

void ClientSession::on_send(int data_size)
{
}

void ClientSession::on_disconnected()
{
    Session::on_disconnected();
    m_section->exit_section(get_id());
    m_section = nullptr;
}

void ClientSession::execute_packet(Packet* packet)
{
    Session::execute_packet(packet);

    if (performance_check_mode)
    {
        long long start_tick  = 0;
        packet->pop(start_tick);

        S2C_RES_RTT_PACKET rtt_packet;
        rtt_packet.rtt_protocol_number = packet->get_protocol();
        rtt_packet.start_tick = start_tick;

        do_send(rtt_packet);
    }

    xdelete packet;
}

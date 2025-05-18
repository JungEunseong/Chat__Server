#include "pch.h"
#include "ChatClientSession.h"

NetworkCore* ChatClientSession::get_network_core()
{
    return ClientSession::get_network_core();
}

void ChatClientSession::on_connected()
{
    ClientSession::on_connected();
}

void ChatClientSession::on_send(int data_size)
{
    ClientSession::on_send(data_size);
}

void ChatClientSession::on_disconnected()
{
    ClientSession::on_disconnected();
}

void ChatClientSession::execute_packet(Packet* packet)
{
    ClientSession::execute_packet(packet);
}

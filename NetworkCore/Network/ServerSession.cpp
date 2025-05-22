#include "pch.h"
#include "ServerSession.h"

void ServerSession::on_connected()
{
    Session::on_connected();
}

int ServerSession::on_recieve()
{
    return Session::on_recieve();
}

void ServerSession::on_send(int data_size)
{
    Session::on_send(data_size);
}

void ServerSession::on_disconnected()
{
    Session::on_disconnected();
}

void ServerSession::execute_packet(Packet* packet)
{
    Session::execute_packet(packet);
}

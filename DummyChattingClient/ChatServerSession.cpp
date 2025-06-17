#include "pch.h"
#include "ChatServerSession.h"

void dummy_client::ChatServerSession::init()
{
    ServerSession::init();
}

NetworkCore* dummy_client::ChatServerSession::get_network_core()
{
    return ServerSession::get_network_core();
}

void dummy_client::ChatServerSession::on_connected()
{
    ServerSession::on_connected();
}

void dummy_client::ChatServerSession::on_send(int data_size)
{
    ServerSession::on_send(data_size);
}

void dummy_client::ChatServerSession::on_disconnected()
{
    ServerSession::on_disconnected();
}

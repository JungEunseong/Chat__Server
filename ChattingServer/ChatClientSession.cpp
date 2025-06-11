#include "pch.h"
#include "ChatClientSession.h"


void ChatClientSession::init()
{
    ClientSession::init();
    
    m_handlers.emplace(packet_number::LOGIN, [this](auto* p){this->login_hadler(p); });
}

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

void ChatClientSession::login_hadler(Packet* packet)
{
   C2S_REQ_LOGIN recv_packet_from_client;
    recv_packet_from_client.Read(*packet);
    
    NetworkSection* section = get_section();

    S2C_RES_LOGIN send_packet_from_client;
    send_packet_from_client.nickname = recv_packet_from_client.nickname;
    section->broadcast(send_packet_from_client);
}
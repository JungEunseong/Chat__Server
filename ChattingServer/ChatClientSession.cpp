#include "pch.h"
#include "ChatClientSession.h"


void ChatClientSession::init()
{
    ClientSession::init();
    
    m_handlers.emplace(packet_number::LOGIN, [this](auto* p){this->login_hadler(p); });
    m_handlers.emplace(packet_number::CHAT_MESSAGE, [this](auto* p){this->chat_message_hadler(p); });
    
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

void ChatClientSession::login_hadler(Packet* packet)
{
   C2S_REQ_LOGIN recv_packet_from_client;
    recv_packet_from_client.Read(*packet);

    std::wcout << L"[LOG] " << m_nickname << L"log in" << std::endl;
    
    m_nickname = recv_packet_from_client.nickname;
    
    S2C_RES_LOGIN send_packet_from_client;
    send_packet_from_client.nickname = recv_packet_from_client.nickname;
    
    NetworkSection* section = get_section();
    send_packet_from_client.is_mine = false;
    section->broadcast(send_packet_from_client, this);

    send_packet_from_client.is_mine = true;
    do_send(send_packet_from_client);
}

void ChatClientSession::chat_message_hadler(Packet* packet)
{
    C2S_REQ_CHAT_MESSAGE recv_packet_from_client;
    recv_packet_from_client.Read(*packet);

    // TODO: USE LOGGER
    std::wcout << L"[LOG] " << m_nickname << L" chatted => message: " << recv_packet_from_client.message << std::endl;
    

    if (true == m_nickname.empty())
    {
        do_disconnect();
        std::wcout << "nickname is empty" << std::endl;
        return;
    }
    
    S2C_NTF_CHAT_MESSAGE send_packet_from_client;
    send_packet_from_client.nickname = m_nickname;
    send_packet_from_client.message = recv_packet_from_client.message;

    NetworkSection* section = get_section();
    section->broadcast(send_packet_from_client, this);
    
}

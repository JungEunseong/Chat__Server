#include "pch.h"
#include "ChatServerSession.h"

void chat_client::ChatServerSession::init()
{
    ServerSession::init();
    
    m_handlers.emplace(packet_number::LOGIN, [this](auto* p){this->login_hadler(p); });
    m_handlers.emplace(packet_number::CHAT_MESSAGE, [this](auto* p){this->chat_message_hadler(p); });
}

NetworkCore* chat_client::ChatServerSession::get_network_core()
{
    return ServerSession::get_network_core();
}

void chat_client::ChatServerSession::on_connected()
{
    ServerSession::on_connected();
    std::wcout << L"connect success" << std::endl;

    std::wstring name;
    std::wcin >> name;

    C2S_REQ_LOGIN send_packet_to_server;
    send_packet_to_server.nickname = name;

    if (false == do_send(send_packet_to_server))
    {
        // TODO: LOG
        do_disconnect();
        return;
    }
}

void chat_client::ChatServerSession::on_send(int data_size)
{
    ServerSession::on_send(data_size);
}

void chat_client::ChatServerSession::on_disconnected()
{
    ServerSession::on_disconnected();
}

void chat_client::ChatServerSession::logic_thread_work()
{
    while (true == m_is_connected)
    {
        std::cout << "chat input: ";
        std::wstring input;
        std::wcin >> input;

        if (input[0] == '@')
        {
            // TODO: Cheat
        }
        else
        {
            C2S_REQ_CHAT_MESSAGE send_packet_to_server;
            send_packet_to_server.message = input;

            do_send(send_packet_to_server);
        }
    }
}

void chat_client::ChatServerSession::login_hadler(Packet* packet)
{
    S2C_RES_LOGIN recv_packet_from_server;
    recv_packet_from_server.Read(*packet);

    std::wcout <<  recv_packet_from_server.nickname << L" has entered" << std::endl;

    if (recv_packet_from_server.is_mine)
        m_logic_thread = std::thread([this](){ logic_thread_work(); });
}

void chat_client::ChatServerSession::chat_message_hadler(Packet* packet)
{
    S2C_NTF_CHAT_MESSAGE recv_packet_from_server;
    recv_packet_from_server.Read(*packet);

    std:: wcout << L"[" <<  recv_packet_from_server.nickname << L"]: " << recv_packet_from_server.message << std::endl;
}

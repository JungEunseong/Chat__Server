#include "pch.h"
#include "ChatServerSession.h"

NetworkCore* ChatServerSession::get_network_core()
{
    return ServerSession::get_network_core();
}

void ChatServerSession::on_connected()
{
    ServerSession::on_connected();
    std::cout << "연결 완료" << std::endl;

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

void ChatServerSession::on_send(int data_size)
{
    ServerSession::on_send(data_size);
}

void ChatServerSession::on_disconnected()
{
    ServerSession::on_disconnected();
}

void ChatServerSession::execute_packet(Packet* packet)
{
    ServerSession::execute_packet(packet);
}

void ChatServerSession::logic_thread()
{
    while (true == m_is_connected)
    {
        std::cout << "입력: " << std::endl;
        std::string input;
        std::cin >> input;

        if (input[0] == '@')
        {
            // TODO: Cheat
        }
        else
        {
            // TODO: Chatting
        }
    }
}

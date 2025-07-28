#include "pch.h"
#include "ChatServerSession.h"

#include <random>
#include <string>

void dummy_client::ChatServerSession::init()
{
    ServerSession::init();
    static int dummy_client_nickname = 0;

    m_nickname = std::to_wstring(++dummy_client_nickname);

    m_handlers.emplace(packet_number::RTT_PACKET, [this](auto* p){this->rtt_packet_hadler(p); });
    m_handlers.emplace(packet_number::LOGIN, [this](auto* p){this->login_hadler(p); });
    m_handlers.emplace(packet_number::CHAT_MESSAGE, [this](auto* p){this->chat_message_hadler(p); });
    
}

NetworkCore* dummy_client::ChatServerSession::get_network_core()
{
    return ServerSession::get_network_core();
}

void dummy_client::ChatServerSession::on_connected()
{
    ServerSession::on_connected();

    std::wcout << L"connected" << std::endl;
    C2S_REQ_LOGIN send_packet_to_server;
    send_packet_to_server.nickname = m_nickname;

    if (false == do_send(send_packet_to_server))
    {
        std::wcout << "do send fail" << std::endl;
        do_disconnect();
        return;
    }
}

void dummy_client::ChatServerSession::on_send(int data_size)
{
    ServerSession::on_send(data_size);
}

void dummy_client::ChatServerSession::on_disconnected()
{
    ServerSession::on_disconnected();
}

void dummy_client::ChatServerSession::login_hadler(Packet* packet)
{
    S2C_RES_LOGIN recv_packet_from_server;
    recv_packet_from_server.Read(*packet);

    //std::wcout <<  recv_packet_from_server.nickname << L" has entered" << std::endl;

    if (recv_packet_from_server.is_mine)
        m_logic_thread = std::thread([this](){ logic_thread_work(); });
}

void dummy_client::ChatServerSession::chat_message_hadler(Packet* packet)
{
    S2C_NTF_CHAT_MESSAGE recv_packet_from_server;
    recv_packet_from_server.Read(*packet);

    //std:: wcout << L"[" <<  recv_packet_from_server.nickname << L"]: " << recv_packet_from_server.message << std::endl;
}

void dummy_client::ChatServerSession::rtt_packet_hadler(Packet* packet)
{
    S2C_RES_RTT_PACKET recv_packet_from_server;
    recv_packet_from_server.Read(*packet);

    long long current_tick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    DummyChattingClientService* service =  static_cast<DummyChattingClientService*>(get_network_core());
    if (nullptr != service)
    {
        long long rtt_tick = current_tick - recv_packet_from_server.start_tick;
        service->push_rtt_data(recv_packet_from_server.rtt_protocol_number, current_tick - recv_packet_from_server.start_tick);
    }
}

void dummy_client::ChatServerSession::logic_thread_work()
{
    std::srand(static_cast<unsigned int>(time(0)));
    
    while (true == m_is_connected)
    {
        std::wstring msg;

        msg = std::to_wstring(std::rand());
      
        C2S_REQ_CHAT_MESSAGE send_packet_to_server;
        send_packet_to_server.message = msg;

        do_send(send_packet_to_server);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void dummy_client::ChatServerSession::execute_packet(Packet* packet)
{
    ServerSession::execute_packet(packet);
   
}

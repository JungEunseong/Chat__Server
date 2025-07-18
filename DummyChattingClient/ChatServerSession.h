#pragma once

namespace dummy_client
{
    class ChatServerSession : public ServerSession
    {
    public:
        ChatServerSession() = default;
        ~ChatServerSession() override = default;
    
    public:
        void init() override;

    public:
        NetworkCore* get_network_core() override;
        void on_connected() override;
        void on_send(int data_size) override;
        void on_disconnected() override;

    public:
        void login_hadler(Packet* packet);
        void chat_message_hadler(Packet* packet);
        void rtt_packet_hadler(Packet* packet);

    private:
        void logic_thread_work();

    public:
        void execute_packet(Packet* packet) override;

    private:

        std::thread m_logic_thread;
        std::wstring m_nickname;
    };
}
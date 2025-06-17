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
    };
}
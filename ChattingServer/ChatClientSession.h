﻿#pragma once

class ChatClientSession : public ClientSession
{
public:
    ChatClientSession() = default;
    ~ChatClientSession() override = default;
public:
    void init() override;

public:
    NetworkCore* get_network_core() override;
    
public:
    void on_connected() override;
    void on_send(int data_size) override;
    void on_disconnected() override;
    
    void execute_packet(Packet* packet) override;

public:
    void login_hadler(Packet* packet);

private:

    std::wstring m_nickname;
};

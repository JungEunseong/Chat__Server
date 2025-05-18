#pragma once

class Lobby : public NetworkSection
{
public:
    Lobby() = default;
    ~Lobby() override = default;
public:
    void enter_section(ClientSession* session) override;
    void exit_section(int session_id) override;

    
};

#pragma once

class ChatServerService : public ServerBase
{
public:
    ChatServerService() = default;
    ~ChatServerService() override = default;

public:
    void init(int iocp_thread_count = 1, int section_count = 1) override;

protected:
    void on_iocp_io(NetworkIO* io, int bytes_transferred) override;
    NetworkSection* select_first_section() override;
    
private:
    class Lobby* m_lobby;
};

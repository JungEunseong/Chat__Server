#pragma once

class ChatServerService : public ServerBase
{
public:
    ChatServerService() = default;
    ~ChatServerService() override = default;

protected:
    void on_iocp_io(NetworkIO* io, int bytes_transferred) override;

private:
    
};

#pragma once

class ChatClientService : public ClientBase
{
public:
    ChatClientService() = default;
    ~ChatClientService() override = default;

protected:
    void on_iocp_io(NetworkIO* io, int bytes_transferred) override;
};

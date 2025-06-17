#pragma once

class DummyChattingClientService : public ClientBase
{
public:
    DummyChattingClientService() = default;
    ~DummyChattingClientService() override = default;

protected:
    void on_iocp_io(NetworkIO* io, int bytes_transferred) override;
};

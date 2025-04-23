#pragma once

enum IoType
{
    CONNECT,
    DISCONNECT,
    ACCEPT,
    RECV,
    SEND,
};

class NetworkIO : public OVERLAPPED
{
public:
    NetworkIO(IoType type) : io_type(type){ Init(); }
    ~NetworkIO() = default;

    void Init()
    {
        ::memset(this,0, sizeof(OVERLAPPED));
    }

public:
    IoType GetType() { return io_type; }
private:
    IoType io_type;
};

class AcceptIO : public NetworkIO
{
public:
    AcceptIO() : NetworkIO(IoType::ACCEPT) { }

    SOCKET m_socket;
    char m_accept_buffer[1024];
};

class ConnectIO : public NetworkIO
{
public:
    ConnectIO() : NetworkIO(IoType::CONNECT) { }

    std::string m_ip;
    int m_port;
};

class RecvIO : public NetworkIO
{
public:
    RecvIO() : NetworkIO(IoType::RECV) { }

    class Session* m_session;
};

class SendIO : public NetworkIO
{
public:
    SendIO() : NetworkIO(IoType::SEND) { }

    void Clear()
    {
        Init();
        m_buffers.clear();
    }

    Session* m_session;
    std::vector<WSABUF> m_buffers;
};

class DisconnectIO : public NetworkIO
{
public:
    DisconnectIO() : NetworkIO(IoType::DISCONNECT) { }
    
    Session* m_session;
};
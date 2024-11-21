﻿#pragma once

enum IoType
{
    CONNECT,
    DISCONNECT,
    ACCEPT,
    RECV,
    SEND,
};

struct NetworkIO : public OVERLAPPED
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

struct AcceptIO : public NetworkIO
{
public:
    AcceptIO() : NetworkIO(IoType::ACCEPT) { }

    SOCKET m_socket;
    char m_accept_buffer[1024];
    
};

struct ConnectIO : public NetworkIO
{
public:
    ConnectIO() : NetworkIO(IoType::CONNECT) { }

    std::string ip;
    int port;
};

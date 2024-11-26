#pragma once

class Session
{
public:
    Session() = default;
    virtual ~Session() = default;
    
public:
    void SetSocket(SOCKET socket){ m_connecting_socket = socket; }
    void SetRemoteIp(const char* ip){ m_remote_ip = std::string(ip); }
    void SetRemoteIp(std::string&& ip){ m_remote_ip = ip; }
    void SetRemotePort(int port){ m_remote_ip = port; }

public:
    virtual void OnConnected();
    virtual void OnRecv(char* buffer, int data_size);
    virtual void OnSend(int data_size);
    virtual void OnDisconnected();

private:
    SOCKET m_connecting_socket;
    std::string m_remote_ip;
    int m_remote_port;

    RecvBuffer m_recv_buffer;
};

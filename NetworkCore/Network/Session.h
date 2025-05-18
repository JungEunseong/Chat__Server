#pragma once
#include "RecvBuffer.h"

class Session
{
public:
    Session() : m_multi_sender(this) { }
    virtual ~Session() = default;

public:
    static int generate_session_id();
public:
    void init();
public:
    int get_id() { return m_session_id; };
    void set_id(int id) { m_session_id = id; };

    SOCKET get_socket() { return m_connecting_socket; }
    void set_socket(SOCKET socket){ m_connecting_socket = socket; }
    void set_remote_ip(const char* ip){ m_remote_ip = std::string(ip); }
    void set_remote_ip(std::string&& ip){ m_remote_ip = ip; }
    void set_remote_port(int port){ m_remote_ip = port; }
    virtual NetworkCore* get_network_core() abstract;
public:
    bool do_connect();
    bool do_recieve();
    bool do_send(Packet* packet);
    bool do_disconnect();

    void complete_connect(int bytes_transferred);
    void complete_recieve(int bytes_transferred);
    void complete_send(int bytes_transferred);
    void complete_disconnect();
public:
    virtual void on_connected() abstract;
    virtual int on_recieve();
    virtual void on_send(int data_size) abstract;
    virtual void on_disconnected() abstract;
    virtual void execute_packet(Packet* packet) abstract;
    

    RecvBuffer& get_recv_buffer() { return m_recv_buffer; }
    RecvIO& get_recv_io(){ return m_recv_io; }
protected:
    int m_session_id;
    
    SOCKET m_connecting_socket;
    std::string m_remote_ip;
    int m_remote_port;

    RecvBuffer m_recv_buffer;
    MultiSender m_multi_sender;
    ConnectIO m_connect_io;
    RecvIO m_recv_io;
    DisconnectIO m_disconnect_io;
};

#pragma once
#include "RecvBuffer.h"

class Session
{
public:
    Session() = default;
    virtual ~Session() = default;

public:
    static int generate_session_id();
public:
    int get_id() { return m_session_id; };
    void set_id(int id) { m_session_id = id; };

    SOCKET get_socket() { return m_connecting_socket; }
    void set_socket(SOCKET socket){ m_connecting_socket = socket; }
    void set_remote_ip(const char* ip){ m_remote_ip = std::string(ip); }
    void set_remote_ip(std::string&& ip){ m_remote_ip = ip; }
    void set_remote_port(int port){ m_remote_ip = port; }
public:
    bool do_connect();
    bool do_recieve();
    bool do_send(Packet* packet);
    bool do_disconnect();

private:
    void complete_connect();
    void complete_recieve();
    void complete_send(int send_size);
    void complete_disconnect();
public:
    virtual void on_connected();
    virtual void on_recv(std::shared_ptr<Packet> packet);
    virtual void on_send(int data_size);
    virtual void on_disconnected();
    virtual void execute_packet(std::shared_ptr<Packet>) = 0;
    

    RecvBuffer& get_recv_buffer() { return m_recv_buffer; }

    unsigned int get_section_id();
    std::shared_ptr<class NetworkSection> get_section()  { return m_section; }
    void set_section(std::shared_ptr<class NetworkSection> section) { m_section = section; }

    RecvIO& get_recv_io(){ return m_recv_io; }
private:
    int m_session_id;
    
    SOCKET m_connecting_socket;
    std::string m_remote_ip;
    int m_remote_port;

    std::shared_ptr<class NetworkSection> m_section;
    
    RecvBuffer m_recv_buffer;
    MultiSender m_multi_sender;
    RecvIO m_recv_io;
};

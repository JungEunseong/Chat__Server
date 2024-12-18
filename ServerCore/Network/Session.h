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
    
    void set_socket(SOCKET socket){ m_connecting_socket = socket; }
    void set_remote_ip(const char* ip){ m_remote_ip = std::string(ip); }
    void set_remote_ip(std::string&& ip){ m_remote_ip = ip; }
    void set_remote_port(int port){ m_remote_ip = port; }
public:
    void do_connect();
    void do_recieve();
    void do_send();
    void do_disconnect();

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
private:
    int m_session_id;
    
    SOCKET m_connecting_socket;
    std::string m_remote_ip;
    int m_remote_port;

    std::shared_ptr<class NetworkSection> m_section;
    RecvBuffer m_recv_buffer;
};
